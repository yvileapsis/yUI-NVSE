#pragma once
#include <GameObjects.h>
#include <GameTiles.h>
#include <GameAPI.h>
#include <GameExtraData.h>

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

enum SpecialInputCode
{
	kInputCode_Backspace = 0x80000000,
	kInputCode_ArrowLeft = 0x80000001,
	kInputCode_ArrowRight = 0x80000002,
	kInputCode_ArrowUp = 0x80000003,
	kInputCode_ArrowDown = 0x80000004,
	kInputCode_Home = 0x80000005,
	kInputCode_End = 0x80000006,
	kInputCode_Delete = 0x80000007,
	kInputCode_Enter = 0x80000008,
	kInputCode_PageUp = 0x80000009,
	kInputCode_PageDown = 0x8000000A
};

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
	kAddr_InitFontInfo = 0xA12020,
	kAddr_GetItemHealthPerc = 0x4BCDB0,
	kAddr_GetItemValue = 0x4BD400,
	kAddr_EvaluateConditions = 0x680C60,
	kAddr_EvaluateCondition = 0x681600,
};

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

struct InterfaceManagerStruct0178;

// 584
class InterfaceManager
{
public:
	struct Struct0178
	{
		UInt32 unk00;
		UInt32 NiTPrimitiveArray[9];
		UInt8 byte28;
		UInt8 byte29;
		UInt8 byte2A;
		UInt8 byte2B;
		UInt32 startTime;
		float durationX;
		float durationY;
		float intensityX;
		float intensityY;
		float frequencyX;
		float frequencyY;
		float unk48;
		float unk4C;
		UInt32 imageSpaceEffectParam;
		UInt8 isFlycamEnabled;
		UInt8 byte55;
		UInt8 byte56;
		UInt8 byte57;
		float fBlurRadiusHUD;
		float fScanlineFrequencyHUD;
		float fBlurIntensityHUD;
	};

	struct RefAndNiNode
	{
		TESObjectREFR* ref;
		NiNode* node;
	};

	struct VATSHighlightData
	{
		UInt32 mode;						// 000
		RefAndNiNode target;				// 004
		UInt32 numHighlightedRefs;			// 00C
		UInt32 flashingRefIndex;			// 010
		RefAndNiNode highlightedRefs[32];	// 014
		UInt32 unk114;						// 114
		UInt8 isOcclusionEnabled;			// 118
		UInt8 byte119[16];					// 119
		UInt8 byte12B;						// 12B
		UInt32 unk12C[16];					// 12C
		UInt32 unk16C[16];					// 16C
		UInt8 byte1AC[16];					// 1AC
		UInt32 unk1BC[16];					// 1BC
		UInt32 unk1FC[16];					// 1FC
		UInt32 selectedLimbID;				// 23C
		UInt32 count240;					// 240 indexes into unk244 when it is appended to
		UInt32 unk244[16];					// 244
		UInt8 byte284;						// 284
		UInt8 pad285[3];					// 285
		UInt32 unk288;						// 288
		float unk28C;						// 28C
		UInt32 unk290;						// 290
		float vatsDistortTime;				// 294
		UInt8 byte298;						// 298
		UInt8 pad299[3];					// 299
		float time29C;						// 29C
		float unk2A0;						// 2A0
		float unk2A4;						// 2A4
		float unk2A8;						// 2A8
		float unk2AC;						// 2AC
		float unk2B0;						// 2B0
		UInt8 byte2B4;						// 2B4
		UInt8 byte2B5;						// 2B5
		UInt8 pad2B6[2];					// 2B6
		float unk2B8;						// 2B8
		float unk2BC;						// 2BC
		float fVATSTargetPulseRate;			// 2C0
		NiRefObject* unk2C4;				// 2C4
		UInt32 unk2C8;						// 2C8
		NiRefObject* unk2CC;				// 2CC

		void AddRef(TESObjectREFR* ref);
		void AddRefAndSetFlashing(TESObjectREFR* ref);
		void __forceinline ResetRefs() { ThisCall(0x800ED0, this); };
	};

	struct Tutorials
	{
		SInt32 tutorialFlags[41];
		UInt32 currentShownHelpID;
		UInt32 timeA8;
	};

	enum PipboyTabs
	{
		kStats = 1,
		kItems,
		kData
	};

	enum KeyModifier
	{
		kAltHeld = 0x1,
		kControlHeld = 0x2,
		kShiftHeld = 0x4,
	};

	enum Mode
	{
		kGameMode = 1,
		kMenuMode
	};

	enum Helps
	{
		kHelpPipBoyItems = 0x5,
		kHelpPipBoyRepair = 0x6,
		kSPECIAL = 0xC,
		kHelpChargenTagSkills = 0xE,
		kHelpChargenRace = 0xF,
		kHelpLeveling = 0x10,
		kHelpDialogue = 0x11,
		kHelpHacking = 0x13,
		kHelpLockpickingPC = 0x14,
		kHelpVATSPC = 0x15,
		kHelpContainer = 0x16,
		kHelpBarter = 0x17,
		kHelpTerminal = 0x18,
		kHelpPipBoyStats = 0x19,
		kHelpMapMenu = 0x1A,
		kHelpVATSXbox = 0x1B,
		kHelpLockpickingXBox = 0x1C,
		kHelpItemMod = 0x1D,
		kHelpCaravanBetting = 0x1E,
		kHelpCaravanDeckBuilding = 0x1F,
		kHelpCaravanStartingCaravans = 0x20,
		kHelpCaravanContractWar = 0x21,
		kHelpWeapons = 0x22,
		kHelpApparel = 0x23,
		kHelpAmmo = 0x24,
		kHelpAmmoBox = 0x25,
		kHelpCrafting = 0x26,
		kHelpReputation = 0x27,
		kHelpHardcoreNeeds = 0x28,
	};
	
	UInt32					flags;				// 000
	SceneGraph*				sceneGraph004;		// 004
	SceneGraph*				sceneGraph008;		// 008
	UInt32					currentMode;		// 00C	1 = GameMode; 2 = MenuMode
	// checked for 1 at 0x70BA97
	// set to 2 at 0x70BA8D
	// set to 3 at 0x714E96, 0x714E20 - checked at 0x70B94E
	// set to 4 at 0x714D5D, 0x715177 (CloseAllMenus)
	// set to 5 at 0x70B972 - checked at 0x70BA84, 0x70CA14, 0x70CC7A
	UInt32					unk010;				// 010
	UInt32					unk014;				// 014
	UInt32					pickLength;			// 018
	UInt32					unk01C;				// 01C
	UInt8					byte020;			// 020
	UInt8					byte021;			// 021
	UInt8					byte022;			// 022
	UInt8					byte023;			// 023
	UInt32					unk024;				// 024
	TileImage*				cursor;				// 028
	float					flt02C;				// 02C
	float					flt030;				// 030
	float					flt034;				// 034
	float					cursorX;			// 038
	float					flt03C;				// 03C
	float					cursorY;			// 040
	float					mouseWheel;			// 044	-120.0 = down; 120.0 = up
	float					flt048;				// 048
	Tile*					draggedTile;		// 04C
	int						unk050;				// 050
	float					flt054;				// 054
	float					flt058;				// 058
	int						unk05C;				// 05C
	int						unk060;				// 060
	int						unk064;				// 064
	UInt32					unk068[2];			// 068
	TList<TESObjectREFR>	selectableRefs;		// 070
	UInt32					selectedRefIndex;	// 078
	bool					debugText;			// 07C
	UInt8					byte07D;			// 07D
	UInt8					byte07E;			// 07E
	UInt8					byte07F;			// 07F
	NiNode*					niNode080;			// 080
	NiNode*					niNode084;			// 084
	UInt32					unk088;				// 088
	void*					shaderAccum08C;		// 08C
	void*					shaderAccum090;		// 090
	void*					shadowScene094;		// 094
	void*					shadowScene098;		// 098
	Tile*					menuRoot;			// 09C
	Tile*					globalsTile;		// 0A0	globals.xml
	NiNode*					unk0A4;				// 0A4 saw Tile? seen NiNode
	UInt32					unk0A8;				// 0A8
	NiObject*				unk0AC;				// 0AC seen NiAlphaProperty
	UInt32					unk0B0[3];			// 0B0
	Tile					*activeTileAlt;		// 0BC
	UInt32					unk0C0;				// 0C0
	UInt32					unk0C4;				// 0C4
	UInt8					byte0C8;			// 0C8
	UInt8					byte0C9;			// 0C9
	UInt8					byte0CA;			// 0CA
	UInt8					byte0CB;			// 0CB
	Tile*					activeTile;			// 0CC
	Menu*					activeMenu;			// 0D0
	Tile*					tile0D4;			// 0D4
	Menu*					menu0D8;			// 0D8
	UInt32					unk0DC[2];			// 0DC
	UInt8					msgBoxButton;		// 0E4 -1 if no button pressed
	UInt8					byte0E5;			// 0E5
	UInt8					byte0E6;			// 0E6
	UInt8					byte0E7;			// 0E7
	UInt32					unk0E8;				// 0E8
	UInt8					byte0EC;			// 0EC
	UInt8					hasMouseMoved;		// 0ED
	UInt8					byte0EE;			// 0EE
	UInt8					byte0EF;			// 0EF
	TESObjectREFR*			debugSelection;		// 0F0	compared to activated object during Activate
	UInt32					unk0F4;				// 0F4
	UInt32					unk0F8;				// 0F8
	TESObjectREFR*			crosshairRef;		// 0FC
	UInt32					unk100[4];			// 100
	UInt8					byte110;			// 110
	UInt8					pad111[3];			// 111
	UInt32					menuStack[10];		// 114
	void*					ptr13C;				// 13C	Points to a struct, possibly. First member is *bhkSimpleShapePhantom
	UInt32					unk140;				// 140
	UInt32					unk144;				// 144
	UInt8					byte148;			// 148
	UInt8					isShift;			// 149
	UInt8					byte14A;			// 14A
	UInt8					byte14B;			// 14B
	KeyModifier				keyModifiers;		// 14C
	UInt32					currentKey;			// 150
	UInt32					keyRepeatStartTime;	// 154
	UInt32					lastKeyRepeatTime;	// 158
	UInt32					unk15C[4];			// 15C
	void*					renderedMenu;		// 16C
	UInt8					byte170;			// 170
	UInt8					byte171;			// 171
	UInt8					byte172;			// 172
	UInt8					byte173;			// 173
	FOPipboyManager*		pipboyManager;		// 174
	Struct0178				unk178;				// 178
	VATSHighlightData		vatsHighlightData;	// 1DC
	float					scale4AC;			// 4AC
	float					unk4B0;				// 4B0
	UInt8					isRenderedMenuOrPipboyManager;		// 4B4
	UInt8					byte4B5;			// 4B5
	UInt8					byte4B6;			// 4B6
	UInt8					byte4B7;			// 4B7
	UInt32					queuedPipboyTabToSwitchTo;			// 4B8
	UInt32					pipBoyMode;			// 4BC
	void					(*onPipboyOpenCallback)(void);		// 4C0
	UInt32					unk4C4[2];			// 4C4
	UInt8					byte4CC;			// 4CC
	UInt8					byte4CD;			// 4CD
	UInt8					pad4CE;				// 4CE
	UInt8					pad4CF;				// 4CF
	UInt32					unk4D0;				// 4D0
	Tutorials				help;				// 4D4

