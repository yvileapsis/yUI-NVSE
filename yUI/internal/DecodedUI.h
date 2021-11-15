#pragma once
#include "GameData.h"
#include "GameProcess.h"
#include "GameUI.h"
#include "GameObjects.h"
#include "GameAPI.h"
#include "GameOSDepend.h"
#include "SafeWrite.h"
#include "GameScript.h"
#include "NiObjects.h"
#include "GameTypes.h"

#include "DecodedMusic.h"
#include "DecodedSaves.h"
#include "game_types.h"


enum InventoryMenuHotkey
{
	Inv_HotKeyWheel = 0x5,
	Inv_Equip = 0x6,
	Inv_Drop = 0x7,
	Inv_Repair = 0x8,
	Inv_HotkeyButton = 0x9,
	Inv_Cancel = 0xA,
	Inv_ModMenu = 0x13,
};

enum
{
	kAddr_AddExtraData = 0x40FF60,
	kAddr_RemoveExtraType = 0x410140,
	kAddr_LoadModel = 0x447080,
	kAddr_ApplyAmmoEffects = 0x59A030,
	kAddr_MoveToMarker = 0x5CCB20,
	kAddr_ApplyPerkModifiers = 0x5E58F0,
	kAddr_ReturnThis = 0x6815C0,
	kAddr_PurgeTerminalModel = 0x7FFE00,
	kAddr_EquipItem = 0x88C650,
	kAddr_UnequipItem = 0x88C790,
	kAddr_ReturnTrue = 0x8D0360,
	kAddr_TileGetFloat = 0xA011B0,
	kAddr_TileSetFloat = 0xA012D0,
	kAddr_TileSetString = 0xA01350,
	kAddr_InitFontInfo = 0xA12020,
	kAddr_GetItemHealthPerc = 0x4BCDB0,
	kAddr_GetItemValue = 0x4BD400,
	kAddr_ReadXML = 0xA01B00,
	kAddr_TileFromTemplate = 0xA1DDB0,
	kAddr_EvaluateConditions = 0x680C60,
	kAddr_EvaluateCondition = 0x681600,
};

template <typename Item> struct ListBoxItem
{
	Tile* tile;
	Item* object;
};

// 30
template <typename Item> class ListBox : public BSSimpleList<ListBoxItem<Item>>
{
public:
	Tile*			parentTile;			// 0C
	Tile*			selected;			// 10
	Tile*			scrollBar;			// 14
	const char*		templateName;		// 18
	UInt16			itemCount;			// 1C
	UInt16			pad1E;				// 1E
	float			unk20;				// 20
	float			storedListIndex;	// 24
	float			storedCurrentValue;	// 28
	UInt16			unk2C;				// 2C
	UInt16			pad2E;				// 2E

/*	Item* GetSelected()
	{
		ListNode<ListBoxItem<Item>>* iter = list.Head();
		ListBoxItem<Item>* item;
		do
		{
			item = iter->data;
			if (item && (item->tile == selected))
				return item->object;
		} while (iter = iter->next);
		return NULL;
	}

	Tile* GetNthTile(SInt32 index)
	{
		if (index >= 0)
		{
			ListNode<ListBoxItem<Item>>* iter = list.Head();
			do
			{
				if (!index)
				{
					return iter->data ? iter->data->tile : NULL;
				}
				index--;
			} while (iter = iter->next);
		}
		return NULL;
	}*/

	void Clear()
	{
		ListNode<ListBoxItem<Item>>* iter = this->list.Head();
		ListBoxItem<Item>* item;
		do
		{
			item = iter->data;
			if (!item) continue;
			if (item->tile)
				item->tile->Destroy(true);
			GameHeapFree(item);
		} while (iter = iter->next);
		this->list.RemoveAll();
		selected = NULL;
		itemCount = 0;
	}

	typedef bool(__cdecl* FilterFunction)(Item* form);
	void Filter(FilterFunction callback)
	{
		ThisCall(0x729FE0, this, callback);
	}
	
	// Identical to Filter, but passing a value instead of a pointer
	void FilterVal(bool(__cdecl* callback)(Item))
	{
		ThisCall(0x730BB0, this, callback);
	}

	typedef void(__cdecl* ForEachFunc)(Tile*, Item*);
	void ForEach(ForEachFunc func, int maxIndex1 = -1, int maxIndex2 = 0x7FFFFFFF)
	{
		ThisCall(0x7314C0, this, func, maxIndex1, maxIndex2);
	}

	Tile* GetTileFromItem(Item** item)
	{
		return ThisCall<Tile*>(0x7A22D0, this, item);
	}

	void SaveScrollPosition()
	{
		ThisCall(0x7312E0, this);
	}

	int GetNumVisibleItems()
	{
		return ThisCall<int>(0x71AE60, this);
	}

	void RestorePosition(bool playSound)
	{
		ThisCall(0x731360, this, playSound);
	}

	Tile* Insert(Item* item, const char* text, signed int (*sortingFunction)(ListBoxItem<Item>* a1, ListBoxItem<Item>* a2) = nullptr, const char* templateName = nullptr)
	{
		return ThisCall<Tile*>(0x71A320, this, item, text, sortingFunction, templateName);
	}

	void HighlightLastItem()
	{
		int lastIndex = this->itemCount - 1;
		Tile* tile = this->GetNthTile(lastIndex);

		this->SetSelectedTile(tile);
		this->ScrollToHighlight();
	}

	void SetParentEnabled(bool isEnabled)
	{
		static UInt32 enabledTrait = CdeclCall<UInt32>(0xA00940, "_enabled", -1);
		parentTile->SetFloat(enabledTrait, isEnabled);
	}
};
typedef ListBox<ContChangesEntry> MenuItemEntryList;

// 10C
class ContainerMenu : public Menu		// 1008
{
public:
	ContainerMenu();
	~ContainerMenu();

	enum Mode
	{
		kNormal = 0x1,
		kPickpocket,
		kTeammate,
		kRockItLauncher
	};

	TileImage* tile028;		// 028
	TileText* tile02C;		// 02C
	TileImage* tile030;		// 030
	TileText* tileInventoryWeight;		// 034
	TileImage* tile038;		// 038
	TileImage* tile03C;		// 03C
	TileText* tile040;		// 040
	TileImage* tile044;		// 044
	TileImage* tile048;		// 048
	TileImage* tile04C;		// 04C
	TileImage* takeAllTile;	// 050
	TileImage* tile054;		// 054
	TileImage* tile058;		// 058
	TileRect* tile05C;		// 05C
	TileRect* tile060;		// 060
	TileRect* tile064;		// 064
	TileRect* tile068;		// 068
	TileRect* tile06C;		// 06C
	TileRect* tile070;		// 070
	TESObjectREFR* containerRef;	// 074
	tList<void>			list078;		// 078
	UInt32				mode;			// 080
	UInt32				valueTransfered;// 084
	UInt8				hasPickedPocket;// 088
	UInt8				hasFailedPickpocket;	// 089
	UInt8				pad08A;			// 08A
	UInt8				pad08B;			// 08B
	UInt32				leftFilter;		// 08C
	UInt32				rightFilter;	// 090
	UInt32				menuSoundID;	// 094
	MenuItemEntryList	leftItems;		// 098
	MenuItemEntryList	rightItems;		// 0C8
	MenuItemEntryList* currentItems;	// 0F8
	UInt32				unk0FC;			// 0FC
	Sound				menuSound;		// 100

	static ContainerMenu* GetSingleton() { return *(ContainerMenu**)(0x11D93F8); }
	static ContChangesEntry* GetSelection() { return *(ContChangesEntry**)(0x11D93FC); }
	static void SetSelection(ContChangesEntry* entry) { *(ContChangesEntry**)(0x11D93FC) = entry; }
};

class BookMenu : public Menu
{
public:
	BookMenu();
	~BookMenu();
	TileImage* tile28;
	TileImage* tile2C;
	Actor* activator;
	TESObjectBOOK* bookObject;
	UInt32 unk38;
};

struct HotKeyWheel
{
	TileRect* parent;
	TileRect* hotkeys[8];
	UInt8 byte24;
	UInt8 gap25[3];
	UInt32 unk28;
	UInt32 selectedHotkeyTrait;
	UInt32 selectedTextTrait;
};

// 278
class HUDMainMenu : public Menu			// 1004
{
public:
	HUDMainMenu();
	~HUDMainMenu();

	struct QueuedMessage
	{
		char	msgText[0x204];			// 000
		char	iconPate[MAX_PATH];		// 204
		char	soundPath[MAX_PATH];	// 308
		float	displayTime;			// 40C
	};

	struct QueuedQuestText
	{
		char msg[0x104];
		bool byte104;
		bool byte105;
	};

	struct SubtitleData
	{
		char text[MAX_PATH];
		UInt32 startTime;
		float displayTime;
		NiPoint3 pos;
		UInt32 priority;
	};

	enum VisibilityFlags
	{
		kActionPoints = 0x1,
		kHitPoints = 0x2,
		kRadiationMeter = 0x4,
		kEnemyHealth = 0x8,
		kQuestReminder = 0x10,
		kRegionLocation = 0x20,
		kReticleCenter = 0x40,
		kSneakMeter = 0x80,
		kMessages = 0x100,
		kInfo = 0x200,
		kSubtitles = 0x400,
		kHotkeys = 0x800,
		kXpMeter = 0x1000,
		kBreathMeter = 0x2000,
		kExplosivePositioning = 0x4000,
		kCrippledLimbIndicator = 0x8000,
		kHardcoreMode = 0x10000,
	};

	enum HUDStates
	{
		kRECALCULATE = 0x1,
		kNormal,
		kPipBoy,
		kPause,
		kLoading,
		kDialog,
		kVATSMenu,
		kVATSPlayback,
		kContainer,
		kBeginSit,
		kSleepWait,
		kPlayerDisabledControls,
		kUnusedD,
		kLevelUpMenu,
		kHacking,
		kComputers,
		kMessage,
		kSpecialBook,
		kLoveTester,
		kVanityCam,
		kUnused15,
		kAiming,
		kAimingScope,
		kIntroMovie,
		kGambling,
	};

