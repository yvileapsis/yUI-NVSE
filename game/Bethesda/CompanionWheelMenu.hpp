#pragma once
#include "Menu.hpp"

// 88
class CompanionWheelMenu : Menu
{
public:
	CompanionWheelMenu();
	~CompanionWheelMenu();

	enum Buttons
	{
		kAggressivePassive = 0x0,
		kUseStimpak = 0x1,
		kStayFollow = 0x2,
		kTalkTo = 0x3,
		kBackUp = 0x4,
		kNearFar = 0x5,
		kOpenInventory = 0x6,
		kRangedMelee = 0x7,
		kExit = 0xB,

		CWM_RadialLabel = 0x8,
		CWM_ButtonPreviewText = 0x9,
		CWM_ButtonContextText = 0xA,
		CWM_Subtitle = 0xC,
		CWM_Callout_Exit = 0xD,
		CWM_Callout_Select = 0xE,
		CWM_Callout_Navigate = 0xF,
	};

	Tile*	tiles[16];
	Actor*	companionRef;
	UInt8	isIsFollowingLong;
	UInt8	isFollowerSwitchAggressive;
	UInt8	isCombatStyleRanged;
	UInt8	isWaiting;
	UInt32	lastSelectedTileID;
	UInt8	talkToActorWhenClosingMenu;
	UInt8	gap75[3];
	UInt32	time78;
	Sound	sound7C;
	static CompanionWheelMenu* GetSingleton() { return *reinterpret_cast<CompanionWheelMenu**>(0x11D92B8); };
};
static_assert(sizeof(CompanionWheelMenu) == 0x88);