#pragma once
#include <defs/minimal_geode.hpp>

#include <unordered_map>
#include <asp/sync.hpp> // mutex

#include <util/crypto.hpp> // base64
#include <util/singleton.hpp>

#include <asp/time/SystemTime.hpp>

struct GameServer {
    std::string id;
    std::string name;
    std::string region;
    std::string address;

    int ping;
    uint32_t playerCount;
};

// This class is fully thread safe to use.
class GameServerManager : public SingletonBase<GameServerManager> {
protected:
    friend class SingletonBase;
    GameServerManager();

public:
    constexpr static const char* STANDALONE_ID = "__standalone__server_id__";
    constexpr static const char* STANDALONE_SETTING_KEY = "_last-standalone-addr";
    constexpr static const char* LAST_CONNECTED_SETTING_KEY = "_last-connected-addr";
    constexpr static const char* SERVER_RESPONSE_CACHE_KEY = "_last-cached-servers-response";

    asp::AtomicBool pendingChanges;

    // Returns true if a new server has been added, otherwise false.
    Result<> addServer(std::string_view serverId, std::string_view name, std::string_view address, std::string_view region);
    // Returns true if a new server has been added, or the data has changed, false otherwise.
    Result<bool> addOrUpdateServer(std::string_view serverId, std::string_view name, std::string_view address, std::string_view region);
    void clear();
    size_t count();

    void setActive(std::string_view id);
    std::string getActiveId();
    void clearActive();

    // remove all servers except the one with the given id
    void clearAllExcept(std::string_view id);

    std::optional<GameServer> getActiveServer();
    std::optional<GameServer> getServer(std::string_view id);
    std::unordered_map<std::string, GameServer> getAllServers();

    // return ping on the active server
    int getActivePing();

    // save the given address as a last connected standalone address
    void saveStandalone(std::string_view addr);
    std::string loadStandalone();

    void saveLastConnected(std::string_view addr);
    std::string loadLastConnected();

    void updateCache(std::string_view response);
    void clearCache();
    Result<> loadFromCache();

    /* pings */

    uint32_t startPing(std::string_view serverId);
    void finishPing(uint32_t pingId, uint32_t playerCount);

    void startKeepalive();
    void finishKeepalive(uint32_t playerCount);

    // backups (for connection test)
    void backupInternalData();
    void restoreInternalData();

protected:
    // expansion of GameServer with pending pings
    struct GameServerData {
        GameServer server;
        std::unordered_map<uint32_t, asp::time::SystemTime> pendingPings;
    };

    struct InnerData {
        std::unordered_map<std::string, GameServerData> servers;
        std::string active; // current game server ID
        uint32_t activePingId;
        std::string cachedServerResponse;
    };

    asp::Mutex<InnerData> _data;
    asp::Mutex<InnerData> _dataBackup;
};