	UInt32							unk028;			// 028
	union
	{
		Tile* tiles[0x56];
		struct
		{
			TileImage* tile02C;		// 02C	HitPoints\meter
			TileText* tile030;		// 030	HitPoints\justify_right_text
			TileRect* tile034;		// 034	HitPoints\compass_window\compass_icon_group
			TileRect* tile038;		// 038	HitPoints\compass_window\compass_icon_group
			TileRect* tile03C;		// 03C	HitPoints\compass_window\compass_icon_group
			TileImage* tile040;		// 040	HitPoints\compass_window
			TileImage* tile044;		// 044	ActionPoints\meter
			TileText* tile048;		// 048	ActionPoints\justify_right_text
			TileText* tile04C;		// 04C	ActionPoints\justify_right_text
			TileImage* tile050;		// 050	ActionPoints\meter
			TileImage* tile054;		// 054	ActionPoints\MeterBackground
			TileText* tile058;		// 058	ActionPoints\justify_right_text
			TileRect* tile05C;		// 05C	QuestReminder\QuestStages
			TileRect* tile060;		// 060	QuestReminder\QuestAdded
			TileText* tile064;		// 064	Region_Location\justify_left_text
			TileText* tile068;		// 068	Region_Location\justify_left_text
			TileImage* tile06C;		// 06C	RadiationMeter\radiation_bracket
			TileImage* tile070;		// 070	RadiationMeter\radiation_pointer
			TileText* tile074;		// 074	RadiationMeter\radiation_text_value
			TileText* tile078;		// 078	RadiationMeter\radiation_text
			TileImage* tile07C;		// 07C	EnemyHealth\enemy_health_bracket
			TileImage* tile080;		// 080	EnemyHealth\meter
			TileText* tile084;		// 084	EnemyHealth\justify_center_text
			TileText* sneakLabel;	// 088	SneakMeter\sneak_nif
			TileImage* tile08C;		// 08C	Messages\message_icon
			TileText* tile090;		// 090	Messages\justify_left_text
			TileImage* tile094;		// 094	Messages\message_bracket
			TileText* tile098;		// 098	Subtitles\justify_center_text
			TileRect* tileTakePrompt;		// 09C	Info\justify_center_hotrect
			TileText* tile0A0;		// 0A0	Info\justify_center_hotrect\PCShortcutLabel
			TileImage* tile0A4;		// 0A4	Info\justify_center_hotrect\xbox_button
			TileText* tile0A8;		// 0A8	Info\justify_center_text
			TileText* tileLockLevelStr;	// 0AC	Info\justify_center_text
			TileText* tile0B0;		// 0B0	Info\justify_center_text
			TileText* tilePickupWeightStr;	// 0B4	Info\justify_right_text
			TileText* tilePickupWG;			// 0B8	Info\justify_left_text
			TileText* tilePickupValueStr;	// 0BC	Info\justify_right_text
			TileText* tilePickupVAL;		// 0C0	Info\justify_left_text
			TileImage* tile0C4;		// 0C4	Info\info_seperator
			TileRect* tile0C8;		// 0C8	Hokeys\hotkey_selector
			TileText* tile0CC;		// 0CC	Hokeys\justify_center_text
			TileImage* tile0D0;		// 0D0	HitPoints\left_bracket
			TileImage* tile0D4;		// 0D4	ActionPoints\right_bracket
			TileImage* tile0D8;		// 0D8	XPMeter\XPBracket
			TileText* tile0DC;		// 0DC	XPMeter\XPAmount
			TileText* tile0E0;		// 0E0	XPMeter\XPLabel
			TileImage* tile0E4;		// 0E4	XPMeter\XPPointer
			TileText* tile0E8;		// 0E8	XPMeter\XPLastLevel
			TileText* tile0EC;		// 0EC	XPMeter\XPNextLevel
			TileText* tile0F0;		// 0F0	XPMeter\XPLevelUp
			TileImage* tile0F4;		// 0F4	ReticleCenter\reticle_center
			TileImage* tile0F8;		// 0F8	crippled_limb_indicator\Face
			TileImage* tile0FC;		// 0FC	crippled_limb_indicator\Head
			TileImage* tile100;		// 100	crippled_limb_indicator\Torso
			TileImage* tile104;		// 104	crippled_limb_indicator\Left_Arm
			TileImage* tile108;		// 108	crippled_limb_indicator\Right_Arm
			TileImage* tile10C;		// 10C	crippled_limb_indicator\Left_Leg
			TileImage* tile110;		// 110	crippled_limb_indicator\Right_Leg
			TileRect* tile114;		// 114	ActionPoints
			TileRect* tile118;		// 118	HitPoints
			TileRect* tile11C;		// 11C	RadiationMeter
			TileRect* tile120;		// 120	EnemyHealth
			TileRect* tile124;		// 124	QuestReminder
			TileRect* tile128;		// 128	Region_Location
			TileRect* tile12C;		// 12C	ReticleCenter
			TileRect* tile130;		// 130	SneakMeter
			TileRect* tile134;		// 134	Messages
			TileRect* tile138;		// 138	Info
			TileRect* tile13C;		// 13C	Subtitles
			TileRect* tile140;		// 140	Hokeys
			TileRect* tile144;		// 144	XPMeter
			int* tile148;			// 148	BreathMeter
			TileRect* tile14C;		// 14C	Explosive_positioning_rect
			TileRect* crippledLimbIndicator;		// 150	crippled_limb_indicator
			TileImage* tile154;		// 154	DDTIcon
			TileImage* tile158;		// 158	DDTIconEnemy
			TileText* tile15C;		// 15C	AmmoTypeLabel
			TileRect* tile160;		// 160	HardcoreMode
			TileText* tile164;		// 164	HardcoreMode\Dehydration
			TileText* tile168;		// 168	HardcoreMode\Sleep
			TileText* tile16C;		// 16C	HardcoreMode\Hunger
			TileImage* tile170;		// 170	DDTIcon
			TileImage* tile174;		// 174	DDTIconEnemyAP
			TileText* tile178;		// 178	HardcoreMode\Rads
			TileText* tile17C;		// 17C	HardcoreMode\LMBs
			TileImage* tile180;		// 180	CNDArrows
		};
	};

	UInt32						unk184;				// 184
	float						flt188;				// 188
	tList<QueuedMessage>		queuedMessages;		// 18C
	UInt32						currMsgKey;			// 194
	BSSimpleArray<SubtitleData>	subtitlesArr;		// 198
	SubtitleData* currentSubtitle;	// 1A8 
	Sound						radiationSound;		// 1AC
	TESObjectREFR* crosshairRef;		// 1B8
	UInt32						unk1BC;				// 1BC
	Bitfield32					visibilityOverrides;// 1C0
	UInt32						stage;				// 1C4
	HotKeyWheel					hotkeyWheel;		// 1C8	Hokeys\hotkey_selector
	UInt8					isUsingScope;			// 1FC
	UInt8					byte1FD;				// 1FD
	UInt8					byte1FE;				// 1FE
	UInt8					byte1FF;				// 1FF
	NiControllerSequence* niContSeq;				// 200
	UInt8					isLevelUpQueued;		// 204
	UInt8					autoDisplayObjectives;	// 205
	UInt8					pad206[2];				// 206
	UInt32					unk208;					// 208
	UInt32					questTargets;			// 20C
	UInt32					unk210;					// 210
	UInt32					compassWidth;			// 214
	UInt32					maxCompassAngle;		// 218
	Actor* healthTarget;			// 21C
	UInt32					unk220;					// 220
	Timer					timer224;				// 224
	UInt32					unk244;					// 244
	UInt32					unk248[4];				// 248
	tList<QueuedQuestText>	queuedQuestTextList;	// 258
	UInt8					byte260;				// 260
	UInt8					byte261;				// 261
	UInt8					pad262[2];				// 262
	tList<UInt32>			xpMessages;				// 264
	tList<UInt32>			list26C;				// 26C
	float					hudShake;				// 274

	static HUDMainMenu* GetSingleton() { return *(HUDMainMenu**)(0x11D96C0); }
	static void __cdecl SetQuestUpdateText(char* src, bool a2, bool a3) { CdeclCall(0x77A5B0, src, a2, a3); }
	static float GetOpacity() { return *(float*)0x11D979C; };
};
STATIC_ASSERT(sizeof(HUDMainMenu) == 0x278);

class Tile3D;

class LoadingMenu : public Menu
{
public:
	LoadingMenu();
	~LoadingMenu();

	struct TileDatas
	{
		UInt32 x;
		UInt32 y;
		UInt32 height;
		UInt32 width;
		UInt32 font;
		UInt8 justify;
	};

	Tile* tile028[38];
	/*
	Tile3D* loading_nif;
	TileText* loading_plateno_text2;
	TileText* loading_info_text;
	TileText* loading_currobj_text;
	TileText* loading_stat1_label;
	TileText* loading_stat1_val;
	TileText* loading_stat2_label;
	TileText* loading_stat2_val;
	TileText* loading_stat3_label;
	TileText* loading_stat3_val;
	TileImage* loading_levelprog_meter;
	Tile3D* loading_pinwheel;
	TileImage* loading_world_image;
	TileText* loading_plateno_text1;
	TileText* loading_currobj_label;
	TileText* loading_levelprog_label;
	TileRect* loading_levelprog_rect;
	TileText* loading_stat_label_rect;
	TileRect* loading_stat_val_rect;
	TileText* loading_stat4_label;
	TileText* loading_stat4_val;
	TileText* loading_stat5_label;
	TileText* loading_stat5_val;
	TileText* loading_stat6_label;
	TileText* loading_stat6_val;
	TileText* loading_stat7_label;
	TileText* loading_stat7_val;
	TileRect* loading_currobj_rect;
	TileText* loading_levelprog_curlevel;
	TileText* loading_levelprog_nextlevel;
	TileText* loading_levelprog_char_marker;
	TileRect* loading_stat_rect;
	TileImage* loading_tile_slide_01;
	TileImage* loading_tile_slide_02;
	TileText* hidden_tip_text_01;
	TileText* tip_text_02;
	TileImage* tip_matte_01;
	TileImage* tip_matte_02;
	*/
	UInt32 minX;
	UInt32 minY;
	UInt32 maxX;
	UInt32 maxY;
	DList<Tile> statTiles;
	DList<Tile> currObjTiles;
	DList<Tile> levelProgTiles;
	DList<Tile> hiddenTipTiles;
	DList<Tile> tipTiles;
	UInt32 miscStatArray[43];
	String str1B8;
	NiPoint4 backgroundColor;
	tList<TESLoadScreen> loadScrList;
	void* ptr1D8;
	TESLoadScreen* loadScr1DC;
	UInt32 nextUpdateTime;
	UInt32 time1E4;
	TESWorldSpace* worldspace;
	UInt32 unk1E8;
	UInt32 unk1EC;
	Sound sound1F0;
	Sound sound1FC;
	void* srcTexture[4];
	UInt32 unk21C;
	UInt16 wrd220;
	UInt16 flags;
	LoadingMenu::TileDatas tileDatas[38];
	UInt32 unk5B4;
	UInt32 unk5B8;
	UInt32 unk5BC;