	InterfaceManager();
	~InterfaceManager();

	static InterfaceManager*	GetSingleton(void) { return *reinterpret_cast<InterfaceManager**>(0x11D8A80); };
	static bool					IsMenuVisible(const UInt32 menuType);
	static Menu *				GetMenuByType(const UInt32 menuType);
	static Menu *				TempMenuByType(const UInt32 menuType);
	static TileMenu*			GetMenuByPath(const char* componentPath, const char** slashPos);
	static Tile::Value*			GetMenuComponentValue(const char* componentPath);
	static Tile*				GetMenuComponentTile(const char* componentPath);
	static Tile::Value*			GetMenuComponentValueAlt(const char* componentPath);
	
	UInt32						GetTopVisibleMenuID();
	Tile*						GetActiveTile();
	static void	__forceinline	RefreshItemsList() { StdCall(0x704AF0); }
	static void					RefreshItemsListQuick();
	static bool					RefreshItemsListForm(TESForm* form = nullptr);
};
static_assert(sizeof(InterfaceManager) == 0x580);
static_assert(sizeof(InterfaceManager::Struct0178) == 0x64);


void Debug_DumpMenus(void);

enum
{
	kMenuType_Min =				0x3E9,
	kMenuType_Message =			kMenuType_Min,
	kMenuType_Inventory,
	kMenuType_Stats,
	kMenuType_HUDMain,
	kMenuType_Loading =			0x3EF,
	kMenuType_Container,
	kMenuType_Dialog,
	kMenuType_SleepWait =		0x3F4,
	kMenuType_Start,
	kMenuType_LockPick,
	kMenuType_Quantity =		0x3F8,
	kMenuType_Map =				0x3FF,
	kMenuType_Book =			0x402,
	kMenuType_LevelUp,
	kMenuType_Repair =			0x40B,
	kMenuType_RaceSex,
	kMenuType_Credits =			0x417,
	kMenuType_CharGen,
	kMenuType_TextEdit =		0x41B,
	kMenuType_Barter =			0x41D,
	kMenuType_Surgery,
	kMenuType_Hacking,
	kMenuType_VATS,
	kMenuType_Computers,
	kMenuType_RepairServices,
	kMenuType_Tutorial,
	kMenuType_SpecialBook,
	kMenuType_ItemMod,
	kMenuType_LoveTester =		0x432,
	kMenuType_CompanionWheel,
	kMenuType_TraitSelect,
	kMenuType_Recipe,
	kMenuType_SlotMachine =		0x438,
	kMenuType_Blackjack,
	kMenuType_Roulette,
	kMenuType_Caravan,
	kMenuType_Trait =			0x43C,
	kMenuType_Max =				kMenuType_Trait,
};

struct EventCallbackScripts;

template <typename Item> struct ListBoxItem
{
	Tile* tile;
	Item* object;
	UInt8 byte08;
	UInt8 pad09[3];
};


