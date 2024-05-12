#pragma once
#include "Menu.hpp"
#include "Tile.hpp"
#include "BSSoundHandle.hpp"

// 2C
struct HackingText
{
	enum
	{
		kDisplaysCharacterByCharacter = 0x1,
		kBlinks = 0x2,
	};

	String str;
	Tile*	pkTileText;
	Tile::Value*	pkTileValueVisibility;
	UInt32 displayRate;
	UInt32 flashOnDuration;
	UInt32 flashOffDuration;
	UInt32 nextDisplayTime;
	UInt32 nextBlinkTime;
	UInt32 currentlyDisplayedText;
	UInt8 flags;
	UInt8 isTextRemaining;
	UInt8 gap2A[2];
};
static_assert(sizeof(HackingText) == 0x2C);

class HackingMenu : public Menu
{
public:
	HackingMenu();
	~HackingMenu();

	enum Stages
	{
		kFailed = 3,
		kSuccess = 4
	};

	enum
	{
		kHackingMenu_NumHeaders = 4, // the number of lines printed above the hacking minigame (ROBCO INDUSTRIES, ENTER PASSWORD NOW etc.)
		kHackingMenu_LineLength = 0xC,
		kHackingMenu_AddressLength = 7, // e.g. '0xF688 '
		kHackingMenu_EmptyPWord = 0x198,
	};

	struct Entry
	{
		HackingText text;
		Tile*	pkTile;
		Tile* templateTile;
	};

	Entry* lines[39];
	UInt32 unk0C4[7];
	HackingText bottomLine;
	Tile* hacking_intro_rect;
	Tile* hacking_header_rect;
	Tile* hacking_password_file_rect;
	Tile* hacking_password_log_rect;
	Tile* hacking_cursor;
	Tile* hacking_password_entry;
	Tile* hacking_locked_message_text_1;
	Tile* hacking_locked_message_text_2;
	BSSoundHandle UIHackingFanHumLP;
	BSSoundHandle UIHackingPassGood;
	BSSoundHandle UIHackingPassBad;
	BSSoundHandle UIHackingCharScroll;
	BSSoundHandle UIHackingCharEnter;
	UInt32 printedLines_TileChildList[3];
	UInt32 tileChildList174[3];
	BSSimpleList<char> words;
	UInt32 tileChildList188[3];
	TESObjectREFR* targetBaseRef;
	TESObjectREFR* targetRef;
	char* correctAnswer;
	UInt32 attemptsRemaining;
	UInt32 maxAttemptsRemaining;
	UInt32 numWordsRemaining;
	UInt32 correctAnswerLength;
	UInt32* wordPositions;
	UInt32 topTextHeight;
	UInt32 topTextLineHeight;
	UInt32 highlightedCharacterXIndex;
	UInt32 activeTile;
	UInt32 stage;
	TileImage* hacking_password_file_target;
	UInt32 controllerSelectedTileIdx;
	UInt32 transitionToComputersMenuTime;
	Tile::Value* hacking_cursor_visibleTrait;
	UInt8 shouldNotHighlightWordOnHover;
	UInt8 hasAllowanceBeenReplenished;
	UInt8 hasShownHelp;
	UInt8 pad1DB;

	static HackingMenu* GetSingleton() { return *reinterpret_cast<HackingMenu**>(0x11D95B8); }

	void RemoveDud(const char* word);
};
static_assert(sizeof(HackingMenu) == 0x1DC);