	static LoadingMenu* GetSingleton() { return *(LoadingMenu**)0x11DA0C0; };
};
STATIC_ASSERT(sizeof(LoadingMenu) == 0x5C0);

class CharGenMenu : public Menu
{
	CharGenMenu();
	~CharGenMenu();
public:
	UInt32 isTagSkills;
	Tile* tile02C;
	Tile* tile030;
	Tile* tileActorValueIcon;
	Tile* tileActorValueDescription;
	Tile* tileNumSkills;
	Tile* tileReset;
	Tile* tileDone;
	UInt32 currPoints;
	UInt32 numPoints;
	float listIndex;
	UInt8 isModValues;
	UInt8 showInitialTaggedSkills;
	UInt8 gap056[2];
	ListBox<UInt32> actorValues;

	static CharGenMenu* GetSingleton() { return *(CharGenMenu**)0x11D920C; };
};
STATIC_ASSERT(sizeof(CharGenMenu) == 0x88);

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

enum MiscStatCode
{
	kMiscStat_QuestsCompleted,
	kMiscStat_LocationsDiscovered,
	kMiscStat_PeopleKilled,
	kMiscStat_CreaturesKilled,
	kMiscStat_LocksPicked,
	kMiscStat_ComputersHacked,
	kMiscStat_StimpaksTaken,
	kMiscStat_RadXTaken,
	kMiscStat_RadAwayTaken,
	kMiscStat_ChemsTaken,
	kMiscStat_TimesAddicted,
	kMiscStat_MinesDisarmed,
	kMiscStat_SpeechSuccesses,
	kMiscStat_PocketsPicked,
	kMiscStat_PantsExploded,
	kMiscStat_BooksRead,
	kMiscStat_HealthFromStimpaks,
	kMiscStat_WeaponsCreated,
	kMiscStat_HealthFromFood,
	kMiscStat_WaterConsumed,
	kMiscStat_SandmanKills,
	kMiscStat_ParalyzingPunches,
	kMiscStat_RobotsDisabled,
	kMiscStat_TimesSlept,
	kMiscStat_CorpsesEaten,
	kMiscStat_MysteriousStrangerVisits,
	kMiscStat_DoctorBagsUsed,
	kMiscStat_ChallengesCompleted,
	kMiscStat_MissFortunateOccurrences,
	kMiscStat_Disintegrations,
	kMiscStat_HaveLimbsCrippled,
	kMiscStat_SpeechFailures,
	kMiscStat_ItemsCrafted,
	kMiscStat_WeaponModifications,
	kMiscStat_ItemsRepaired,
	kMiscStat_TotalThingsKilled,
	kMiscStat_DismemberedLimbs,
	kMiscStat_CaravanGamesWon,
	kMiscStat_CaravanGamesLost,
	kMiscStat_BarterAmountTraded,
	kMiscStat_RouletteGamesPlayed,
	kMiscStat_BlackjackGamesPlayed,
	kMiscStat_SlotsGamesPlayed,
	kMiscStat_Max
};

// 2A4
class StatsMenu : public Menu			// 1003
{
public:
	StatsMenu();
	~StatsMenu();

	enum Tabs
	{
		kStatusEffects = 0,
		kSPECIAL = 1,
		kSkills = 2,
		kPerks = 3,
		kGeneral = 4,
	};

	struct AlchItemData
	{
		AlchemyItem* alchItem;
		TileImage* tileImg;
		void* dataPtr;
		bool			(*callback)(void* arg);
	};

	class StatusDataList
	{
	public:
		struct StatusData
		{
			UInt32 magnitude;
			UInt32 unk04;
			UInt32 duration;
			UInt32 unk0C;
			UInt32 avCode;
		};

		tList<StatusData> statusDataList;
	};

	class NamedEffectsList
	{
	public:
		tList<void> list04;
	};

	struct StatusEffect
	{
		SpellItem* spellItem;
		StatusDataList statusDataList;
		NamedEffectsList namedEffectsList;
	};

	AlchItemData					alchItemData[4];	// 028	0: Stimpak, 1: Rad-X, 2: RadAway, 3: Doctor's Bag
	BSSimpleList<EffectSetting>		effectList;			// 068
	StatusEffect* radEffects;
	StatusEffect* h20effects;
	StatusEffect* hungerEffects;
	StatusEffect* sleepEffects;
	BSSimpleList<StatusEffect>		statusEffList;		// 084
	TileImage* tile090;			// 090
	TileImage* tile094;			// 094
	TileImage* tile098;			// 098
	TileImage* tile09C;			// 09C
	TileImage* tile0A0;			// 0A0
	TileImage* tile0A4;			// 0A4
	TileImage* tile0A8;			// 0A8
	TileImage* tile0AC;			// 0AC
	TileImage* tile0B0;			// 0B0
	TileImage* tile0B4;			// 0B4
	TileImage* tile0B8;			// 0B8
	TileImage* tile0BC;			// 0BC
	TileImage* tile0C0;			// 0C0
	TileRect* tile0C4;			// 0C4
	TileImage* tile0C8;			// 0C8
	TileRect* tile0CC;			// 0CC
	TileImage* tile0D0;			// 0D0
	TileRect* tile0D4;			// 0D4
	TileImage* tile0D8;			// 0D8
	TileRect* tile0DC;			// 0DC
	TileImage* tile0E0;			// 0E0
	TileRect* tile0E4;			// 0E4
	TileImage* tile0E8;			// 0E8
	TileRect* tile0EC;			// 0EC
	TileImage* tile0F0;			// 0F0
	TileImage* tile0F4;			// 0F4
	TileImage* tile0F8;			// 0F8
	TileText* tile0FC;			// 0FC
	TileImage* tile100;			// 100
	TileImage* tile104;			// 104
	TileImage* tile108;			// 108
	TileImage* tile10C;			// 10C
	TileImage* tile110;			// 110
	TileImage* tile114;			// 114
	TileImage* tile118;			// 118
	TileText* tile11C;			// 11C
	TileImage* tile120;			// 120
	TileText* tile124;			// 124
	TileText* tile128;			// 128
	TileText* tile12C;			// 12C
	TileRect* tile130;			// 130
	TileRect* tile134;			// 134
	TileRect* tile138;			// 138
	TileRect* tile13C;			// 13C
	TileText* tile140;			// 140
	TileText* tile144;			// 144
	TileImage* tile148;			// 148
	TileImage* tile14C;			// 14C
	TileText* tile150;			// 150
	TileImage* tile154;			// 154
	TileImage* tile158;			// 158
	TileText* tile15C;			// 15C
	TileImage* tile160;			// 160
	TileImage* tile164;			// 164
	TileImage* tile168;			// 168
	TileImage* tile16C;			// 16C
	TileImage* tile170;			// 170
	TileImage* tile174;			// 174
	TileImage* tile178;			// 178
	TileImage* tile17C;			// 17C
	ListBox<ActorValueCode>			SPECIALList;		// 180
	ListBox<ActorValueCode>			skillList;			// 1B0
	ListBox<PerkRank>				perkRankList;		// 1E0
	ListBox<MiscStatCode>			miscStatIDList;		// 210
	ListBox<StatusEffect>			statusEffListBox;	// 240
	ListBox<TESReputation>			reputationList;		// 270
	UInt8 isInHealLimbSelectMode; // 2A0
	UInt8 byte2A1[3];			  // 2A1
	static StatsMenu* GetSingleton() { return *(StatsMenu**)0x11DACE0; };
};

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

	Tile* tiles[16];
	Actor* companionRef;
	UInt8 isIsFollowingLong;
	UInt8 isFollowerSwitchAggressive;
	UInt8 isCombatStyleRanged;
	UInt8 isWaiting;
	UInt32 lastSelectedTileID;
	UInt8 talkToActorWhenClosingMenu;
	UInt8 gap75[3];
	UInt32 time78;
	Sound sound7C;
	static CompanionWheelMenu* GetSingleton() { return *(CompanionWheelMenu**)0x11D92B8; };
};
STATIC_ASSERT(sizeof(CompanionWheelMenu) == 0x88);

struct VATSTargetInfo
{
	enum ActionTypes
	{
		kAPCost_AttackUnarmed = 0x0,
		kAPCost_AttackOneHandMelee = 0x1,
		kAPCost_AttackTwoHandMelee = 0x2,
		kAPCost_AttackPistol = 0x3,
		kAPCost_AttackRifle = 0x4,
		kAPCost_AttackHandle = 0x5,
		kAPCost_AttackLauncher = 0x6,
		kAPCost_AttackGrenade = 0x7,
		kAPCost_AttackMine = 0x8,
		kAPCost_Reload = 0x9,
		kAPCost_Crouch = 0xA,
		kAPCost_Stand = 0xB,
		kAPCost_SwitchWeapon = 0xC,
		kAPCost_ToggleWeaponDrawn = 0xD,
		kAPCost_Heal = 0xE,
		kAPCost_OneHandThrown = 0x13,
		kAPCost_AttackThrown = 0x14,
		kAPCost_UnarmedAttackGround = 0x15,
		kAPCost_MAX = 0x16,
	};