// 30
template <typename Item> class ListBox : public BSSimpleList<ListBoxItem<Item>>
{
public:
	enum
	{
		kFlag_RecalculateHeightsOnInsert = 1,
		kFlag_FreeContChangeOnListItemDestruction = 2, // assumes the object is a ContChangesEntry - do not set this if the object isn't one...
	};

	Tile*			parentTile;			// 0C
	Tile*			selected;			// 10
	Tile*			scrollBar;			// 14
	const char*		templateName;		// 18
	UInt16			itemCount;			// 1C
	UInt16			pad1E;				// 1E
	float			unk20;				// 20
	float			storedListIndex;	// 24
	float			storedScrollbarPos;	// 28
	UInt16			flags;				// 2C
	UInt16			pad2E;				// 2E

	Item* GetSelected()
	{
		TListNode<ListBoxItem<Item>>* iter = BSSimpleList<Item>::list.Head();
		do
		{
			const auto item = iter->data;
			if (item && (item->tile == selected))
				return item->object;
		} while ((iter = iter->next));
		return nullptr;
	}

	Tile* GetNthTile(SInt32 index)
	{
		if (index >= 0)
		{
			TListNode<ListBoxItem<Item>>* iter = BSSimpleList<Item>::list.Head();
			do
			{
				if (!index)
				{
					return iter->data ? iter->data->tile : NULL;
				}
				index--;
			} while ((iter = iter->next));
		}
		return nullptr;
	}

	void Clear()
	{
		TListNode<ListBoxItem<Item>>* iter = BSSimpleList<Item>::list.Head();
		do
		{
			const auto item = iter->data;
			if (!item) continue;
			if (item->tile)
				item->tile->Destroy(true);
			GameHeapFree(item);
		} while ((iter = iter->next));
		BSSimpleList<Item>::list.RemoveAll();
		selected = nullptr;
		itemCount = 0;
	}

	typedef bool(__cdecl* FilterFunction)(Item* form);
	void Filter(FilterFunction callback)
	{
		ThisCall(0x729FE0, this, callback);
	}

	// identical to Filter, but hooked by InventorySortButton for filtering contchanges
	void FilterAlt(FilterFunction callback)
	{
		ThisCall(0x730BB0, this, callback);
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

	Item* GetItemForTile(Tile* tile)
	{
		TListNode<ListBoxItem<Item>>* iter = BSSimpleList<Item>::list.Head();
		do
		{
			const auto item = iter->data;
			if (item && (item->tile == tile))
				return item->object;
		} while ((iter = iter->next));
		return nullptr;
	}

	void SaveScrollPosition()
	{
		ThisCall(0x7312E0, this);
	}

	int GetNumVisibleItems()
	{
		return ThisCall<int>(0x71AE60, this);
	}

	void RestorePosition(bool playSound = false)
	{
		ThisCall(0x731360, this, playSound);
	}

	Tile* Insert(Item* item, const char* text, signed int (*sortingFunction)(ListBoxItem<Item>* a1, ListBoxItem<Item>* a2) = nullptr, const char* _templateName = nullptr)
	{
		if (!this->parentTile) return nullptr;
		auto _template = _templateName ? _templateName : this->templateName;
		if (!_template) return nullptr;

		Tile* newTile = this->parentTile->AddTileFromTemplate(_template);
		if (!newTile->GetValue(kTileValue_id))
		{
			newTile->SetFloat(kTileValue_id, -1);
		}
		if (text)
		{
			newTile->SetString(kTileValue_string, text);
		}

		auto listItem = (ListBoxItem<Item>*)GameHeapAlloc(sizeof(ListBoxItem<Item*>));
		listItem->tile = newTile;
		listItem->object = item;
		listItem->byte08 = 0;
		if (sortingFunction)
		{
			ThisCall(0x7A7EB0, &this->list, listItem, sortingFunction); // InsertSorted
			if (this->flags & kFlag_RecalculateHeightsOnInsert)
			{
				ThisCall(0x71A670, this);
			}
		}
		else
		{
			this->list.Append(listItem);
			if (this->flags & kFlag_RecalculateHeightsOnInsert)
			{
				ThisCall(0x7269D0, this, newTile);
				ThisCall(0x71AD30, this);
			}
			newTile->SetFloat(kTileValue_listindex, this->itemCount++);
		}

		if (this->itemCount == 1)
		{
			auto numVisibleItemsTrait = Tile::TraitNameToID("_number_of_visible_items");
			if (this->parentTile->GetFloat(numVisibleItemsTrait) > 0)
			{
				auto valPtr = ThisCall<Tile::Value*>(0xA00E90, this->parentTile, kTileValue_height);
				ThisCall(0xA09200, valPtr);
				ThisCall(0xA09130, valPtr, 2000, newTile, kTileValue_height);

				auto numVisible = this->parentTile->GetFloat(numVisibleItemsTrait);
				ThisCall(0xA09080, valPtr, kTileValue_Mul, numVisible);
				ThisCall(0xA09410, valPtr, 0);
			}
		}

		return newTile;
	}

	Tile* InsertVal(Item item, const char* text, signed int (*sortingFunction)(ListBoxItem<Item>* a1, ListBoxItem<Item>* a2) = nullptr, const char* _templateName = nullptr)
	{
		return ThisCall<Tile*>(0x754690, this, item, text, sortingFunction, _templateName);
	}

	void HighlightLastItem()
	{
		int lastIndex = this->itemCount - 1;
		Tile* tile = this->GetNthTile(lastIndex);

		this->SetSelectedTile(tile);
		this->ScrollToHighlight();
	}

	void ScrollToTop()
	{
		this->storedScrollbarPos = 0;
		this->storedListIndex = 0;
		this->RestorePosition();
	}

	void SetParentEnabled(bool isEnabled)
	{
		static UInt32 enabledTrait = Tile::TraitNameToID("_enabled");
		parentTile->SetFloat(enabledTrait, isEnabled);
	}

	bool IsEnabled()
	{
		static UInt32 enabledTrait = Tile::TraitNameToID("_enabled");
		return parentTile && parentTile->GetFloat(enabledTrait);
	}

	void Init()
	{
		// initialises the fields and appends the menu list to the global listbox array
		ThisCall(0x723750, this);
	}

	void Destroy()
	{
		ThisCall(0x723820, this);
	}
};

class Menu
{
public:
	Menu() { ThisCall(0xA1C4A0, this); };
	~Menu() { ThisCall(0xA1C520, this); };

	enum vtable_offsets
	{
		kDestructor = 0,
		kSetTile = 4,
		kHandleLeftClickPress = 8,
		kHandleClick = 0xC,
		kHandleMouseover = 0x10,
		kHandleUnmouseover = 0x14,
		kPostDragTileChange = 0x18,
		kPreDragTileChange = 0x1C,
		kHandleActiveMenuClickHeld = 0x20,
		kOnClickHeld = 0x24,
		kHandleMousewheel = 0x28,
		kUpdate = 0x2C,
		kHandleKeyboardInput = 0x30,
		kGetID = 0x34,
		kHandleSpecialKeyInput = 0x38,
		kHandleControllerInput = 0x3C,
		kOnUpdateUserTrait = 0x40,
		kHandleControllerConnectOrDisconnect = 0x44
	};
	
	virtual void	Destructor(bool freeMemory);
	virtual void	SetTile(UInt32 tileID, Tile* value);
	virtual void	HandleLeftClick(UInt32 tileID, Tile* activeTile);
	virtual void	HandleClick(UInt32 tileID, Tile* clickedButton);	// buttonID = <id> trait defined in XML
	virtual void	HandleMouseover(UInt32 tileID, Tile* tile);	//called on mouseover, activeTile is moused-over Tile
	virtual void	HandleUnmouseover(UInt32 tileID, Tile* tile);
	virtual void	PostDragTileChange(UInt32 tileID, Tile* newTile, Tile* activeTile);
	virtual void	PreDragTileChange(UInt32 tileID, Tile* oldTile, Tile* activeTile);
	virtual void	HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile);
	virtual void	OnClickHeld(UInt32 tileID, Tile* activeTile);
	virtual void	HandleMousewheel(UInt32 tileID, Tile* tile);
	virtual void	Update(void);
	virtual bool	HandleKeyboardInput(UInt32 inputChar);	//for keyboard shortcuts, return true if handled
	virtual UInt32	GetID(void);
	virtual bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState);
	virtual bool	HandleControllerInput(int a2, Tile* tile) { return false; };
	virtual void    OnUpdateUserTrait(int tileVal);
	virtual void	HandleControllerConnectOrDisconnect(bool isControllerConnected);

	// 14
	struct TemplateInstance
	{
		UInt32		unk00;		// 00
		Float32		flt04;		// 04
		String		tileName;	// 08
		Tile*		tile;		// 10
	};
	
	// 14
	struct TemplateData
	{
		const char*				templateName;	// 00
		TileExtra*				tileExtra;		// 04
		DList<TemplateInstance>	instances;		// 08
	};
	
	TileMenu*			tile{};				// 04
	TList<TemplateData>	menuTemplates;		// 08
	UInt32				unk10{};			// 10
	UInt32				unk14{};			// 14	Same as id (20)
	UInt32				unk18{};			// 18
	UInt8				byte1C{};			// 1C
	UInt8				byte1D{};			// 1D
	UInt8				byte1E{};			// 1E
	UInt8				byte1F{};			// 1F
	UInt32				id{};				// 20
	UInt32				visibilityState{};	// 24 :: Initialised to 4
	// check 1 at 0xA0B174, 0x70D529, 0x70D592 :: set at 0x712224
	// check 2 at 0x711FF1 :: set 2 at 0xA1D987 (when closes menu), 0xA1DA41
	// check 4 at 0xA1D9EC (when closing menu) :: set at 0x7036A4, 0x71204D
	// check 8 at 0x712194 :: set 8 at 0xA1DB8F (when opening menu), 0x720B39

	Menu*					HandleMenuInput(UInt32 tileID, Tile* clickedTile);
	__forceinline Tile*		AddTileFromTemplate(Tile* destTile, const char* templateName, UInt32 arg3 = 0)	{ return ThisCall<Tile*>(0xA1DDB0, this, destTile, templateName, arg3); }
	bool					GetTemplateExists(const char* templateName);

	void					Close()
	{
		this->tile->SetFloat(6002, 1);
		ThisCall(0xA1D910, this);
	}

	__forceinline void			Free() { ThisCall(0xA1C520, this); }

	__forceinline void			RegisterTile(Tile* tile, bool noAppendToMenuStack) { ThisCall(0xA1DC70, this, tile, noAppendToMenuStack); };
	__forceinline void			HideTitle(bool noFadeIn) { ThisCall(0xA1DC20, this, noFadeIn); };
};

class MenuItemEntryList : public ListBox<ContChangesEntry>
{
public:
	void RestoreScrollPositionProxy(Float32 listIndex, Float32 scrollbarPos) { ThisCall(0x7832E0, this, listIndex, scrollbarPos); }
};

struct ContainerBarterItems
{
	// structure used in both the container and barter menus
	SInt32				leftFilter;
	SInt32				rightFilter;
	UInt32				menuSoundID;
	MenuItemEntryList	leftItems;
	MenuItemEntryList	rightItems;
	MenuItemEntryList*	currentItems;
};


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

	enum Buttons
	{
		CM_Items_LeftFilterArrow = 0x0,
		CM_ItemsTitle = 0x1,
		CM_Items_RightFilterArrow = 0x2,
		CM_Items_CapsLabel = 0x3,
		CM_Items_InventoryList = 0x4,
		CM_Container_LeftFilterArrow = 0x5,
		CM_ContainerTitle = 0x6,
		CM_Container_RightFilterArrow = 0x7,
		CM_Container_InventoryList = 0x8,
		CM_ArrowIcon = 0x9,
		CM_TakeAllButton = 0xA,
		CM_ExitButton = 0xB,
		CM_ItemIcon = 0xC,
		CM_ItemData = 0xD,
		CM_list_template_container = 0x14,
	};

	enum FilterCategory
	{
		kAllItems,
		kWeapons,
		kArmor,
		kAid,
		kMisc,
		kAmmo,
		kMAX = kAmmo,
	};

	TileImage*			tile028;		// 028
	TileText*			itemsTitle;		// 02C
	TileImage*			tile030;		// 030
	TileText*			tileInventoryWeight;		// 034
	TileImage*			tile038;		// 038
	TileImage*			tile03C;		// 03C
	TileText*			containerTitle;	// 040
	TileImage*			tile044;		// 044
	TileImage*			tile048;		// 048
	TileImage*			tile04C;		// 04C
	TileImage*			takeAllTile;	// 050
	TileImage*			tile054;		// 054
	TileImage*			tile058;		// 058
	TileRect*			tile05C;		// 05C
	TileRect*			tile060;		// 060
	TileRect*			tile064;		// 064
	TileRect*			tile068;		// 068
	TileRect*			tile06C;		// 06C
	TileRect*			tile070;		// 070
	TESObjectREFR*		containerRef;	// 074
	TList<void>			list078;		// 078
	UInt32				mode;			// 080
	UInt32				valueTransfered;// 084
	UInt8				hasPickedPocket;// 088
	UInt8				hasFailedPickpocket;	// 089
	UInt8				pad08A;			// 08A
	UInt8				pad08B;			// 08B
	SInt32				leftFilter;		// 08C
	SInt32				rightFilter;	// 090
	UInt32				menuSoundID;	// 094
	MenuItemEntryList	leftItems;		// 098
	MenuItemEntryList	rightItems;		// 0C8
	MenuItemEntryList*	currentItems;	// 0F8
	UInt32				unk0FC;			// 0FC
	Sound				menuSound;		// 100

	static ContainerMenu*		GetSingleton() { return *reinterpret_cast<ContainerMenu**>(0x11D93F8); }
	static ContChangesEntry*	GetSelection() { return *reinterpret_cast<ContChangesEntry**>(0x11D93FC); }
	static void					SetSelection(ContChangesEntry* entry) { *reinterpret_cast<ContChangesEntry**>(0x11D93FC) = entry; }
	void __forceinline			Refresh(TESForm* form = nullptr) { ThisCall(0x75C280, form); }

};

class BookMenu : public Menu
{
public:
	BookMenu();
	~BookMenu();

	TileImage*		tile28;
	TileImage*		tile2C;
	Actor*			activator;
	TESObjectBOOK*	bookObject;
	UInt32			unk38;
};

struct HotKeyWheel
{
	TileRect*	parent;
	TileRect*	hotkeys[8];
	UInt8		byte24;
	UInt8		gap25[3];
	UInt32		selectedHotkey;
	UInt32		selectedHotkeyTrait;
	UInt32		selectedTextTrait;

