#pragma once
#include "BSSimpleList.hpp"
#include "NiTList.hpp"
#include "BSString.hpp"
#include "NiColor.hpp"
#include "TileMenu.hpp"
#include "BSMemObject.hpp"

class Tile;
class TileMenu;
class TileExtra;
class TileImage;
class TileText;
class TileRect;

enum MenuSpecialKeyboardInputCode
{
	kMenu_Enter = -2,
	kMenu_UpArrow = 0x1,
	kMenu_DownArrow = 0x2,
	kMenu_RightArrow = 0x3,
	kMenu_LeftArrow = 0x4,
	kMenu_Space = 0x9,
	kMenu_Tab = 0xA,
	kMenu_ShiftEnter = 0xB,
	kMenu_AltEnter = 0xC,
	kMenu_ShiftLeft = 0xD,
	kMenu_ShiftRight = 0xE,
	kMenu_PageUp = 0xF,
	kMenu_PageDown = 0x10,
};

// 0x28
class Menu : public BSMemObject
{
public:

	enum Type : UInt32
	{
		kType_Min			= 0x3E9,
		kType_Message		= kType_Min,
		kType_Inventory,
		kType_Stats,
		kType_HUDMain,
		kType_Loading		= 0x3EF,
		kType_Container,
		kType_Dialog,
		kType_SleepWait		= 0x3F4,
		kType_Start,
		kType_LockPick,
		kType_Quantity		= 0x3F8,
		kType_Map			= 0x3FF,
		kType_Book			= 0x402,
		kType_LevelUp,
		kType_Repair		= 0x40B,
		kType_RaceSex,
		kType_Credits		= 0x417,
		kType_CharGen,
		kType_TextEdit		= 0x41B,
		kType_Barter		= 0x41D,
		kType_Surgery,
		kType_Hacking,
		kType_VATS,
		kType_Computers,
		kType_RepairServices,
		kType_Tutorial,
		kType_SpecialBook,
		kType_ItemMod,
		kType_LoveTester	= 0x432,
		kType_CompanionWheel,
		kType_TraitSelect,
		kType_Recipe,
		kType_SlotMachine	= 0x438,
		kType_Blackjack,
		kType_Roulette,
		kType_Caravan,
		kType_Trait			= 0x43C,
		kType_Max			= kType_Trait,
	};

	// check 1 at 0xA0B174, 0x70D529, 0x70D592 :: set at 0x712224
	// check 2 at 0x711FF1 :: set 2 at 0xA1D987 (when closes menu), 0xA1DA41
	// check 4 at 0xA1D9EC (when closing menu) :: set at 0x7036A4, 0x71204D
	// check 8 at 0x712194 :: set 8 at 0xA1DB8F (when opening menu), 0x720B39
	enum State : UInt32
	{
		kMenuStateIsReady	= 1 << 0,
		kMenuStateIsFadeOut	= 1 << 1,
		kMenuStateIsInit	= 1 << 2,
		kMenuStateIsFadeIn	= 1 << 3,
	};


	Menu() { ThisCall(0xA1C4A0, this); };
	virtual ~Menu() { ThisCall(0xA1C520, this); };