	int actionType;
	UInt8 isSuccess;
	UInt8 byte05;
	UInt8 isMysteriousStrangerVisit;
	UInt8 byte07;
	UInt8 remainingShotsToFire;
	UInt8 count09;
	UInt8 gap0A[2];
	TESObjectREFR* ref;
	UInt32 avCode;
	ActorHitData* hitData;
	float unk18;
	float unk1C;
	float apCost;
	UInt8 isMissFortuneVisit;
	UInt8 gap25[3];
};
STATIC_ASSERT(sizeof(VATSTargetInfo) == 0x28);

struct VATSTargetBodyPartData
{
	NiPoint2 screenPos;
	NiPoint3 relativePos;
	NiPoint3 pos;
	UInt32 bodyPartID;
	float percentVisible;
	float hitChance;
	UInt8 isOnScreen;
	UInt8 byte2D;
	UInt8 shouldCalculateBodyPartVisibilityPercents;
	UInt8 byte2F;
	Tile* tile_body_part_percent;
	float unk34;
	UInt8 byte38;
	UInt8 gap38[3];
};

class VATSMenu : public Menu
{
	VATSMenu();
	~VATSMenu();
public:
	enum Buttons
	{
		kVATS_queuedActions = 0x1,
		kVATS_leftArrow = 0x2,
		kVATS_rightArrow = 0x3,
		kVATS_acceptButton = 0x4,
		kVATS_returnButton = 0x5,
		kVATS_bodyPartButton = 0x6,
		kVATS_selectButton = 0x7,
		kVATS_specialAttackButton = 0x1E,
		kVATS_specialAttack2Button = 0x1F,
	};

	UInt32 unk028;


	TileImage* tile02C[31];
	/*
	TileImage* tile030;
	TileImage* tile034;
	TileImage* tile038;
	TileImage* tile03C;
	TileImage* tile040;
	TileImage* tile044;
	TileImage* tile048;
	TileImage* tile04C;
	TileImage* tile050;
	TileText* tile054;
	TileText* tile058;
	TileText* tile05C;
	TileText* tile060;
	TileImage* tile064;
	TileImage* tile068;
	TileImage* tile06C;
	TileImage* tile070;
	TileText* tile074;
	TileRect* tile078;
	TileRect* tile07C;
	TileRect* tile080;
	TileImage* tile084;
	TileRect* tile088;
	TileImage* tile08C;
	TileImage* tile090;
	TileImage* tile094;
	TileImage* tile098;
	TileText* tile09C;
	TileImage* tile0A0;
	TileImage* tile0A4;
	*/
	UInt32 unk0A8[2];
	ListBox<UInt32> queuedActions;
	float actionPoints;
	float maxActionPoints;
	UInt32 unk0E8;
	UInt32 unk0EC;
	float clipAmmo;
	float reserveAmmo;
	UInt8 isFullyZoomedIn;
	UInt8 unk0F9;
	UInt8 unk0FA;
	UInt8 unk0FB;
	UInt32 unk0FC;
	VATSTargetBodyPartData* currentBodyPartData;
	UInt8 hasConcentratedFire;
	UInt8 gap105[3];
	UInt32 unk108;
	UInt8 byte10C;
	UInt8 byte10D;
	UInt8 byte10E;
	UInt8 byte10F;
	UInt8 byte110;
	UInt8 pad111;
	UInt8 pad112;
	UInt8 pad113;
	UInt32 time114;
	UInt8 byte118;
	UInt8 pad119[3];
	VATSTargetInfo targetInfo;

	static VATSMenu* GetSingleton() { return *(VATSMenu**)0x11DB0D4; };
	static TESObjectREFR* GetTarget() { return *(TESObjectREFR**)0x11F21CC; };
};
STATIC_ASSERT(sizeof(VATSMenu) == 0x144);

enum Scancodes
{
	_Escape = 0x1,
	One = 0x2,
	Two = 0x3,
	Three = 0x4,
	Four = 0x5,
	Five = 0x6,
	Six = 0x7,
	Seven = 0x8,
	Eight = 0x9,
	Nine = 0xA,
	Zero = 0xB,
	Minus = 0xC,
	Equals = 0xD,
	Backspace = 0xE,
	Tab = 0xF,
	Q = 0x10,
	W = 0x11,
	E = 0x12,
	R = 0x13,
	T = 0x14,
	Y = 0x15,
	U = 0x16,
	I = 0x17,
	O = 0x18,
	P = 0x19,
	LeftBracket = 0x1A,
	RightBracket = 0x1B,
	Enter = 0x1C,
	LeftControl = 0x1D,
	A = 0x1E,
	S = 0x1F,
	D = 0x20,
	F = 0x21,
	G = 0x22,
	H = 0x23,
	J = 0x24,
	K = 0x25,
	L = 0x26,
	SemiColon = 0x27,
	Apostrophe = 0x28,
	Tilde = 0x29,
	LeftShift = 0x2A,
	BackSlash = 0x2B,
	Z = 0x2C,
	X = 0x2D,
	C = 0x2E,
	V = 0x2F,
	B = 0x30,
	N = 0x31,
	M = 0x32,
	Comma = 0x33,
	Period = 0x34,
	ForwardSlash = 0x35,
	RightShift = 0x36,
	NumStar = 0x37,
	LeftAlt = 0x38,
	SpaceBar = 0x39,
	CapsLock = 0x3A,
	F1 = 0x3B,
	F2 = 0x3C,
	F3 = 0x3D,
	F4 = 0x3E,
	F5 = 0x3F,
	F6 = 0x40,
	F7 = 0x41,
	F8 = 0x42,
	F9 = 0x43,
	F10 = 0x44,
	NumLock = 0x45,
	ScrollLock = 0x46,
	Num7 = 0x47,
	Num8 = 0x48,
	Num9 = 0x49,
	NumMinus = 0x4A,
	Num4 = 0x4B,
	Num5 = 0x4C,
	Num6 = 0x4D,
	NumPlus = 0x4E,
	Num1 = 0x4F,
	Num2 = 0x50,
	Num3 = 0x51,
	Num0 = 0x52,
	NumPeriod = 0x53,
	F11 = 0x57,
	F12 = 0x58,
	NumEnter = 0x9C,
	RightControl = 0x9D,
	NumSlash = 0xB5,
	PrintScrn = 0xB7,
	RightAlt = 0xB8,
	Home = 0xC7,
	UpArrow = 0xC8,
	PageUp = 0xC9,
	LeftArrow = 0xCB,
	RightArrow = 0xCD,
	End_ = 0xCF,
	DownArrow = 0xD0,
	PageDown = 0xD1,
	Delete_ = 0xD3,
	LeftMouse = 0x100,
	RightMouse = 0x101,
	MiddleMouse = 0x102,
	MouseButton3 = 0x103,
	MouseButton4 = 0x104,
	MouseButton5 = 0x105,
	MouseButton6 = 0x106,
	MouseButton7 = 0x107,
	MouseWheelUp = 0x108,
	MouseWheelDown = 0x109,
};

class TileRect;

class InventoryMenu : public Menu		// 1002
{
public:
	InventoryMenu();
	~InventoryMenu();

	enum
	{
		kWeapons,
		kApparel,
		kAid,
		kMisc,
		kAmmo
	};

	struct ScrollPos
	{
		SInt32 listIndex;
		SInt32 currentValue;
	};

	TileRect* tile028;		// 028	IM_Headline_PlayerCapsInfo
	TileRect* tile02C;		// 02C	IM_Headline_PlayerHPInfo
	TileRect* tile030;		// 030	IM_Headline_PlayerDRInfo
	TileRect* tile034;		// 034	IM_Headline_PlayerWGInfo
	TileImage* tile038;		// 038	IM_InventoryList
	TileRect* tile03C;		// 03C	IM_HotKeyWheel
	TileImage* tile040;		// 040	IM_EquipButton
	TileImage* tile044;		// 044	IM_DropButton
	TileImage* tile048;		// 048	IM_RepairButton
	TileImage* tile04C;		// 04C	IM_HotkeyButton
	TileImage* tile050;		// 050	IM_CancelButton
	TileImage* tile054;		// 054	IM_ItemIcon
	TileRect* tile058;		// 058	IM_ItemInfoRect
	TileRect* tile05C;		// 05C	IM_Tabline
	TileRect* tile060;		// 060	DAMInfo
	TileRect* tile064;		// 064	DPSInfo
	TileRect* tile068;		// 068	StrengthReqInfo
	TileRect* tile06C;		// 06C	DamageResistInfo
	TileRect* tile070;		// 070	DamageThresholdInfo
	TileImage* tile074;		// 074	IM_ModButton
	TileImage* tile078;		// 078	IM_ItemIconBadge
	TileRect* tile07C;		// 07C	IM_Headline_PlayerDTInfo
	TileText* tile080;		// 080	IM_StrReq
	UInt32				filter;			// 084
	ScrollPos	tabScrollPositions[6];	// 088, the scroll index for Weapons, Apparel, Aid, Misc, Ammo and the Keyring
	MenuItemEntryList	itemsList;		// 0B8
	HotKeyWheel			hotkeyWheel;	// 0E8
	tList<ContChangesEntry> changedItemsList; // 11C

	static InventoryMenu* GetSingleton() { return *(InventoryMenu**)0x11D9EA4; };
	static ContChangesEntry* GetSelection() { return *(ContChangesEntry**)0x11D9EA8; };
	bool IsKeyringOpen() const;
};
STATIC_ASSERT(sizeof(InventoryMenu) == 0x124);


typedef tList<ContChangesEntry> BarterItemList;

class BarterMenu : public Menu			// 1053
{
public:
	BarterMenu();
	~BarterMenu();