	void SetVisible(bool isVisible) { ThisCall(0x701760, this, isVisible); };
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
		char	msg[0x104];
		bool	byte104;
		bool	byte105;
	};

	struct SubtitleData
	{
		char		text[MAX_PATH];
		UInt32		startTime;
		float		displayTime;
		NiPoint3	pos;
		UInt32		priority;
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
			TileImage*		tileHitPointsMeter;		// 02C	HitPoints\meter
			TileText*		tileHitPointsText;		// 030	HitPoints\justify_right_text
			TileRect*		tileHitPointsCompassIcon1;		// 034	HitPoints\compass_window\compass_icon_group
			TileRect*		tileHitPointsCompassIcon2;		// 038	HitPoints\compass_window\compass_icon_group
			TileRect*		tileHitPointsCompassIcon3;		// 03C	HitPoints\compass_window\compass_icon_group
			TileImage*		tileHitPointsCompass;		// 040	HitPoints\compass_window
			TileImage*		tileActionPointsMeter1;		// 044	ActionPoints\meter
			TileText*		tileActionPointsMeterText1;		// 048	ActionPoints\justify_right_text
			TileText*		tileActionPointsMeterText2;		// 04C	ActionPoints\justify_right_text
			TileImage*		tileActionPointsMeter2;		// 050	ActionPoints\meter
			TileImage*		tileActionPointsMeter3;		// 054	ActionPoints\MeterBackground
			TileText*		tileActionPointsMeterText3;		// 058	ActionPoints\justify_right_text
			TileRect*		tile05C;		// 05C	QuestReminder\QuestStages
			TileRect*		tile060;		// 060	QuestReminder\QuestAdded
			TileText*		tile064;		// 064	Region_Location\justify_left_text
			TileText*		tile068;		// 068	Region_Location\justify_left_text
			TileImage*		tile06C;		// 06C	RadiationMeter\radiation_bracket
			TileImage*		tile070;		// 070	RadiationMeter\radiation_pointer
			TileText*		tile074;		// 074	RadiationMeter\radiation_text_value
			TileText*		tile078;		// 078	RadiationMeter\radiation_text
			TileImage*		tile07C;		// 07C	EnemyHealth\enemy_health_bracket
			TileImage*		tile080;		// 080	EnemyHealth\meter
			TileText*		tile084;		// 084	EnemyHealth\justify_center_text
			TileText*		sneakLabel;	// 088	SneakMeter\sneak_nif
			TileImage*		tile08C;		// 08C	Messages\message_icon
			TileText*		tile090;		// 090	Messages\justify_left_text
			TileImage*		tile094;		// 094	Messages\message_bracket
			TileText*		tile098;		// 098	Subtitles\justify_center_text
			TileRect*		tileTakePrompt;		// 09C	Info\justify_center_hotrect
			TileText*		tile0A0;		// 0A0	Info\justify_center_hotrect\PCShortcutLabel
			TileImage*		tile0A4;		// 0A4	Info\justify_center_hotrect\xbox_button
			TileText*		tile0A8;		// 0A8	Info\justify_center_text
			TileText*		tileLockLevelStr;	// 0AC	Info\justify_center_text
			TileText*		tile0B0;		// 0B0	Info\justify_center_text
			TileText*		tilePickupWeightStr;	// 0B4	Info\justify_right_text
			TileText*		tilePickupWG;			// 0B8	Info\justify_left_text
			TileText*		tilePickupValueStr;	// 0BC	Info\justify_right_text
			TileText*		tilePickupVAL;		// 0C0	Info\justify_left_text
			TileImage*		tile0C4;		// 0C4	Info\info_seperator
			TileRect*		tile0C8;		// 0C8	Hokeys\hotkey_selector
			TileText*		tile0CC;		// 0CC	Hokeys\justify_center_text
			TileImage*		tile0D0;		// 0D0	HitPoints\left_bracket
			TileImage*		tile0D4;		// 0D4	ActionPoints\right_bracket
			TileImage*		tile0D8;		// 0D8	XPMeter\XPBracket
			TileText*		tile0DC;		// 0DC	XPMeter\XPAmount
			TileText*		tile0E0;		// 0E0	XPMeter\XPLabel
			TileImage*		tile0E4;		// 0E4	XPMeter\XPPointer
			TileText*		tile0E8;		// 0E8	XPMeter\XPLastLevel
			TileText*		tile0EC;		// 0EC	XPMeter\XPNextLevel
			TileText*		tile0F0;		// 0F0	XPMeter\XPLevelUp
			TileImage*		tile0F4;		// 0F4	ReticleCenter\reticle_center
			TileImage*		tileCrippledLimbFace;			// 0F8	crippled_limb_indicator\Face
			TileImage*		tileCrippledLimbHead;			// 0FC	crippled_limb_indicator\Head
			TileImage*		tileCrippledLimbTorso;			// 100	crippled_limb_indicator\Torso
			TileImage*		tileCrippledLimbLeftArm;		// 104	crippled_limb_indicator\Left_Arm
			TileImage*		tileCrippledLimbRightArm;		// 108	crippled_limb_indicator\Right_Arm
			TileImage*		tileCrippledLimbLeftLeg;		// 10C	crippled_limb_indicator\Left_Leg
			TileImage*		tileCrippledLimbRightLeg;		// 110	crippled_limb_indicator\Right_Leg
			TileRect*		tileActionPoints;				// 114	ActionPoints
			TileRect*		tileHitPoints;					// 118	HitPoints
			TileRect*		tileRadiationMeter;				// 11C	RadiationMeter
			TileRect*		tileEnemyHealth;				// 120	EnemyHealth
			TileRect*		tileQuestReminder;				// 124	QuestReminder
			TileRect*		tileRegionLocation;				// 128	Region_Location
			TileRect*		tileReticleCenter;				// 12C	ReticleCenter
			TileRect*		tileSneakMeter;					// 130	SneakMeter
			TileRect*		tileMessages;					// 134	Messages
			TileRect*		tileInfo;						// 138	Info
			TileRect*		tileSubtitles;					// 13C	Subtitles
			TileRect*		tileHotkeys;					// 140	Hokeys
			TileRect*		tileXPMeter;					// 144	XPMeter
			TileRect*		tileBreathMeter;				// 148	BreathMeter
			TileRect*		tileExplosivePositioningRect;	// 14C	Explosive_positioning_rect
			TileRect*		tileCrippledLimbIndicator;		// 150	crippled_limb_indicator
			TileImage*		tileDDTIcon;					// 154	DDTIcon
			TileImage*		tileDDTIconEnemy;				// 158	DDTIconEnemy
			TileText*		tileAmmoTypeLabel;				// 15C	AmmoTypeLabel
			TileRect*		tileHardcoreMode;				// 160	HardcoreMode
			TileText*		tileHardcoreModeDehydration;	// 164	HardcoreMode\Dehydration
			TileText*		tileHardcoreModeSleep;			// 168	HardcoreMode\Sleep
			TileText*		tileHardcoreModeHunger;			// 16C	HardcoreMode\Hunger
			TileImage*		tileDDTIconAP;					// 170	DDTIcon
			TileImage*		tileDDTIconEnemyAP;				// 174	DDTIconEnemyAP
			TileText*		tileHardcoreModeRads;			// 178	HardcoreMode\Rads
			TileText*		tileHardcoreModeLMBs;			// 17C	HardcoreMode\LMBs
			TileImage*		tileCNDArrows;					// 180	CNDArrows
		};
	};

	UInt32						unk184;					// 184
	float						flt188;					// 188
	TList<QueuedMessage>		queuedMessages;			// 18C
	UInt32						currMsgKey;				// 194
	BSSimpleArray<SubtitleData>	subtitlesArr;			// 198
	SubtitleData*				currentSubtitle;		// 1A8 
	Sound						radiationSound;			// 1AC
	TESObjectREFR*				crosshairRef;			// 1B8
	UInt32						visibilityFlags;		// 1BC
	Bitfield32					visibilityOverrides;	// 1C0
	UInt32						stage;					// 1C4
	HotKeyWheel					hotkeyWheel;			// 1C8	Hokeys\hotkey_selector
	UInt8						isUsingScope;			// 1FC
	UInt8						byte1FD;				// 1FD
	UInt8						byte1FE;				// 1FE
	UInt8						byte1FF;				// 1FF
	NiControllerSequence*		niContSeq;				// 200
	UInt8						isLevelUpQueued;		// 204
	UInt8						autoDisplayObjectives;	// 205
	UInt8						pad206[2];				// 206
	UInt32						unk208;					// 208
	UInt32						questTargets;			// 20C
	UInt32						unk210;					// 210
	UInt32						compassWidth;			// 214
	UInt32						maxCompassAngle;		// 218
	Actor*						healthTarget;			// 21C
	UInt32						unk220;					// 220
	Timer						timer224;				// 224
	UInt32						unk244;					// 244
	UInt32						unk248[4];				// 248
	TList<QueuedQuestText>		queuedQuestTextList;	// 258
	UInt8						byte260;				// 260
	UInt8						byte261;				// 261
	UInt8						pad262[2];				// 262
	TList<UInt32>				xpMessages;				// 264
	TList<UInt32>				list26C;				// 26C
	float						hudShake;				// 274

	static HUDMainMenu* GetSingleton() { return *reinterpret_cast<HUDMainMenu**>(0x11D96C0); }
	static void __cdecl SetQuestUpdateText(char* src, bool a2, bool a3) { CdeclCall(0x77A5B0, src, a2, a3); }
	static float GetOpacity() { return *(float*)0x11D979C; };
	void RemoveQueuedQuestAndLocationUpdates();
	void RemoveCrippledLimbIndicator();
};
static_assert(sizeof(HUDMainMenu) == 0x278);


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

	union
	{
		Tile* tile028[38];
		struct
		{
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
		};
	};
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
	TList<TESLoadScreen> loadScrList;
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

	static LoadingMenu* GetSingleton() { return *reinterpret_cast<LoadingMenu**>(0x11DA0C0); };
};
static_assert(sizeof(LoadingMenu) == 0x5C0);



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
	SInt32 currPoints;
	SInt32 numPoints;
	float listIndex;
	UInt8 isModValues;
	UInt8 showInitialTaggedSkills;
	UInt8 gap056[2];
	ListBox<UInt32> actorValues;

	static CharGenMenu* GetSingleton() { return *reinterpret_cast<CharGenMenu**>(0x11D920C); };
};
static_assert(sizeof(CharGenMenu) == 0x88);

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

		TList<StatusData> statusDataList;
	};

	class NamedEffectsList
	{
	public:
		TList<void> list04;
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

	bool ScrollNotes(MenuSpecialKeyboardInputCode direction, bool scaleScroll);

	static StatsMenu* GetSingleton() { return *reinterpret_cast<StatsMenu**>(0x11DACE0); };
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
	static CompanionWheelMenu* GetSingleton() { return *reinterpret_cast<CompanionWheelMenu**>(0x11D92B8); };
};
static_assert(sizeof(CompanionWheelMenu) == 0x88);


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
	TList<void> list144;
	BSSimpleArray<NiAVObject> unk14C;
	TList<void> list15C;
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
	static CaravanMenu* GetSingleton() { return *reinterpret_cast<CaravanMenu**>(0x11D917C); };
};
static_assert(sizeof(CaravanMenu) == 0xE88);

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

	static ItemModMenu* GetSingleton() { return *reinterpret_cast<ItemModMenu**>(0x11D9F54); };
	static ContChangesEntry* GetTarget() { return *reinterpret_cast<ContChangesEntry**>(0x11D9F58); };
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
	TList<BGSPerk> perkList;

	static TraitMenu* GetSingleton() { return *reinterpret_cast<TraitMenu**>(0x11DAF74); };
};


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

	static RepairServicesMenu*	GetSingleton() { return *reinterpret_cast<RepairServicesMenu**>(0x11DA7F0); };
	static Actor*				GetActor() { return *reinterpret_cast<Actor**>(0x11DA7F4); };
	void __forceinline			Refresh() { ThisCall(0x7B8B30, this); }
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

	static RepairMenu* GetSingleton() { return *reinterpret_cast<RepairMenu**>(0x11DA75C); };
	static ContChangesEntry* GetSelection() { return *reinterpret_cast<ContChangesEntry**>(0x11DA760); };
	static void SetSelection(ContChangesEntry* entry) { *reinterpret_cast<ContChangesEntry**>(0x11DA760) = entry; };
};

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
static_assert(sizeof(VATSTargetInfo) == 0x28);

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

	union
	{
		Tile* tiles[31];
		struct
		{
			TileImage* tile02C;
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
		};
	};

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

	static VATSMenu* GetSingleton() { return *reinterpret_cast<VATSMenu**>(0x11DB0D4); };
	static TESObjectREFR* GetTarget() { return *reinterpret_cast<TESObjectREFR**>(0x11F21CC); };
};
static_assert(sizeof(VATSMenu) == 0x144);

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

	enum Buttons
	{
		kButton_HotKeyWheel = 0x5,
		kButton_Equip = 0x6,
		kButton_Drop = 0x7,
		kButton_Repair = 0x8,
		kButton_HotkeyButton = 0x9,
		kButton_Cancel = 0xA,
		kButton_ModMenu = 0x13,
	};


	struct ScrollPos
	{
		SInt32				listIndex;
		SInt32				currentValue;
	};

	TileRect*				tileCapsInfo;
	TileRect*				tilePlayerHPInfo;
	TileRect*				tilePlayerDRInfo;
	TileRect*				tilePlayerWGInfo;
	TileImage*				tileInventoryList;
	TileRect*				tileHotKeyWheel;
	TileImage*				tileEquipButton;
	TileImage*				tileDropButton;
	TileImage*				tileRepairButton;
	TileImage*				tileHotkeyButton;
	TileImage*				tileCancelButton;
	TileImage*				tileItemIcon;
	TileRect*				tileItemInfoRect;
	TileRect*				tileTabline;
	TileRect*				tileDAMInfo;
	TileRect*				tileDPSInfo;
	TileRect*				tileStrengthReqInfo;
	TileRect*				tileDamResistInfo;
	TileRect*				tileDamThresholdInfo;
	TileImage*				tileModButton;
	TileImage*				tileItemIconBadge;
	TileRect*				tilePlayerDTInfo;
	TileText*				tileStrReq;
	UInt32					filter;					// 084
	ScrollPos				tabScrollPositions[6];	// 088, the scroll index for Weapons, Apparel, Aid, Misc, Ammo and the Keyring
	MenuItemEntryList		itemsList;				// 0B8
	HotKeyWheel				hotkeyWheel;			// 0E8
	TList<ContChangesEntry> changedItemsList;		// 11C

	static InventoryMenu*		GetSingleton() { return *reinterpret_cast<InventoryMenu**>(0x11D9EA4); }
	static UInt32				GetMenuID() { return 1002; }
	static ContChangesEntry*	GetSelection() { return *reinterpret_cast<ContChangesEntry**>(0x11D9EA8); }
	static bool					IsKeyringOpen();
	static void					RestoreScrollPosition() { CdeclCall(0x7800C0); }

	void						ResetInventorySelectionAndHideDataTile() { ThisCall(0x781B10, this); }
};
static_assert(sizeof(InventoryMenu) == 0x124);


