#pragma once
#include <defs/geode.hpp>

#include "room_listing_cell.hpp"
#include <data/types/room.hpp>
#include <ui/general/list/list.hpp>

class RoomListingPopup : public geode::Popup<> {
protected:
    using RoomList = GlobedListLayer<RoomListingCell>;

    friend class RoomListingCell;

    static constexpr float POPUP_WIDTH = 390.f;
    static constexpr float POPUP_HEIGHT = 240.f;
    static constexpr float LIST_WIDTH = POPUP_WIDTH * 0.9f;
    static inline const cocos2d::CCSize contentSize = {LIST_WIDTH, 160.f};

	bool setup() override;

    RoomList* listLayer = nullptr;
    cocos2d::extension::CCScale9Sprite* background;
    bool modActionsOn = false;
    size_t shownRoomCount = 0;

    void onReload(cocos2d::CCObject* sender);
    void createCells(std::vector<RoomListingInfo> rlp);
    void toggleModActions(bool enabled);
    void updateTitle(size_t roomCount);
    void decrementRoomCountInTitle();

public:
	static RoomListingPopup* create();
    void close();
};