	TileImage* tile028;		// 028
	TileImage* tile02C;		// 02C
	TileImage* tile030;		// 030
	TileText* tile034;		// 034
	TileImage* tile038;		// 038
	TileText* tile03C;		// 03C
	TileImage* tile040;		// 040
	TileImage* tile044;		// 044
	TileText* tile048;		// 048
	TileImage* tile04C;		// 04C
	TileText* tile050;		// 050
	TileImage* tile054;		// 054
	TileRect* tile058;		// 058
	TileImage* tile05C;		// 05C
	TileRect* tile060;		// 060
	TileRect* tile064;		// 064
	TileRect* tile068;		// 068
	TileRect* tile06C;		// 06C
	TileRect* tile070;		// 070
	TileImage* tile074;		// 074
	TileImage* tile078;		// 078
	TileRect* tile07C;		// 07C
	TESObjectREFR* merchantRef;	// 080
	float				barterTotalSum;	// 084
	UInt32				unk088;			// 088
	UInt32				playerGold;		// 08C
	UInt32				merchantGold;	// 090
	float				buyValueMult;	// 094
	float				sellValueMult;	// 098
	UInt32				leftFilter;		// 09C
	UInt32				rightFilter;	// 0A0
	UInt32				unk0A4;			// 0A4
	MenuItemEntryList	leftItems;		// 0A8
	MenuItemEntryList	rightItems;		// 0D8
	MenuItemEntryList* currentItems;	// 108
	BarterItemList		leftBarter;		// 10C
	BarterItemList		rightBarter;	// 114
	UInt32				unk11C;			// 11C

	static BarterMenu* GetSingleton() { return *(BarterMenu**)0x11D8FA4; }
};

// 44
class QuantityMenu : public Menu		// 1016
{
public:

	TileRect* tile28;		// 28
	TileImage* tile2C;		// 2C	QM_DecreaseArrow
	TileImage* tile30;		// 30	QM_IncreaseArrow
	TileText* tile34;		// 34
	TileImage* tile38;		// 38
	TileImage* tile3C;		// 3C
	float				currentQtt;		// 40

	static QuantityMenu* GetSingleton() { return *(QuantityMenu**)0x11DA618; };
};


// 2C
struct HackingText
{
	enum
	{
		kDisplaysCharacterByCharacter = 0x1,
		kBlinks = 0x2,
	};

	String str;
	Tile* tileText;
	Tile::Value* tileValueVisibility;
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
STATIC_ASSERT(sizeof(HackingText) == 0x2C);

struct MenuEntry
{
	String entryText;
	String resultText;
	Script resScript;
	ConditionList conditions;
	BGSNote* displayNote;
	BGSTerminal* subMenu;
	UInt8 entryFlags;
	UInt8 pad75[3];
};
STATIC_ASSERT(sizeof(MenuEntry) == 0x78);

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

	HackingText* lines[39];
	UInt32 unk0C4[7];
	HackingText bottomLine;
	Tile* tile10C[8];
	Sound UIHackingFanHumLP;
	Sound UIHackingPassGood;
	Sound UIHackingPassBad;
	Sound UIHackingCharScroll;
	Sound UIHackingCharEnter;
	UInt32 tileChildList168[3];
	UInt32 tileChildList174[3];
	tList<char> words;
	UInt32 tileChildList188[3];
	TESObjectREFR* targetBaseRef;
	TESObjectREFR* targetRef;
	char* correctAnswer;
	UInt32 attemptsRemaining;
	UInt32 maxAttemptsRemaining;
	UInt32 numWordsRemaining;
	UInt32 correctAnswerLength;
	UInt32* unk1B0;
	UInt32 topTextHeight;
	UInt32 topTextLineHeight;
	UInt32 highlightedCharacterXIndex;
	UInt32 activeTile;
	UInt32 stage;
	TileImage* tile1C8;
	UInt32 unk1CC;
	UInt32 transitionToComputersMenuTime;
	UInt32* unk1D4;
	UInt8 shouldNotHighlightWordOnHover;
	UInt8 hasAllowanceBeenReplenished;
	UInt8 byte1DA;
	UInt8 pad1DB;

	static HackingMenu* GetSingleton() { return *(HackingMenu**)(0x11D95B8); }
};
STATIC_ASSERT(sizeof(HackingMenu) == 0x1DC);

// 0FC
class ComputersMenu : Menu
{
public:
	ComputersMenu();
	~ComputersMenu();

	enum Buttons
	{
		kComputersServer = 0x0,
		kComputersFileFirectoryListbox = 0x1,
		kComputersDisplayZone = 0x2,
		kComputersHeader1 = 0x3,
		kComputersHeader2 = 0x4,
		kComputersResultText = 0x5,
		kComputersCursor = 0x6,
		kComputersWelcome = 0x7,
		kComputersResultPrompt = 0x8,
		kComputersIntroWelcome = 0x9,
		kComputersIntroLogon = 0xA,
		kComputersIntroEnterPW = 0xB,
		kComputersIntroPWDisplay = 0xC,
		kComputersBackground = 0xD,
		kComputersDisplayZoneText = 0xE,
		kComputersSeparator = 0xF,
	};


	Tile* tile_computers_server;
	Tile* tile_computers_file_directory;
	Tile* tile_computers_display_zone;
	Tile* tile_computers_header1;
	Tile* tile_computers_header2;
	Tile* tile_computers_result_text;
	Tile* tile_computers_cursor;
	Tile* tile_computers_welcome;
	Tile* tile_computers_result_prompt;
	Tile* tile_computers_intro_welcome;
	Tile* tile_computers_intro_logon;
	Tile* tile_computers_intro_enterpw;
	Tile* tile_computers_intro_pwdisplay;
	Tile* tile_computers_background;
	Tile* tile_computers_display_zone_text;
	Tile* tile_computers_separator;
	ListBox<MenuEntry> terminalMenuItemsListBox;
	tList<void> subMenuBGSTerminals;
	UInt32 time0A0;
	Tile::Value* isComputersCursorVisible;
	BGSTerminal* targetRefBaseForm;
	TESObjectREFR* targetRef;
	UInt32 stage;
	String noteTextStr;
	const char* remainingNoteText; // pointer into the current note text, moves as you change page 
	UInt8 byte0C0;
	UInt8 isRenderingSubmenu;
	UInt8 gap0C2[2];
	Sound UIHackingFanHumLP;
	Sound UIHackingCharScroll;
	Sound UIHackingCharEnter;
	DList<HackingText>* hackingTexts;
	UInt32 playNoteAudioCallback;
	UInt32 showNoteTextCallback;
	UInt32 showNoteImageCallback;
	UInt32 unk0F8;

	void ShowOrAdvanceNote(BGSNote* note) { ThisCall(0x7590C0, this, note); }

	static ComputersMenu* GetSingleton() { return *(ComputersMenu**)0x11D9334; };
};
STATIC_ASSERT(sizeof(ComputersMenu) == 0xFC);

class SlotMachineMenu : public Menu
{
public:
	SlotMachineMenu();
	~SlotMachineMenu();

	enum Buttons
	{
		kSpin = 0x3,
		kIncreaseBet,
		kDecreaseBet,
		kPayoutList,
		kBetMax,
		kExit,
	};

	UInt32 stage;
	UInt32 unk2C[40];
	UInt32 betAmount;
	UInt32 unkCC[34];
};

class RouletteMenu : public Menu
{
public:
	RouletteMenu();
	~RouletteMenu();

	enum Buttons
	{
		kIncreaseBet = 0x6,
		kDecreaseBet
	};

	UInt32 stage;
	UInt32 unk2C[4079];
	UInt32 betAmount;
	UInt32 unk3FEC[10];
};

class BlackJackMenu : public Menu
{
public:
	BlackJackMenu();
	~BlackJackMenu();

	enum Suits
	{
		kBJHearts = 0x0,
		kBJClubs = 0x1,
		kBJDiamonds = 0x2,
		kBJSpades = 0x3,
	};

	enum Buttons
	{
		kCurrentBetDisplay = 0x0,
		kChipsDisplay = 0x1,
		kCasinoNameDisplay = 0x2,
		kHit = 0x3,
		kDouble_Deal = 0x4,
		kIncreaseBet = 0x5,
		kDecreaseBet = 0x6,
		kSurrender_MaxBet = 0x7,
		kStay_Exit = 0x8,
		kStatusText = 0x9,
	};

	struct BlackJackCard
	{
		UInt32 value;
		Suits suit;
		NiRefObject* ref8;
		NiRefObject* refC;
	};

	UInt32 stage;
	UInt32 unk2C;
	UInt32 unk30;
	Tile* tiles34[10];
	UInt32 unk5C;
	float blackjackPayout;
	float shufflePercent;
	UInt32 playerLuck;
	UInt32 currentBet;
	UInt32 unk70;
	UInt32 numCurrency;
	UInt8 isLuckyWin;
	UInt8 isUnluckyLoss;
	UInt8 gap7A[2];
	UInt32 unk7C;
	UInt8 byte_somethingWinningsQuest;
	UInt8 gap81[3];
	UInt32 unk84[3];
	BlackJackCard cards[208];
	tList<BlackJackCard>* dealerHandCards;
	tList<BlackJackCard>* playerHandCards;
	tList<BlackJackCard>* playerSplitHandCards;
	UInt32 numDecks;
	UInt32 numRemainingCards;
	UInt8 unkDA4;
	UInt8 gapDA5[3];
	float unkDA8;
	UInt8 dealerStandsOnSoft17;
	UInt8 gapDAD[3];
	UInt32 flags;
	TESModelTextureSwap* blackjackTable;
	NiRefObject* hand1Nif;
	NiRefObject* hand2Nif;
	NiRefObject* dealerNif;
	TESModelTextureSwap* chip1;
	TESModelTextureSwap* chip5;
	TESModelTextureSwap* chip10;
	TESModelTextureSwap* chip25;
	TESModelTextureSwap* chip100;
	TESModelTextureSwap* chip500;
	TESModelTextureSwap* chip1_Shadow;
	TESModelTextureSwap* chip5_Shadow;
	TESModelTextureSwap* chip10_Shadow;
	TESModelTextureSwap* chip25_Shadow;
	TESModelTextureSwap* chip100_Shadow;
	TESModelTextureSwap* chip500_Shadow;
	NiControllerManager* blackjackTableControllerManager;
	NiControllerManager* hand1ControllerManager;
	NiControllerManager* hand2ControllerManager;
	NiControllerManager* dealerControllerManager;
	NiControllerSequence* Players_Discard;
	NiControllerSequence* Dealers_Discard;
	NiControllerSequence* Hand1_Discard;
	NiControllerSequence* Hand1_01;
	NiControllerSequence* Hand1_02;
	NiControllerSequence* Hand1_03;
	NiControllerSequence* Hand1_04;
	NiControllerSequence* Hand1_05;
	NiControllerSequence* Hand1_06;
	NiControllerSequence* Hand1_07;
	NiControllerSequence* Hand2_Discard;
	NiControllerSequence* Hand2_01;
	NiControllerSequence* Hand2_02;
	NiControllerSequence* Hand2_03;
	NiControllerSequence* Hand2_04;
	NiControllerSequence* Hand2_05;
	NiControllerSequence* Hand2_06;
	NiControllerSequence* Hand2_07;
	NiControllerSequence* Dealer_Discard;
	NiControllerSequence* Dealer_01;
	NiControllerSequence* Dealer_02;
	NiControllerSequence* Dealer_03;
	NiControllerSequence* Dealer_04;
	NiControllerSequence* Dealer_05;
	NiControllerSequence* Dealer_06;
	NiControllerSequence* Dealer_07;
	NiControllerSequence* Dealer_Reveal;
};
STATIC_ASSERT(sizeof(BlackJackMenu) == 0xE70);