typedef TList<ContChangesEntry> BarterItemList;
class BarterMenu : public Menu			// 1053
{
public:
	BarterMenu();
	~BarterMenu();

	enum Buttons
	{
		BM_LeftFilterArrow = 0x2,
		BM_ItemsTitle = 0x3,
		BM_RightFilterArrow = 0x4,
		BM_Container_LeftFilterArrow = 0x7,
		BM_Container_Title = 0x8,
		BM_Container_RightFilterArrow = 0x9,
		BM_ButtonX = 0xD,
		BM_ButtonB = 0x13,
		BM_ListBox = 0x17,
	};


	TileImage* tile028;		// 028
	TileImage* tile02C;		// 02C
	TileImage* tile030;		// 030
	TileText* itemsTitle;	// 034
	TileImage* tile038;		// 038
	TileText* tile03C;		// 03C
	TileImage* tile040;		// 040
	TileImage* tile044;		// 044
	TileText* containerTitle;// 048
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
	UInt32				menuSoundID;	// 0A4
	MenuItemEntryList	leftItems;		// 0A8
	MenuItemEntryList	rightItems;		// 0D8
	MenuItemEntryList* currentItems;	// 108
	BarterItemList		leftBarter;		// 10C
	BarterItemList		rightBarter;	// 114
	UInt32				unk11C;			// 11C

	static BarterMenu* GetSingleton() { return *reinterpret_cast<BarterMenu**>(0x11D8FA4); }
	void __forceinline Refresh(TESForm* form = nullptr) { ThisCall(0x72DC30, form); }

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

	static QuantityMenu* GetSingleton() { return *reinterpret_cast<QuantityMenu**>(0x11DA618); };
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
static_assert(sizeof(HackingText) == 0x2C);

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
static_assert(sizeof(MenuEntry) == 0x78);

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
		Tile* tile;
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
	Sound UIHackingFanHumLP;
	Sound UIHackingPassGood;
	Sound UIHackingPassBad;
	Sound UIHackingCharScroll;
	Sound UIHackingCharEnter;
	UInt32 printedLines_TileChildList[3];
	UInt32 tileChildList174[3];
	TList<char> words;
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

// 0FC
class ComputersMenu : public Menu
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
	TList<void> subMenuBGSTerminals;
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
	bool IsInNote() { return remainingNoteText; };

	static ComputersMenu* GetSingleton() { return *reinterpret_cast<ComputersMenu**>(0x11D9334); };
};
static_assert(sizeof(ComputersMenu) == 0xFC);

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
	TList<BlackJackCard>* dealerHandCards;
	TList<BlackJackCard>* playerHandCards;
	TList<BlackJackCard>* playerSplitHandCards;
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
static_assert(sizeof(BlackJackMenu) == 0xE70);


class LoveTesterMenu : public Menu
{
	struct LoveTesterTextItem;
	struct LoveTesterLightBulbGlows;
	struct LoveTesterPage
	{
		BSSimpleArray<LoveTesterTextItem*> loveTesterTextItemArray;
		BSSimpleArray<LoveTesterLightBulbGlows*> LoveTesterLightBulbGlowsArray;
	};

public:
	enum Pages
	{
		kStart = 0,
		kStrength,
		kPerception,
		kEndurance,
		kCharisma,
		kIntelligence,
		kAgility,
		kLuck,
		kReview
	};

	enum Buttons
	{
		kNextPage = 0x0,
		kPreviousPage = 0x1,
		kIncreaseValue = 0x2,
		kDecreaseValue = 0x3,
		kExitMenu = 0x4,
		kClickable = 0x5,
		kIndexUp = 0x6,
		kIndexDown = 0x7,
	};

	TileImage* tile028;
	TileImage* tile02C;
	TileImage* tile030;
	TileImage* tile034;
	TileImage* tile038;
	TileImage* tile03C;
	TileImage* tile040;
	TileImage* tile044;
	UInt32 currentPageNumber;
	UInt32 finalPageSelectedSpecialIndex; // used with a controller connected
	float time050;
	UInt8 isNotInChangePageAnim; // used to prevent switching pages while the anim plays
	UInt8 pad055;
	UInt8 pad056;
	UInt8 pad057;
	UInt32 totalPointsToAllocate;
	BSFadeNode* NV_VitoMaticVigorTester_Activate;
	BSFadeNode* NV_VitoMaticVigorTester_Cabinet;
	LoveTesterPage unkPage;
	LoveTesterPage vigorTesterPages[9];
	LoveTesterPage unkPage2;
	NiSourceTexture* BBNumbers[11];
	NiSourceTexture* BBMessages[4];
	NiSourceTexture* BBRTOff;
	NiSourceTexture* BBRTOn;
	NiSourceTexture* BBLTOff;
	NiSourceTexture* BBLTOn;
	NiSourceTexture* BBXOff;
	NiSourceTexture* BBXOn;
	NiSourceTexture* BBDecreaseOff;
	NiSourceTexture* BBDecreaseOn;
	NiSourceTexture* BBIncreaseOff;
	NiSourceTexture* BBIncreaseOn;
	NiSourceTexture* FilamentGlowOFF;
	NiSourceTexture* FilamentGlowON;