	virtual void	SetTile(UInt32 auiTileID, Tile* apkActiveTile) {};
	virtual void	HandleClick(UInt32 auiTileID, Tile* apkActiveTile) {};
	virtual void	HandleUnclick(UInt32 auiTileID, Tile* apkActiveTile) {};	// buttonID = <id> trait defined in XML
	virtual void	HandleMouseover(UInt32 auiTileID, Tile* apkActiveTile) {};	//called on mouseover, activeTile is moused-over Tile
	virtual void	HandleUnmouseover(UInt32 auiTileID, Tile* apkActiveTile) {};
	virtual void	PostDragTileChange(UInt32 auiTileID, Tile* apkNewTile, Tile* apkActiveTile) {}; // unused by vanilla menus
	virtual void	PreDragTileChange(UInt32 auiTileID, Tile* apkOldTile, Tile* apkActiveTile) {}; // unused by vanilla menus
	virtual void	HandleActiveMenuClickHeld(UInt32 auiTileID, Tile* apkActiveTile) {}; // StartMenu, RaceSexMenu, VATSMenu, BookMenu
	virtual void	OnClickHeld(UInt32 auiTileID, Tile* apkActiveTile) {}; // unused by vanilla menus
	virtual void	HandleMousewheel(UInt32 auiTileID, Tile* apkActiveTile) {};
	virtual void	Update() {}; // Called every frame while the menu is active
	virtual bool	HandleKeyboardInput(UInt32 aiInputChar) { return false; };	//for keyboard shortcuts, return true if handled
	virtual UInt32	GetID() { return 0; };
	virtual bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode aeCode, Float32 afKeyState) { return false; };
	virtual bool	HandleControllerInput(int aiCode, Tile* apkActiveTile) { return false; };
	virtual void	OnUpdateUserTrait(int aiTileVal) {}; // unimplemented by any vanilla menus, called at 0xA1F28E - called when a tilevalue > 10000 is used (any field starting with an underscore like <_XXX></_XXX> is a user trait)
	virtual void	HandleControllerConnectOrDisconnect(bool abIsControllerConnected) {};

	// 0x14
	struct TemplateInstance {
		UInt32				unk00;
		Float32				flt04;
		BSStringT<char>		kTileName;
		Tile*				pkTile;
	};

	// 0x14
	struct TemplateData {
		const char*					pcTemplateName;
		TileExtra*					pkTileExtra;
		NiTList<TemplateInstance*>	instances;
	};

	TileMenu*					pkRootTile;			// 0x4
	BSSimpleList<TemplateData*>	kMenuTemplates;		// 0x8
	UInt32						unk10;				// 0x10
	UInt32						unk14;				// 0x14
	UInt32						unk18;				// 0x18
	UInt8						byte1C;				// 0x1C
	UInt8						hasList08Data;		// 0x1D
	UInt8						byte1E;				// 0x1E
	UInt8						byte1F;				// 0x1F
	Type						eID;				// 0x20
	State						eVisibilityState;	// 0x24


	Menu*						HandleMenuInput(UInt32 tileID, Tile* clickedTile);
	__forceinline Tile*			AddTileFromTemplate(Tile* destTile, const char* templateName, UInt32 arg3 = 0)	{ return ThisCall<Tile*>(0xA1DDB0, this, destTile, templateName, arg3); }
	bool						GetTemplateExists(const std::string& templateName) const;

	__forceinline void			RegisterTile(Tile* apkTile, bool abNoAppendToMenuStack) { ThisCall(0xA1DC70, this, apkTile, abNoAppendToMenuStack); };
	__forceinline void			Open(bool noFadeIn = false) { ThisCall(0xA1DC20, this, noFadeIn); }; // fade in
	__forceinline void			Close() { pkRootTile->Set(Tile::kValue_fadeout, 1); ThisCall(0xA1D910, this); } // fade out

	static bool					IsVisible(const UInt32 menuType);
	bool						IsVisible() const;
	static TileMenu*			GetTileMenu(const UInt32 menuType);
	TileMenu*					GetTileMenu() const;
	static TileMenu*			GetTileMenu(const std::string& componentPath);
	static Menu*				GetMenu(const UInt32 menuType);
	static Menu*				GetMenu(const std::string& componentPath);
	static Menu*				TempMenuByType(const UInt32 menuType);

	__forceinline static void	RefreshItemsList() { StdCall(0x704AF0); }
	static bool					RefreshItemsListForm(TESForm* form = nullptr);
	static void					RefreshItemsListQuick();

	void  SetTileFade(Tile*	pkTile, bool startToFinish = true, Float32 duration = 0.25)
	{
		pkRootTile->SetGradual("_alpha", startToFinish ? 0 : 255.0, !startToFinish ? 0 : 255.0, duration);
	}
};
static_assert(sizeof(Menu) == 0x28);

enum MouseButtonNames
{
	kLeftClick = 0,
	kRightClick
};

enum MenuSoundCode
{
	kUIMenuOK = 0x1,
	kUIMenuCancel = 0x2,
	kUIMenuPrevNext = 0x3,
	kUIMenuFocus = 0x4,
	kUIPopUpQuestNew = 0x8,
	kUIPopUpMessageGeneral = 0xA,
	kUIPopUpMessageGeneral_ = 0x13,
	kUIMenuCancel_ = 0x14,
	kUILevelUp = 0x15,
	kUNIMPLEMENTED_ITMScrollOpen = 0x18,
	kUNIMPLEMENTED_ITMScrollClose = 0x19,
	kUNIMPLEMENTED_ITMBookOpen = 0x1A,
	kUNIMPLEMENTED_ITMBookClose = 0x1B,
	kUIMenuMode = 0x24,
};

struct BGSSaveLoadFileEntry
{
	char* name;
	UInt8 isInitialised;
	UInt8 isCorrupt;
	UInt8 gap06;
	UInt8 gap07;
	UInt32 saveNumber;
	char* pcName;
	char* pcTitle;
	char* location;
	char* time;
	UInt32 level;
	UInt32 screenshotWidth;
	UInt32 screenshotHeight;
	UInt32 unk28;
	UInt8 unk2C[8];
	UInt32 index;
	UInt32 versionInfo;
	char versionInfoString[64];
};

/*
static BSSimpleArray<StartMenu::UserOption*>* g_settingsMenuOptions = (BSSimpleArray<StartMenu::UserOption*>*)0x11DAB50;
void MenuButton_DownloadsClick();

Tile::Value* StringToTilePath(const std::string& componentPath);*/