class ImageSpaceModifierInstance;
class DialogMenu : public Menu			// 1009
{
	struct DialogMenu070_00
	{
		char* str;
		UInt32 unk04[9];
		TESTopic* topic;
	};

	struct __declspec(align(4)) DialogMenu070
	{
		DialogMenu070_00* unk00;
		tList<tList<void>> listOfLists04;
		Actor* actor;
		UInt8 isChallengeSpeechFailure;
		UInt8 pad11[3];
		UInt32 unk14;
	};

public:
	DialogMenu();
	~DialogMenu();

	int stage;
	int field_2C;
	TileImage* tile030;		// 030
	TileText* tile034;		// 034
	TileText* tile038;		// 038
	TileImage* tile03C;		// 03C
	ListBox<int>		topicList;		// 040
	DialogMenu070* clickedMenuItem;// 070
	Sound	sound74;	// 074
	TESObjectREFR* partnerRef;	// 080
	UInt8 byte084;				// 84
	UInt8 gap085[3];			// 85
	float unk088;				// 88
	void* unk08C;				// 8C
	void* unk090;				// 90
	TESTopicInfo* infos094[16];	// 94
	void* unk0D4;				// D4
	UInt32 unk0D8[3];			// D8
	TESTopicInfo* infos0E4[8];	// E4
	UInt8 byte104;				// 104
	UInt8 gap105[3];			// 104
	int field_108;				// 108
	int lastMenuClickTime;		// 10C
	tList<TESTopicInfo> infos110;// 110
	tList<TESTopicInfo> infos118;// 118
	tList<TESTopicInfo> infos120;// 120
	float unk128;				// 128
	ImageSpaceModifierInstance* depthOfFieldIMOD; // 12C
	char bWasPlayerThirdPerson;	// 130
	char byte131;				// 131
	char byte132;				// 132
	char byte133;				// 133
	UInt32 unk134;				// 134
	char isBarterMenuShown;		// 138
	char bShouldShowHelpMessage;// 139
	char byte13A;				// 13A
	char byte13B;				// 13B

	bool IsNPCTalking();
	static DialogMenu* GetSingleton() { return *(DialogMenu**)0x11D9510; };
};
STATIC_ASSERT(sizeof(DialogMenu) == 0x13C);

// 230
class MapMenu : public Menu				// 1023
{
public:
	MapMenu();
	~MapMenu();

	enum MapMenuTabs
	{
		kLocalMap = 0x20,
		kWorldMap = 0x21,
		kQuests = 0x22,
		kMisc = 0x23,
		kRadio = 0x24,
	};

	TileText* tile028;		// 028	MM_MainRect\MM_HeadlineRect\MM_Headline_LocationInfo
	TileText* tile02C;		// 02C	MM_MainRect\MM_HeadlineRect\MM_Headline_TimeDateInfo
	TileImage* tile030;		// 030	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMap_ParentImage
	TileImage* tile034;		// 034	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMapCursor
	TileImage* tile038;		// 038	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMap_ParentImage
	TileImage* tile03C;		// 03C	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMapCursor
	TileRect* mapSelectionRectangle;		// 040	MM_MainRect\MM_Highlight_ClipWindow\MM_MapHighlightBox
	TileImage* tile044;		// 044	MM_MainRect\MM_QuestsList
	TileImage* tile048;		// 048	MM_MainRect\MM_NotesList
	TileImage* tile04C;		// 04C	MM_MainRect\MM_NotesList\MM_Notes_SelectedNoteHighlight
	TileImage* tile050;		// 050	MM_MainRect\MM_RadioStationList
	TileImage* tile054;		// 054	MM_MainRect\MM_ButtonRect\MM_ButtonA
	TileImage* tile058;		// 058	MM_MainRect\MM_ButtonRect\MM_ButtonX
	TileRect* tile05C;		// 05C	MM_MainRect\MM_DataRect
	TileImage* tile060;		// 060	MM_MainRect\MM_DataRect\MM_DataTextRect
	TileImage* tile064;		// 064	MM_MainRect\MM_DataRect\MM_QuestObjectivesList
	TileImage* tile068;		// 068	MM_MainRect\MM_WaveformRect\MM_Waveform
	TileRect* tile06C;		// 06C	MM_Tabline
	TileImage* tile070;		// 070	MM_MainRect\MM_ButtonRect\MM_ButtonY
	TileImage* tile074;		// 074	MM_MainRect\MM_ChallengeList
	TileImage* tile078;		// 078	MM_MainRect\MM_ChallengeList\MM_Challenge_SelectedHighlight
	TileText* tile07C;		// 07C	MM_MainRect\MM_Headline_ChallengeType
	UInt8							currentTab;		// 080
	UInt8							pad081[3];		// 081
	TileImage* tileWorldMap; // 084
	UInt32 unk088;
	UInt8 isShowAllNotes;
	UInt8 byte08D;
	UInt8 byte08E;
	UInt8 byte08F;
	BGSNote* currentNote;
	UInt32 timeNoteViewed;
	UInt32 someSoundsList[4];
	BSSimpleArray<String>			arr0A8;			// 0A8
	Sound* maybeHolotapeSound;
	UInt8 isHolotapeVoicePlaying;
	UInt8 pad0BD;
	UInt8 pad0BE;
	UInt8 pad0BF;
	float holotapeTotalTime;
	UInt32 time0C4;
	float radioVolume;
	TESQuest* selectedQuest;
	BGSQuestObjective* currentObjective;
	TileImage* mapMarker;		// 0D4
	tList<TESObjectREFR>			mapMarkerList;	// 0D8
	tList<TESObjectREFR>			doorList;		// 0E0
	float							flt0E8;			// 0E8
	float							flt0EC;			// 0EC
	float							flt0F0;			// 0F0
	float							flt0F4;			// 0F4
	UInt32							unk0F8[4];		// 0F8
	TESObjectCELL* cell108;		// 108
	TESWorldSpace* wspc10C;		// 10C
	UInt32							unk110;			// 110
	TESObjectREFR* lastExtDoor;	// 114
	TESObjectREFR* selectedMarker;// 118
	TESObjectCELL* cell11C;		// 11C
	NiPoint2						clickStartPos;	// 120
	UInt32							unk128;			// 128
	bool							fogOfWar;		// 12C
	UInt8							pad12D[3];		// 12D
	ListBox<TESQuest>				questList;		// 130
	ListBox<BGSNote>				noteList;		// 160
	ListBox<TESObjectREFR>			radioRefList;	// 190
	ListBox<BGSQuestObjective>		objectiveList;	// 1C0
	ListBox<TESChallenge>			challengeList;	// 1F0
	BSSimpleArray<Tile>				arr220;			// 220

	static MapMenu* GetSingleton() { return *(MapMenu**)0x11DA368; };
	static tList<TESObjectREFR> GetAvailableRadioStations() { return *(tList<TESObjectREFR>*)0x11DD59C; };
	static TESObjectREFR* GetCurrentRadioStation() { return *(TESObjectREFR**)0x11DD42C; };

	void Zoom(bool direction, float amount) { ThisCall(0x79C5A0, this, direction, amount); };

	void Move(float deltaX, float deltaY)
	{
		if (!this->tileWorldMap || (this->currentTab != MapMenu::kLocalMap && this->currentTab != MapMenu::kWorldMap))
		{
			return;
		}

		this->tileWorldMap->SetFloat(kTileValue_dragstartx, 1.0F, 1);
		this->tileWorldMap->SetFloat(kTileValue_dragstarty, 1.0F, 1);

		ThisCall(0xA01290, this->tileWorldMap, kTileValue_dragdeltax, deltaX);
		ThisCall(0xA01290, this->tileWorldMap, kTileValue_dragdeltay, deltaY);

		if (this->tile038)
		{
			this->clickStartPos.x = this->tile038->GetValueFloat(kTileValue_x);
			this->clickStartPos.y = this->tile038->GetValueFloat(kTileValue_y);
		}
	}

	void MoveDirection(MenuSpecialKeyboardInputCode direction)
	{
		switch (direction)
		{
		case kMenu_UpArrow:
			this->Move(0.0F, 20.0F);
			break;
		case kMenu_DownArrow:
			this->Move(0.0F, -20.0F);
			break;
		case kMenu_LeftArrow:
			this->Move(20.0F, 0.0F);
			break;
		case kMenu_RightArrow:
			this->Move(-20.0F, 0.0F);
			break;
		}
	}

	void RefreshWorldMap()
	{
		ThisCall(0xA04150, this->tile038);
		this->mapMarkerList.RemoveAll();

		// skip code moving the cursor
		WriteRelJump(0x79DF73, 0x79E053);

		// MapMenu::SetupWorldMap
		ThisCall(0x79CDB0, this);

		// restore code moving the cursor
		SafeWriteBuf(0x79DF73, "\x6A\x00\x68\xB1\x0F", 5);
	}

	void RefreshLocalMap()
	{
		ThisCall(0xA04150, this->tile030);

		// skip code moving the cursor
		WriteRelJump(0x79DF73, 0x79E053);

		// MapMenu::SetupLocalMap
		ThisCall(0x79D410, this);

		// restore code moving the cursor
		SafeWriteBuf(0x79DF73, "\x6A\x00\x68\xB1\x0F", 5);
	}
};
STATIC_ASSERT(sizeof(MapMenu) == 0x230);