	static LoveTesterMenu* GetSingleton() { return *reinterpret_cast<LoveTesterMenu**>(0x11DA2C8); };
};
static_assert(sizeof(LoveTesterMenu) == 0x230);


class SPECIALBookMenu : public Menu
{
	struct SpecialBookButton;
	struct SpecialBookTextItem;
	struct SPECIALBookPage
	{
		BSSimpleArray<SpecialBookButton*> buttonArray;
		BSSimpleArray<SpecialBookTextItem*> textArray;
	};

public:
	enum Buttons
	{
		kNextPage = 0x0,
		kPreviousPage = 0x1,
		kIncreaseValue = 0x2,
		kDecreaseValue = 0x3,
		kExitMenu = 0x4,
		kClickable = 0x5,
		kIndexUp = 0x6,
		kIndexDown = 0x7,
	};

	Tile* unk028[8];

	UInt32 currPage;
	UInt32 finalPageSelectedSpecialIndex;
	float time050;
	UInt8 isNotInChangePageAnim;
	UInt8 gap055[3];
	UInt32 numPoints;
	BSFadeNode* Babybook02Nif;
	SPECIALBookPage pages[9];
	NiRefObject* unk180[11];
	NiRefObject* unk1AC[4];
	NiRefObject* unk1BC[2];
	NiRefObject* unk1C4[2];
	NiRefObject* unk1CC[2];
	NiRefObject* unk1D4[2];
	NiRefObject* unk1DC[2];

	static SPECIALBookMenu* GetSingleton() { return *reinterpret_cast<SPECIALBookMenu**>(0x11DAA38); };
};


class RaceSexMenu : public Menu
{
public:
	~RaceSexMenu();
	RaceSexMenu();
	enum
	{
		kFlagHasRandomizePopupBeenShown = 1 << 7,
	};

	enum TemplateIDs
	{
		kRSM_Sex = 0x0,
		kRSM_Race = 0x1,
		kRSM_Face = 0x2,
		kRSM_Hair = 0x3,
		kRSM_Customize = 0x4,
		kRSM_HairStyle = 0x5,
		kRSM_HairColor = 0x6,
		kRSM_FacialHair = 0x7,
		kRSM_EyeColor = 0x8,
		kRSM_Shape = 0x9,
		kRSM_General = 0xA,
		kRSM_Forehead = 0xB,
		kRSM_Brow = 0xC,
		kRSM_Eyes = 0xD,
		kRSM_Nose = 0xE,
		kRSM_Mouth = 0xF,
		kRSM_Cheeks = 0x10,
		kRSM_Jaw = 0x11,
		kRSM_Chin = 0x12,
		kRSM_Tone = 0x13,
		kRSM_14 = 0x14,
		kRSM_Male = 0x15,
		kRSM_Female = 0x16,
		kRSM_Race2 = 0x17,
		kRSM_Preset = 0x18,
		kRSM_Randomize = 0x19,
		kRSM_Age = 0x1A,
		kRSM_HairType = 0x1B,
		kRSM_HeadPart = 0x1C,
		kRSM_HairColor2 = 0x1D,
		kRSM_HairColorR = 0x1E,
		kRSM_HairColorG = 0x1F,
		kRSM_HairColorB = 0x20,
		kRSM_EyeType = 0x21,
		kRSM_22 = 0x22,
		kRSM_23 = 0x23,
	};

	struct Entry
	{
		Tile* tile;
		UInt32 id;
		void* data; // e.g. TESRace for races, an integer for face sliders
	};

	// 24
	struct List
	{
		DList<Entry> items; // 0
		UInt32 parentID; // or previous menu template ID // C
		UInt32 childID; // next menu e.g. sex->race->face->hair // 10
		Tile* listItemTemplate; // 14
		float height; // 18
		bool(__cdecl* IsItemSelected)(Entry* item); // called whenever something is selected to add a white square beside active options // 1C
		Entry* selectedEntry; // 20

		Entry* GetEntryWithStringCI(const char* name)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->data && !_stricmp((char*)iter->data->data, name))
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		void HandleCallbacks() { ThisCall(0x7B40D0, this); };

		Entry* GetEntryWithData(void* data)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->data == data)
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		Entry* GetEntryWithID(UInt32 _id)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->id == _id)
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		Entry* GetEntryByTile(Tile* tile)
		{
			for (auto iter = items.Head(); iter; iter = iter->next)
			{
				if (iter->data->tile == tile)
				{
					return iter->data;
				}
			}

			return nullptr;
		}

		Entry* AddItem(const char* displayString, UInt32 _id, bool removeSound = true)
		{
			auto entry = ThisCall<Entry*>(0x7B3B80, this, displayString, _id);
			if (removeSound)
			{
				entry->tile->SetString(kTileValue_clicksound, nullptr);
			}
			return entry;
		}

		// adds an item without looping over all items in the list to set their positions...
		Entry* AddItemAlt(const char* str, UInt32 _id)
		{
			auto entry = (Entry*)GameHeapAlloc(sizeof(Entry));
			entry->data = nullptr;
			entry->id = _id;

			auto rsm = RaceSexMenu::GetSingleton();
			auto tile = rsm->AddTileFromTemplate(rsm->rsmBackground, "RSM_list_item_template", 0);
			tile->SetString(kTileValue_string, str);
			tile->SetFloat(kTileValue_user0, this->listItemTemplate->GetFloat(kTileValue_user0));
			tile->SetFloat(kTileValue_target, true);
			tile->SetString(kTileValue_clicksound, nullptr);
			entry->tile = tile;

			this->items.Append(entry);

			return entry;
		}

		void RemoveEntry(Entry* entry)
		{
			if (!entry)
			{
				selectedEntry = nullptr;
				return;
			}

			entry->tile->Destroy(false);
			auto nextInList = items.Remove(entry);
			if (selectedEntry == entry) selectedEntry = nextInList ? nextInList->data : nullptr;
		}

		void AddSubMenu(const char* displayString, List* subMenu)
		{
			ThisCall(0x7B3BB0, this, displayString, subMenu);
		}

		void UpdateListPositions()
		{
			// used to update the Y position of the entries in a list (e.g. after sorting the items list) of the menu
			auto iter = items.Head();
			if (!iter) return;

			auto topTrait = Tile::TraitNameToID("_top_bound");
			auto tile = iter->data->tile;
			float yPos = tile->parent->GetFloat(topTrait);

			tile->SetFloat(kTileValue_y, yPos);
			ThisCall(0xA04640, tile, false);

			while (iter = iter->next)
			{
				auto tileAbove = iter->prev->data->tile;
				yPos = tileAbove->GetFloat(kTileValue_y);
				yPos += tileAbove->GetFloat(kTileValue_height);

				tile = iter->data->tile;
				tile->SetFloat(kTileValue_y, yPos);
				ThisCall(0xA04640, tile, false);
			}

			this->height = max(this->height, items.Tail()->data->tile->GetFloat(kTileValue_height));
		}

		void UpdateSelectedSquares()
		{
			ThisCall(0x7B40D0, this);
		}

		void SetActive()
		{
			ThisCall(0x7B4410, this);
		}

		static List* Create(const char* displayString, UInt32 id)
		{
			auto item = (List*)GameHeapAlloc(sizeof(List));
			ThisCall(0x7B39E0, item, id, displayString);
			return item;
		}

		void Destroy(bool doFree = true) { ThisCall(0x7AC700, this, doFree); };
	};
	static_assert(sizeof(List) == 0x24);

	union
	{
		List* menus[20];
		struct
		{
			List* sex;
			List* race;
			List* face;
			List* hair;
			List* customize;
			List* hairStyle;
			List* hairColor;
			List* facialHair;
			List* eyeColor;
			List* shape;
			List* general;
			List* forehead;
			List* brow;
			List* eyes;
			List* nose;
			List* mouth;
			List* cheeks;
			List* jaw;
			List* chin;
			List* tone;
		};
	};

	TileImage* rsmBackground;
	TileImage* rsmFaceGrab;
	TileImage* rsmScrollUpTarget;
	TileImage* rsmScrollDownTarget;
	TileImage* rsmBackButton;
	TileImage* rsmNextButton;
	String nifPath;
	String confirmDone;
	NiPoint3 point0A0;
	NiPoint3 point0AC;
	float lastClickX;
	UInt32 currentSliderValue;
	Tile* activeClickedSliderTile;
	Entry* currentSlider;
	float currentZoom;
	float somethingZoom;
	float remainingZoom;
	NiNode* node0D4;
	TESNPC* npc;
	NiTArray<TESNPC*> presetNPCs;
	UInt32 flags;
	Sound OBJComputerTerminal01LP;
	Sound QSTBirthRoomLP;
	Sound UIFaceGenZoomLP;
	Sound UIFaceGenRotateLP;
	UInt32 finalPageIndex;
	FaceGenData faceData[2][2];

	static RaceSexMenu* GetSingleton() { return *reinterpret_cast<RaceSexMenu**>(0x11DA634); };

	void RefreshSliders();
	void UpdateScrollButtonVisibilities() { ThisCall(0x7AF6B0, this); };
	UInt32 GetCurrentMenuID() { return this->tile->GetFloat(kTileValue_user0); };
	void SetMenuActive(UInt32 _id) { ThisCall(0x7AF180, this, _id); };
	void UpdatePlayerHead() { ThisStdCall(0x007B25A0, this); }
};
static_assert(sizeof(RaceSexMenu) == 0x1A4);


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
		TList<TList<void>> listOfLists04;
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
	TList<TESTopicInfo> infos110;// 110
	TList<TESTopicInfo> infos118;// 118
	TList<TESTopicInfo> infos120;// 120
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
	static DialogMenu* GetSingleton() { return *reinterpret_cast<DialogMenu**>(0x11D9510); };
};
static_assert(sizeof(DialogMenu) == 0x13C);

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
	union
	{
		Tile* tiles[21];
		struct
		{
			TileText* tile028;		// 028	MM_MainRect\MM_HeadlineRect\MM_Headline_LocationInfo
			TileText* tile02C;		// 02C	MM_MainRect\MM_HeadlineRect\MM_Headline_TimeDateInfo
			TileImage* tile030;		// 030	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMap_ParentImage
			TileImage* tileLocalMapCursor;	// 034	MM_MainRect\MM_LocalMap_ClipWindow\MM_LocalMapCursor
			TileImage* tile038;		// 038	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMap_ParentImage
			TileImage* tileWorldMapCursor;	// 03C	MM_MainRect\MM_WorldMap_ClipWindow\MM_WorldMapCursor
			TileRect* mapSelectionRectangle;	// 040	MM_MainRect\MM_Highlight_ClipWindow\MM_MapHighlightBox
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
		};
	};

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
	TList<TESObjectREFR>			mapMarkerList;	// 0D8
	TList<TESObjectREFR>			doorList;		// 0E0
	float							flt0E8;			// 0E8
	float							flt0EC;			// 0EC
	float							flt0F0;			// 0F0
	float							flt0F4;			// 0F4
	TESForm* markerForm;		// 0F8
	NiPoint3						markerPos;		// 0FC
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

	static MapMenu* GetSingleton() { return *reinterpret_cast<MapMenu**>(0x11DA368); };

	void Zoom(bool direction, float amount);

	void Move(float deltaX, float deltaY);

	void MoveDirection(MenuSpecialKeyboardInputCode direction);

	void RefreshWorldMap();
	void RefreshLocalMap();

	// only works for local map doors due to the hook in MapMenu::SetupLocalMap which sets the _MarkerIndex trait!
	TESObjectREFR* GetLocalMapHoveredDoorRef();
	TESObjectREFR* GetHoveredMarkerRef();

	void SetupLinesToMarker(TESObjectREFR* lastExtDoor, TESObjectREFR* ref, QuestTarget* questTarget, signed int tileID);

	bool ScrollNotes(MenuSpecialKeyboardInputCode direction, bool isShiftHeld);
};
static_assert(sizeof(MapMenu) == 0x230);