// 104
class RecipeMenu : public Menu			// 1077
{
public:
	RecipeMenu();
	~RecipeMenu();

	TileImage* tile028;		// 028	RM_Items_LeftFilterArrow
	TileText* tile02C;		// 02C	RM_ItemsTitle
	TileImage* tile030;		// 030	RM_Items_RightFilterArrow
	TileImage* tile034;		// 034	RM_Items_InventoryList
	TileText* tile038;		// 038	RM_MadeAtVariable
	TileText* tile03C;		// 03C	RM_SkillRequirement
	TileImage* tile040;		// 040	RM_Items_IngredientList
	TileImage* tileAccept;		// 044	RM_ButtonX
	TileImage* tile048;		// 048	RM_ButtonB
	TileImage* tile04C;		// 04C	RM_ItemIcon
	TileRect* tile050;		// 050	RM_ItemData
	TileText* tile054;		// 054	RM_Items_IngredientList
	TileText* tile058;		// 058	RM_ContainerTitle
	TileText* tile05C;		// 05C	RM_SkillRequirementHeader
	TESObjectREFR* sourceRef;		// 060
	TESRecipeCategory* category;		// 064
	UInt32						menuSoundID;	// 068
	ListBox<TESRecipe>			recipeList;		// 06C
	ListBox<TESRecipe>* currentItems;	// 09C
	ListBox<RecipeComponent>	componentList;	// 0A0
	ListBox<Condition>			conditionList;	// 0D0
	UInt8 byte100;			// 100
	UInt8 pad101;			// 101
	UInt8 pad102;			// 102
	UInt8 pad103;			// 103

	static RecipeMenu* GetSingleton() { return *(RecipeMenu**)0x11D8E90; };
};



class Tile3D;
struct StartMenuOption;
class StartMenu : public Menu
{
public:
	~StartMenu();
	StartMenu();

	enum StartMenuFlags
	{
		kInStartMenu = 0x1,
		kHasChangedSettings = 0x2,
		kLoad = 0x4,
		kIsSaveMenuNotLoad = 0x8,
		kIsMainMenuShown = 0x10,
		kAreOptionsInitialised = 0x20,
		kShowDLCPopup = 0x400,
		kIsActionMapping = 0x1000,
		kShowCredits = 0x10000,
		kControllerDisconnected = 0x20000,
		kSomethingSave = 0x40000000,
		kShowMustRestartToSaveSettings = 0x400000,
		ksomething_credits = 0x2000000,
		kDeleteSave = 0x2000,
	};

	TileImage* tile028;
	TileImage* tile02C;
	TileImage* tile030;
	TileImage* tile034;
	TileImage* tile038;
	TileImage* tile03C;
	TileImage* tile040;
	TileImage* tile044;
	TileText* tile048;
	TileText* tile04C;
	TileText* versionString;
	TileImage* tileSaveImage;
	TileText* tile058;
	Tile3D* tile05C;
	TileImage* tile060;
	TileImage* tile064;
	TileText* tileActionMappingDeviceName;
	TileImage* tile06C;
	TileText* tile070;
	TileText* tile074;
	TileText* tile078;
	TileImage* tile07C;
	TileText* tile080;
	ListBox<StartMenuOption> options084;
	ListBox<StartMenuOption> settingsSubMenu;
	ListBox<StartMenuOption> optionsNewGameMenu;
	ListBox<StartMenuOption> actionMappingOptions;
	ListBox<int> listBox144;
	ListBox<BGSSaveLoadFileEntry> savesList;
	UInt32 unk1A4;
	UInt32 flags;
	UInt32 unk1AC;
	UInt32 unk1B0;
	StartMenuOption* brightnessSettingCallback;
	NiSourceTexture* texture1B8;
	UInt32 unk1BC;
	TileImage* tile1C0;
	TileImage* tile1C4;
	UInt32 currentControlsDevice;
	UInt32 unk1CC;
	UInt32 unk1D0;

	static StartMenu* GetSingleton() { return *(StartMenu**)0x11DAAC0; };
};
STATIC_ASSERT(sizeof(StartMenu) == 0x1D4);


// 10
struct StartMenuOption
{
	// which menus the option should be shown in
	enum WhichMenus
	{
		kMainMenu = 0x1,
		kPauseMenu = 0x2,
		kSettingsMenu = 0x4,
		kGameplayMenu = 0x8,
		kDisplayMenu = 0x10,
		kAudioMenu = 0x20,
		kControlsMenu = 0x40,
		kNewGameMenu = 0x80,
		kContinueGameMenu = 0x100,
		kQuitGameMenu = 0x2000,
	};

	UInt32* vtable;
	const char* displayString;
	void (*callback)(void);	// 08
	UInt32 data;

	static StartMenuOption* Create(const char*, void (*)(void), UInt32);
};

// 30
struct StartMenuUserOption : StartMenuOption
{
	enum Type
	{
		// kToggleCycles will show a right arrow at max value which will cycle it back to the lowest value
		kToggleCycles = 0,
		kToggleNoCycle = 1,
		kSlider = 2
	};

	const char* templateName;					// 10
	UInt32 currValue;							// 14
	UInt32 defaultValue;						// 18
	Type type;						// 1C
	UInt32 numOptions;							// 20
	UInt32 buttonWidth;							// 24
	const char** currValueName;					// 28
	void(__cdecl* onSelection)(StartMenuUserOption*);	// 2C

	StartMenuUserOption* Init(char*, void (*)(StartMenuUserOption*), WhichMenus, int, int, int, char**);
	StartMenuUserOption* Create(char*, void(__cdecl*)(StartMenuUserOption*), WhichMenus, int, int, int, char**);

	void UpdateVisibility() { ThisCall(0x7D4CE0, StartMenu::GetSingleton(), this, NULL); };
};


class LockPickMenu : public Menu
{
public:
	~LockPickMenu();
	LockPickMenu();

	UInt32 stage;			// 28
	TileRect* tile2C;		// 2C
	TileRect* tile30;		// 30
	TileRect* tile34;		// 34
	TileImage* tile38;		// 38
	TileText* tile3C;		// 3C
	TileImage* tile40;		// 40
	TileImage* tile44;		// 44
	TileText* tile48;		// 48
	TileImage* tile4C;		// 4C
	TileImage* tile50;		// 50
	TileImage* tile54;		// 54
	TileText* tile58;		// 58
	TileText* tile5C;		// 5C
	TileImage* tile60;		// 60
	TileImage* tile64;		// 64
	TileImage* tile68;		// 68
	TESObjectREFR* targetRef;		// 6C
	UInt32					lockLevel;		// 70
	UInt32					skillLevel;		// 74
	float					fSkillLevel;	// 78
	UInt32					unk7C;			// 7C
	UInt32					unk80;			// 80
	float					flt84;			// 84
	UInt32					sweetSpotLen;	// 88
	float					cylinderAngle;	// 8C
	float					pickAngle;		// 90
	float					pinHealth;		// 94
	UInt8					isForceRotate;	// 98
	UInt8					byte99;			// 99
	UInt8					pad9A[2];		// 99
	NiControllerSequence* ctrlSeq9C;		// 9C
	NiControllerSequence* ctrlSeqA0;		// A0
	NiControllerManager* ctrlManager;		// A4
	NiControllerSequence* ctrlSeqA8;		// A8
	NiControllerSequence* ctrlSeqAC;		// AC
	NiControllerSequence* ctrlSeqB0;		// B0
	NiQuaternion			quaternionB4;	// B4
	NiQuaternion			quaternionC4;	// C4
	NiQuaternion			quaternionD4;	// D4

	static LockPickMenu* GetSingleton() { return *(LockPickMenu**)0x11DA204; };
};
STATIC_ASSERT(sizeof(LockPickMenu) == 0xE4);

// D8
class FORenderedMenu
{
public:
	FORenderedMenu();
	~FORenderedMenu();

	virtual void	Destructor(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual void	Unk_05(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(void);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	Unk_0E(void);
	virtual void	Unk_0F(void);
	virtual void	Unk_10(void);
	virtual void	Unk_11(void);
	virtual void	Unk_12(void);
	virtual void	Unk_13(void);
	virtual void	Unk_14(void);

	NiAVObject* unk04;
	BSFadeNode* node08;
	UInt32 unk0C;
	NiSourceTexture* srcTexture;
	NiNode* node14;
	TileMenu* tileMenu;
	UInt32 unk1C[2];
	NiCamera* camera;
	UInt8 byte028;
	UInt8 gap028[3];
	UInt32 unk02C[5];
	UInt32 blurRadius;
	UInt32 blurIntensity;
	UInt32 unk048;
	UInt8 byte04C;
	UInt8 byte04D;
	UInt8 gap04E[2];
	float time050;
	float burstDuration;
	float burstIntensity2;
	float burstIntensity;
	UInt32 unk060[30];
};

class FOPipboyManager : public FORenderedMenu
{
public:
	FOPipboyManager();
	~FOPipboyManager();

	NiNode* pipboyScreenNode0D8;
	NiNode* unk0DC;
	NiTriStrips* unk0C0;
	NiRefObject* unk0C4;
	NiTriStrips* pipboyLightButton[3];
	NiNode* pipboyLightGlow[3];
	NiTriStrips* scrollKnobs[3];
	float unk10C[3];
	float pipboyKnobScrollPositions[3];
	float pipboyKnobScrollRates[3];
	float tabKnobMinPosition;
	UInt32 unk134;
	UInt32 unk138;
	UInt32 unk13C;
	float tabKnobMaxPosition;
	UInt8 byte144;
	UInt8 gap145[3];
	UInt32 unk148;
	UInt32 unk14C;
	UInt8 byte150;
	UInt8 gap151[3];
	Sound sound154;
	UInt8 byte160;
	UInt8 byte161;
	UInt8 byte162;
	UInt8 gap163;
	float lightFadeStartTime;
	float lightEffectFadeDuration;
	UInt8 isResetPipboyManager;
	UInt8 gap16D[3];
};
STATIC_ASSERT(sizeof(FOPipboyManager) == 0x170);

class LevelUpMenu : public Menu
{
public:
	LevelUpMenu();
	~LevelUpMenu();

	enum Buttons
	{
		kLevelUp_Reset = 0x6,
		kLevelUp_Done = 0x7,
		kLevelUp_Back = 0x8,
		kLevelUp_Skill = 0xB,
		kLevelUp_Perk = 0xC,
		kLevelUp_Minus = 0xD,
		kLevelUp_Plus = 0xE,
	};

	enum Pages
	{
		kSkillSelection = 0,
		kPerkSelection = 1
	};

	UInt32 currentPage; // 0 for skills, 1 for perks
	TileText* tileTitle;
	TileImage* tileSkills;
	TileImage* tilePerks;
	TileImage* tileSelectionIcon;
	TileText* tileSelectionText;
	TileText* tilePointCounter;
	TileImage* tileBtnReset;
	TileImage* tileBtnContinue;
	TileImage* tileBtnBack;
	TileImage* tileStatsIconBadge;
	UInt32 numAssignedSkillPoints;
	UInt32 numAssignedPerks;
	UInt32 numSkillPointsToAssign;
	UInt32 numPerksToAssign;
	ListBox<ActorValueCode> skillListBox;
	ListBox<BGSPerk> perkListBox;
	tList<BGSPerk> availablePerks; // perks to show in the perk listBox

	static LevelUpMenu* GetSingleton() { return *(LevelUpMenu**)0x11D9FDC; }
};
STATIC_ASSERT(sizeof(LevelUpMenu) == 0xCC);




class CaravanMenu : Menu
{
	CaravanMenu();
	~CaravanMenu();
public:
	UInt32 stage;
	UInt32 stage2;
	Tile* tile030[51];
	UInt32 time09C;
	TESObjectREFR* opponentRef;
	UInt32 unk104;
	UInt32 maxBet;
	float difficulty;
	UInt32 unk110;
	BSFadeNode** currencyBillGridNIF;
	BSFadeNode** currencyCoinGridNIF;
	BSFadeNode** currencyCoinNIFs[3];
	BSFadeNode** currencyBillNIFs[3];
	BSSimpleArray<NiAVObject> unk134;
	tList<void> list144;
	BSSimpleArray<NiAVObject> unk14C;
	tList<void> list15C;
	BSSimpleArray<SInt32> unk164[2];
	BSFadeNode** CaravanTableNIF;
	UInt32 unk188;
	UInt32 unk18C;
	UInt32 unk190;
	UInt32 unk194;
	UInt32 unk198[3 * 7];
	UInt32 unk1EC[3 * 7];
	UInt32 unk240;
	UInt32 unk244[4];
	UInt32 unk254;
	UInt32 unk258[6];
	UInt32 unk270;
	UInt32 unk274[6];
	UInt32 unk28C[2];
	UInt32 unk294;
	UInt32 unk298;
	UInt32 unk29C;
	UInt32 unk2A0;
	UInt32 unk2A4;
	UInt32 unk2A8;
	UInt32 unk2AC;
	UInt32 unk2B0;
	UInt32 unk2B4;
	UInt32 unk2B8;
	UInt32 unk2BC;
	UInt32 unk2C0;
	UInt32 unk2C4;
	UInt32 unk2C8;
	UInt32 unk2CC;
	UInt32 unk2D0;
	UInt32 unk2D4;
	UInt32 unk2D8;
	UInt32 unk2DC;
	UInt32 unk2E0;
	UInt32 unk2E4;
	UInt32 unk2E8;
	UInt32 unk2EC;
	UInt32 unk2F0;
	UInt32 unk2F4;
	UInt32 unk2F8;
	UInt32 unk2FC;
	UInt32 unk300;
	UInt32 unk304[6 * 7];
	UInt32 unk3AC[2 * 4 * 7 * 6];
	UInt32 unk8EC[4 * 24];
	BSSimpleArray<TESCaravanCard> cardA6C;
	BSSimpleArray<SInt32> sIntArraysA7C[6];
	UInt32 currentAnte;
	UInt32 opponentFunds;
	UInt32 totalPlayerFunds;
	UInt32 unkAE8;
	UInt32 numCards1;
	UInt32 numCards2;
	UInt32 unkAF4;
	UInt32 unkAF8;
	UInt32 unkAFC;
	UInt32 unkB00;
	BSSimpleArray<TESCaravanCard> cardB04;
	BSSimpleArray<TESCaravanCard> cardB14;
	BSSimpleArray<TESCaravanCard> cardB24;
	BSSimpleArray<TESCaravanCard> cardB34;
	BSSimpleArray<TESCaravanCard> cardsB44[42];
	UInt32 unkDE4;
	UInt32 unkDE8;
	UInt32 unkDEC;
	UInt32 unkDF0;
	UInt32 unkDF4;
	UInt32 unkDF8;
	UInt32 unkDFC;
	UInt32 unkE00;
	UInt32 unkE04;
	UInt32 unkE08;
	UInt32 unkE0C;
	UInt32 unkE10;
	UInt32 unkE14;
	UInt32 unkE18;
	UInt32 unkE1C;
	UInt32 unkE20;
	UInt32 unkE24;
	UInt32 unkE28;
	UInt32 unkE2C;
	UInt32 unkE30;
	UInt32 unkE34;
	UInt32 unkE38;
	float timeE3C;
	UInt32 unkE40;
	UInt32 unkE44;
	UInt32 unkE48;
	UInt32 unkE4C;
	UInt32 unkE50;
	UInt32 unkE54[6];
	UInt32 unkE6C;
	UInt32 unkE70;
	UInt32 flags;
	UInt8 hasShownTutorialMenu;
	UInt8 padE79[3];
	UInt32 helpMenuID;
	UInt32 lastPressedArrowKey;
	UInt8 byteE84;
	UInt8 byteE85;
	UInt8 byteE86;
	UInt8 padE87;
	static CaravanMenu* GetSingleton() { return *(CaravanMenu**)0x11D917C; };
};
STATIC_ASSERT(sizeof(CaravanMenu) == 0xE88);

class ItemModMenu : Menu
{
	~ItemModMenu();
	ItemModMenu();

public:
	TileRect* IMM_ItemName;
	TileImage* IMM_ItemModList;
	TileRect* IMM_LeftBracket;
	TileImage* IMM_ItemIcon;
	TileRect* IMM_ItemHealth;
	TileRect* IMM_ItemStat;
	TileText* IMM_HealthImprovementText;
	TileText* IMM_StatImprovementText;
	TileText* IMM_ChooseItemText;
	TileRect* IMM_ModItemHealth;
	TileRect* IM_ModItemStat;
	TileImage* IMM_ItemModButton;
	TileImage* IMM_CancelButton;
	TileText* IMM_ModDesc;
	MenuItemEntryList itemModList;

	static ItemModMenu* GetSingleton() { return *(ItemModMenu**)0x11D9F54; };
};

class TraitMenu : Menu
{
	TraitMenu();
	~TraitMenu();

public:
	TileText* tileTitleText;
	TileImage* tile2C;
	TileImage* tile30;
	TileText* tile34;
	TileText* tile38;
	TileImage* tileReset;
	TileImage* tileTitle;
	TileImage* tile44;
	TileImage* tile48;
	UInt32 numSelected;
	UInt32 maxSelect;
	ListBox<BGSPerk> perkListBox;
	tList<BGSPerk> perkList;

	static TraitMenu* GetSingleton() { return *(TraitMenu**)0x11DAF74; };
};




struct FontHeightData
{
	float		heightBase;
	float		heightwGap;
};
//s_fontHeightDatas[90];




struct FontTextReplaced;
struct __declspec(align(4)) FontTextReplaced
{
	String str;
	UInt32 wrapWidth;
	UInt32 unk00C;
	UInt32 unk010;
	UInt32 unk014;
	UInt32 unk018;
	UInt8 byte1C;
	UInt8 gap1D[3];
	tList<void> list20;

	FontTextReplaced();

	~FontTextReplaced();

	void GetVariableEscapedText(const char*);
};

STATIC_ASSERT(sizeof(FontTextReplaced) == 0x28);


// A0
class RepairServicesMenu : public Menu	// 1058
{
public:
	RepairServicesMenu();
	~RepairServicesMenu();

	TileText* tile28;		// 28
	TileText* tile2C;		// 2C
	TileImage* tile30;		// 30
	TileImage* tile34;		// 34
	TileRect* tile38;		// 38
	TileRect* tile3C;		// 3C
	TileRect* tile40;		// 40
	TileText* tile44;		// 44
	TileText* tile48;		// 48
	TileText* tile4C;		// 4C
	TileRect* tile50;		// 50
	TileRect* tile54;		// 54
	TileText* tile58;		// 58
	TileImage* tile5C;		// 5C
	TileImage* tile60;		// 60
	TileImage* tile64;		// 64
	MenuItemEntryList	itemList;		// 68
	UInt32				repairAllCost;	// 98
	UInt8				skill;			// 9C
	UInt8				pad9D[3];		// 9D

	static RepairServicesMenu* GetSingleton() { return *(RepairServicesMenu**)0x11DA7F0; };
	static Actor* GetActor() { return *(Actor**)0x11DA7F4; };
};

// 8C
class RepairMenu : public Menu			// 1035
{
public:
	RepairMenu();
	~RepairMenu();

	TileRect* tile28;		// 28
	TileImage* tile2C;		// 2C
	TileRect* tile30;		// 30
	TileImage* tile34;		// 34
	TileRect* tile38;		// 38
	TileRect* tile3C;		// 3C
	TileText* tile40;		// 40
	TileText* tile44;		// 44
	TileText* tile48;		// 48
	TileRect* tile4C;		// 4C
	TileRect* tile50;		// 50
	TileImage* tile54;		// 54
	TileImage* tile58;		// 58
	MenuItemEntryList		repairItems;	// 5C

	static RepairMenu* GetSingleton() { return *(RepairMenu**)0x11DA75C; };
	static ContChangesEntry* GetSelection() { return *(ContChangesEntry**)0x11DA760; };
};

void MenuButton_DownloadsClick();