// 104
class RecipeMenu : public Menu			// 1077
{
public:
	RecipeMenu();
	~RecipeMenu();

	enum Buttons
	{
		RM_Items_LeftFilterArrow = 0x0,
		RM_ItemsTitle = 0x1,
		RM_Items_RightFilterArrow = 0x2,
		RM_Items_InventoryList = 0x3,
		RM_MadeAtVariable = 0x4,
		RM_SkillRequirement = 0x5,
		RM_Items_IngredientList = 0x6,
		RM_ButtonX = 0x7,
		RM_ButtonB = 0x8,
		RM_ItemIcon = 0x9,
		RM_ItemData = 0xA,
		RM_Items_IngredientList2 = 0xB,
		RM_ContainerTitle = 0xC,
		RM_SkillRequirementHeader = 0xD,
	};

	TileImage* tile028;		// 028	RM_Items_LeftFilterArrow
	TileText* itemsTitle;	// 02C	RM_ItemsTitle
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

	static RecipeMenu* GetSingleton() { return *reinterpret_cast<RecipeMenu**>(0x11D8E90); };
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
	float scrolledLoadedSavesHeight;
	UInt32 unk1B0;
	StartMenuOption* brightnessSettingCallback;
	NiSourceTexture* texture1B8;
	UInt32 unk1BC;
	TileImage* tile1C0;
	TileImage* tile1C4;
	UInt32 currentControlsDevice;
	UInt32 unk1CC;
	UInt32 unk1D0;

	static StartMenu* GetSingleton() { return *reinterpret_cast<StartMenu**>(0x11DAAC0); };
};
static_assert(sizeof(StartMenu) == 0x1D4);

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

	static StartMenuOption* Create(const char* str, void (*callback)(void), UInt32 flags);
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

	StartMenuUserOption* Init(char* str, void (*_callback)(StartMenuUserOption*), WhichMenus flags, int _type, int _numOptions, int _buttonWidth, char** optionNames);
	static StartMenuUserOption* Create(char* str, void(__cdecl* callback)(StartMenuUserOption*), WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames);

	void UpdateVisibility() { ThisCall(0x7D4CE0, StartMenu::GetSingleton(), this, NULL); };
};

class NiPointLight;

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
	UInt8					shouldShowHelp;	// 99
	UInt8					pad9A[2];		// 99
	NiControllerSequence* ctrlSeq9C;		// 9C
	NiControllerSequence* ctrlSeqA0;		// A0
	NiControllerManager* ctrlManager;		// A4
	NiControllerSequence* ctrlSeqForward;	// A8
	NiControllerSequence* ctrlSeqBackward;	// AC
	NiControllerSequence* ctrlSeqLeft;		// B0
	NiQuaternion			quaternionB4;	// B4
	NiQuaternion			quaternionC4;	// C4
	NiQuaternion			quaternionD4;	// D4

	static LockPickMenu* GetSingleton() { return *reinterpret_cast<LockPickMenu**>(0x11DA204); };
};
static_assert(sizeof(LockPickMenu) == 0xE4);

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
	virtual void	Update(void);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);
	virtual void	Unk_08(void);
	virtual void	Init(void);
	virtual void	ResetNifs(void);
	virtual void	Unk_0B(void);
	virtual void	OnMenuOpen(void);
	virtual void	OnMenuClose(void);
	virtual void	HandleStaticEffect(float msPassed);
	virtual void	HandleVerticalHoldEffect(float msPassed);
	virtual void	HandleShudderEffect(float msPassed);
	virtual void	Unk_11(float msPassed);
	virtual void	Unk_12(float msPassed);
	virtual void	HandleScanlines(float msPassed);
	virtual void	Unk_14(void);

	NiAVObject* screenTriShape;
	BSFadeNode* node08;
	UInt32 unk0C;
	NiSourceTexture* srcTexture;
	NiNode* node14;
	TileMenu* tileMenu;
	NiPointLight* light1C;
	UInt32 unk20;
	NiCamera* camera;
	UInt8 isInitialised;
	UInt8 gap028[3];
	NiPoint3 pt02C;
	UInt32 unk038;
	UInt32 unk03C;
	float blurRadius;
	float blurIntensity;
	float scanlineFrequency;
	UInt8 byte04C;
	UInt8 isShowStaticEffect;
	UInt8 gap04E[2];
	float staticEffectStartTime;
	float burstDuration;
	float burstIntensity2;
	float burstIntensity;
	UInt8 isShowVerticalHoldEffect;
	UInt8 gap061[3];
	float verticalHoldStartTime;
	float verticalHoldDuration;
	float verticalHoldSpeed;
	float unk070;
	UInt8 isShowShudderHoldEffect;
	UInt8 gap075[3];
	float shudderHoldStartTime;
	float shudderHoldDuration;
	float shudderHoldIntensity;
	float shudderHoldFrequency;
	float unk088;
	float pulseBrightenIntensity;
	float pulseRadiusIntensity;
	UInt8 byte094;
	UInt8 isScanlineEffect;
	UInt8 gap096[2];
	float scanLineStartTime;
	float interval09C;
	UInt32 rate0A0;
	float nextScanlineTime;
	NiPoint4 pt0A8;
	UInt8 isRenderedMenuOrPipboyOpen;
	UInt8 byte0B9;
	UInt8 gap0BA[2];
	UInt32 scanlineTexture;
	UInt32 unk0C0;
	UInt32 unk0C4;
	UInt32 unk0C8;
	UInt32 unk0CC;
	UInt32 unk0D0;
	NiNode* node0D4;
};
static_assert(sizeof(FORenderedMenu) == 0xD8);

class FORenderedMenuRaceSex : public FORenderedMenu
{
public:
	FORenderedMenuRaceSex();
	~FORenderedMenuRaceSex();

	enum RaceMenuType
	{
		kRaceConsole = 0x0,
		kRaceScript = 0x1,
		kBarber = 0x2,
		kPlasticSurgery = 0x3,
	};

	NiNode* unk0D8[4];
	NiNode* buttonLights[4];
	NiNode* unk0F8;
	NiNode* playerNode;
	NiNode* node100;
	NiCamera* camera;
	NiRefObject* PlayerFaceLights01;
	NiNode* playerParentNode;
	NiPoint3 playerNodeTranslate;
	NiMatrix33 playerNodeRotate;
	NiPoint3 bip01Translate;
	UInt8 isPlayerNodeVisible;
	UInt8 gap14D[3];
	UInt32 unk150;
	UInt32 unk154;
	UInt32 unk158;
	UInt32 unk15C;
	UInt32 unk160;
	UInt32 unk164;
	UInt32 unk168;
	RaceMenuType menuType;
};
static_assert(sizeof(FORenderedMenuRaceSex) == 0x170);

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
static_assert(sizeof(FOPipboyManager) == 0x170);

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
		kPerkSelection = 1,
		kCloseMenu = 999  // (any value >= 2 could work)
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
	TList<BGSPerk> availablePerks; // perks to show in the perk listBox

	void SetCurrentPage(Pages newPage) { ThisCall(0x785830, this, newPage); }

	static LevelUpMenu* GetSingleton() { return *reinterpret_cast<LevelUpMenu**>(0x11D9FDC); }
};
static_assert(sizeof(LevelUpMenu) == 0xCC);


struct GradualSetTile
{
	float startValue;
	float endValue;
	UInt32 startTimeMS;
	float durationMS;
	int trait;
	Tile* tile;
	UInt32 type; // 0-4, see 0xA081B5
};
static TList<GradualSetTile>* g_queuedGradualTileFades = (TList<GradualSetTile>*)0x11F3348;

struct SystemColorManager
{
	enum SystemColorNodes
	{
		kNone = 0,
		kHUDMain,
		kHUDAlt,
		kTerminal,
		kPipBoy,
		kStartMenu
	};

	// 0C
	class SystemColor
	{
	public:
		SystemColor();
		~SystemColor();

		virtual void	Destructor(bool doFree);
		virtual UInt32	GetColor();
		virtual void	SetColor(UInt32 newColor);
		virtual bool	IsHard();
		virtual bool	IsSoft();

		String		traitName;

		void SetColorRGB(UInt32 r, UInt32 g, UInt32 b);
	};

	// 10
	class HardSystemColor : public SystemColor
	{
	public:
		HardSystemColor();
		~HardSystemColor();

		UInt32		color;		// 0C
	};

	// 10
	class SoftSystemColor : public SystemColor
	{
	public:
		SoftSystemColor();
		~SoftSystemColor();

		Setting* setting;	// 0C
	};

	DList<SystemColor>	sysColors;
	UInt32				unk0C;

	static SystemColorManager* GetSingleton() { return *reinterpret_cast<SystemColorManager**>(0x11D8A88); };
	int GetColor(SystemColorNodes node) { return ThisCall<int>(0x7190A0, this, node); };
	void SetColor(SystemColorNodes node, UInt32 color) { ThisCall(0x719120, this, node, color); };
};

static BSSimpleArray<StartMenuUserOption*>* g_settingsMenuOptions = (BSSimpleArray<StartMenuUserOption*>*)0x11DAB50;

union Color
{
	UInt32 rgba;
	struct
	{
		UInt8 a, b, g, r;
	};
};

union RGB
{
	UInt32 rgb;
	struct
	{
		UInt8 b, g, r;
	};
};

const UInt32 FontArraySize = 8;

struct FontInfo {
	FontInfo();
	~FontInfo();

	struct Data03C {
		UInt32	unk000;	// 000
		UInt16	wrd004;	// 004	Init'd to 0
		UInt16	wrd006;	// 006	Init'd to 0x0FFFF
	};	// 0008

	struct FontData {
		float	flt000;				// 000
		UInt32	fontTextureCount;	// 004
		UInt32	unk008;
		char	unk00C[8][0x024];	// array of 8 Font Texture Name (expected in Textures\Fonts\*.tex)
	};

	struct TextReplaced {
		String	str000;	// 000	Init'd to ""
		UInt32	unk008;	// 008	Init'd to arg1
		UInt32	unk00C;	// 00C	Init'd to arg2
		UInt32	unk010;	// 010	Init'd to arg3
		UInt32	unk014;	// 014	Init'd to arg4
		UInt32	unk018;	// 018	Init'd to 0
		UInt8	byt01C;	// 01C	Init'd to arg5
		UInt8	fill[3];
	};	// 020

	UInt16						unk000;			// 000	Init'd to 0, loaded successfully in OBSE (word bool ?)
	UInt16						pad002;			// 002
	char* path;			// 004	Init'd to arg2, passed to OpenBSFile
	UInt32						id;				// 008	1 based, up to 8 apparently
	NiObject* unk00C[8];	// 00C	in OBSE: NiTexturingProperty			* textureProperty
	float						unk02C;			// 02C	Those two values seem to be computed by looping through the characters in the font (max height/weight ?)
	float						unk030;			// 030
	UInt32						unk034;			// 038	in OBSE: NiD3DShaderConstantMapEntry	* unk34;
	FontData* fontData;		// 038	Init'd to 0, might be the font content, at Unk004 we have the count of font texture
	Data03C						dat03C;			// 03C
	BSSimpleArray<ButtonIcon>	unk044;			// 044

	static FontInfo* Load(const char* path, UInt32 ID);
	bool GetName(char* out);	// filename stripped of path and extension
};	// 054
static_assert(sizeof(FontInfo) == 0x54);

class FontManager
{
public:
	FontManager();
	~FontManager();

	// 3C


	FontInfo*			fontInfos[8];		// 00
	UInt8				byte20;				// 20
	UInt8				pad21[3];			// 21
	FontInfo*			extraFonts[80];	// 24

	NiPoint3* GetStringDimensions(NiPoint3* outDims, const char* srcString, UInt32 fontID, Float32 wrapwidth = 0x7F7FFFFF, UInt32 startIdx = 0) { return ThisCall<NiPoint3*>(0xA1B020, this, outDims, srcString, fontID, wrapwidth, startIdx); };
	NiPoint3* GetStringDimensions(const char* srcString, UInt32 fontID, Float32 wrapwidth = 0x7F7FFFFF, UInt32 startIdx = 0)
	{
		NiPoint3 out;
		return GetStringDimensions(&out, srcString, fontID, wrapwidth, startIdx);
	}

	static FontManager* GetSingleton() { return *reinterpret_cast<FontManager**>(0x11F33F8); };
};


static_assert(sizeof(FontInfo) == 0x54);

struct __declspec(align(4)) FontTextReplaced
{
	String str;
	UInt32 wrapWidth;
	UInt32 wrapLimit;
	UInt32 initdToZero;
	UInt32 wrapLines;
	UInt32 length;
	UInt8 newLineCharacter;
	UInt8 gap1D[3];
	TList<void> lineWidths;

	FontTextReplaced(): gap1D{}
	{
		str.m_bufLen = 0;
		str.m_data = nullptr;
		str.m_dataLen = 0;
		wrapWidth = 0;
		wrapLimit = 0;
		initdToZero = 0;
		wrapLines = 0;
		length = 0;
		newLineCharacter = 0;
		lineWidths.Init();
	}
	;

	~FontTextReplaced()
	{
		str.Set(NULL);
		lineWidths.RemoveAll();
	}

	void GetVariableEscapedText(const char* input);
};

static_assert(sizeof(FontTextReplaced) == 0x28);
static void(__thiscall* Font__CheckForVariablesInText)(FontInfo*, const char* input, FontTextReplaced* a3) = (void(__thiscall*)(FontInfo*, const char*, FontTextReplaced*))0xA12FB0;


class DebugText
{
public:
	virtual void    Unk_00(void);
	virtual void    Unk_01(UInt32 arg1, UInt32 arg2);
	virtual UInt32  Unk_02(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual UInt32  Unk_03(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4);
	virtual void    Unk_04(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual UInt32  Unk_05(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);
	virtual void    Unk_06(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);
	virtual UInt32  Unk_07(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6, UInt32 arg7);
	virtual UInt32  Unk_08(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);
	virtual UInt32  Unk_09(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5, UInt32 arg6);
	virtual UInt32  Unk_0A(UInt32 arg1);
	virtual void    Unk_0B(UInt32 arg1, UInt32 arg2);

	struct DebugLine
	{
		float           offsetX;    // 00
		float           offsetY;    // 04
		UInt32          alignment;  // 08
		NiNode* node;       // 0C
		String          text;       // 10
		float           flt18;      // 18    Always -1.0
		NiColorAlpha    color;      // 1C
	};

	struct sPrintData
	{
		DebugLine* debugLine;
		UInt32 unk04; // set to 1 in 0xA0F9C2
		UInt32 unk08;
		UInt32 unk0C;
		float duration;
		UInt32 fontNumber;
	};

	struct TextNode
	{
		TextNode* next;		// 00
		TextNode* prev;		// 04
		String text;		// 08
	};

	struct TextList
	{
		TextNode* first;	// 00
		TextNode* last;		// 04
		unsigned int count;	// 08
	};

	enum TextAlign
	{
		kLeftAligned = 0x1,
		kCentered = 0x2,
	};

	DebugLine lines[200];				// 0004
	TextList textList;					// 2264
	BSSimpleArray<sPrintData*> lines2;	// 2270
	unsigned int unk2280;				// 2280
	unsigned int unk2284[3];			// 2284
	unsigned int unk2290[3];			// 2290

	static DebugText* GetSingleton() { return ((DebugText * (*)(bool))0xA0D9E0)(true); }
	void CreateLine(const char* buf, float xPos, float yPos, TextAlign alignment, int a6, float duration, int fontNumber, NiColorAlpha* color) { ThisCall(0xA0F8B0, this, buf, xPos, yPos, alignment, a6, duration, fontNumber, color); }

	DebugText::DebugLine* GetDebugInputLine()
	{
		DebugText::DebugLine* linesPtr = this->lines;
		DebugText::DebugLine* result = linesPtr;
		float maxY = linesPtr->offsetY;
		UInt32 counter = 200;
		do
		{
			linesPtr++;
			if (!linesPtr->text.m_data) break;
			if (maxY < linesPtr->offsetY)
			{
				maxY = linesPtr->offsetY;
				result = linesPtr;
			}
		} while (--counter);
		return result;
	}

	String* GetDebugInput()
	{
		return &(GetDebugInputLine()->text);
	}
};
static_assert(sizeof(DebugText) == 0x229C);


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

struct FontHeightData
{
	float		heightBase;
	float		heightwGap;
};
//s_fontHeightDatas[90];

void MenuButton_DownloadsClick();
void Debug_DumpFontNames(void);