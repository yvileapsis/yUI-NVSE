#pragma once
#include "GameData.h"
#include "nvse/nvse/GameUI.h"
#include <GameObjects.h>
#include <nvse\nvse\GameOSDepend.h>

void (*ToggleMenus)(bool toggleON) = ((void(__cdecl*)(bool))(0x703810));
void (*CreateScreenshot)(char* name) = ((void(__cdecl*)(char*))(0x878860));
void (*RefreshItemListBox)(void) = (void (*)(void))0x704AF0;
void (*TransferContainerItem)(int amount) = (void (*)(int))0x75DC80;
void (__cdecl* ApplyIMOD)(TESImageSpaceModifier* imod, float magnitude, NiNode* node) = (void(__cdecl*)(TESImageSpaceModifier*, float, NiNode*))0x5299A0;
void (*ApplyPerkModifiers)(UInt32 entryPointID, TESObjectREFR* perkOwner, void* arg3, ...) = (void(*)(UInt32, TESObjectREFR*, void*, ...))0x5E58F0;
TESImageSpaceModifier* (*GetGetHitIMOD)(void) = (TESImageSpaceModifier * (*)(void))0x5D2860;
float (*GetItemWeight)(TESForm* baseItem, bool isHardcore) = (float(*)(TESForm*, bool))0x48EBC0;
unsigned int(__cdecl* GetRandomNumberInRange)(int a1, int a2) = (unsigned int(__cdecl*)(int, int))(0x944460);
Tile* (__cdecl* GetMenuTile)(UInt32 id) = (Tile * (__cdecl*)(UInt32))0xA09030;
Menu* (__thiscall* TileList__GetMenu)(Tile*) = (Menu* (__thiscall*)(Tile*))0xA03C90;
void (__thiscall* Menu__RegisterTile)(Menu*, Tile*, bool) = (void (__thiscall*)(Menu*, Tile*, bool))0xA1DC70;
void (__thiscall* Menu__HideTitle)(Menu*, bool) = (void (__thiscall*)(Menu*, bool))0xA1DC20;
float (*GetWeaponDPS)(ActorValueOwner* avOwner, TESObjectWEAP* weapon, float condition, UInt8 arg4, ContChangesEntry* entry, UInt8 arg6, UInt8 arg7, int arg8, float arg9, float arg10, UInt8 arg11, UInt8 arg12, TESForm* ammo) =
(float (*)(ActorValueOwner*, TESObjectWEAP*, float, UInt8, ContChangesEntry*, UInt8, UInt8, int, float, float, UInt8, UInt8, TESForm*))0x645380;
float (*GetWeaponDamage)(TESObjectWEAP* weapon, float condition, ContChangesEntry* entry, TESForm* ammo) = (float (*)(TESObjectWEAP*, float, ContChangesEntry*, TESForm*))0x6450F0;
float(__cdecl* ScaleArmorDTDRByCondition)(UInt16 dtDR, float condition) = (float(__cdecl*)(UInt16, float))0x646360;
void(__cdecl* HandleActorValueChange)(ActorValueOwner* avOwner, int avCode, float oldVal, float newVal, ActorValueOwner* avOwner2) =
(void(__cdecl*)(ActorValueOwner*, int, float, float, ActorValueOwner*))0x66EE50;
void(__cdecl* HUDMainMenu_UpdateVisibilityState)(signed int) = (void(__cdecl*)(signed int))(0x771700);
void (__cdecl *PipBoy_SwitchToMapTab)(char, char) = (void(__cdecl*)(char, char))0x704170;
NiNode* (__cdecl*GetSubNodeByName)(NiAVObject*, char*) = (NiNode*(__cdecl*)(NiAVObject*, char*))(0x4AAE30);
TESForm* (__cdecl* LookupFormByName)(char*) = (TESForm * (__cdecl*)(char*))(0x8D5910);
void* (*NiAllocator)(UInt32 size) = (void* (*)(UInt32))0xAA13E0;
void(__thiscall* NiReleaseThenAdd)(void* address, void* object) = (void(__thiscall*)(void*, void*))(0x66B0D0);
void(__thiscall* HandleStealing)(Actor* thief, TESObjectREFR* target, TESForm* form, int quantity, int value, TESObjectREFR* owner) = (void(__thiscall*)(Actor*, TESObjectREFR*, TESForm*, int, int, TESObjectREFR*))(0x8BFA40);
enum MiscStatCode;
void(__cdecl* IncPCMiscStat)(MiscStatCode code) = (void(__cdecl*)(MiscStatCode))0x4D5C60;
void(__cdecl* DecPCMiscStat)(MiscStatCode code) = (void(__cdecl*)(MiscStatCode))0x60CAD0;

bool(__cdecl* GetIsMenuMode)() = (bool(__cdecl*)())0x702360;

int* (__thiscall* Tile_A01000)(Tile*, enum TileValues) = (int* (__thiscall*)(Tile*, enum TileValues))0xA01000;
void (*EnterTileCriticalSection)() = (void(*)())0xA044F0;
void (*LeaveTileCriticalSection)() = (void(*)())0xA04500;

void(__thiscall* TileValueSetString)(int*, char*, char) = (void(__thiscall*)(int*, char*, char))0xA0A300;

//void(__thiscall* Tile__SetStringValue)(Tile*, enum TileValues, const char*, char) = (void(__thiscall*)(Tile*, enum TileValues, const char*, char))0xA01350;
//void(__thiscall* Tile__SetFloatValue)(Tile*, enum TileValues, float, char) = (void(__thiscall*)(Tile*, enum TileValues, float, char))0xA012D0;

int(__cdecl* strcpy_s_)(char* Dst, rsize_t SizeInBytes, char* Src) = (int(__cdecl*)(char* Dst, rsize_t SizeInBytes, char* Src))0x406D30;


void(__cdecl* MenuButton_Downloads)() = (void(__cdecl*)())0x7D0550;

float* totalUnpausedTime = (float*)0x11F1BF0;
ContChangesEntry** g_modMenuTarget = (ContChangesEntry * *)0x11D9F58;
extern PlayerCharacter* g_thePlayer;
extern DIHookControl* g_DIHook;
bool IsExecutingScript() { return *(bool*)0x11CAEDC; };
bool* isInInitialLoading = (bool*)0x11A0294;
bool* bTrueIronSights = (bool*)0x11D8990;

TESFullName*(__thiscall* TESBoundObject__GetFullName)(TESBoundObject*) = (TESFullName * (__thiscall*)(TESBoundObject*))0x4BE2D0;



struct NiPoint2
{
	float x, y;

	void Scale(float mult)
	{
		x *= mult;
		y *= mult;
	}
};

struct Timer
{
	UInt8 disableCounter;		// 00
	UInt8 gap01[3];				// 01
	float fpsClamp;				// 04
	float fpsClampRemainder;	// 08
	float secondsPassed;		// 0C
	float lastSecondsPassed;	// 10
	UInt32 msPassed;			// 14
	UInt32 unk18;				// 18
	byte isChangeTimeMultSlowly;// 1C
	byte unk1D;					// 1D
	byte unk1E;					// 1E
	byte unk1F;					// 1F
};

struct TimeGlobal : Timer
{
	float unk20;  // 020
	float unk24;  // 024
	float unk28;  // 028
};

class BSGameSound;
class BSWin32AudioListener;
class BSAudioManagerThread;

// 254
class BSSoundInfo
{
public:
	BSSoundInfo();
	~BSSoundInfo();

	UInt32			unk000[72];		// 000
	const char* filePath;		// 120
	UInt32			unk124[76];		// 124
};

// 188
class BSAudioManager
{
public:
	BSAudioManager();
	~BSAudioManager();

	enum Volumes
	{
		kMaster = 0,
		kFoot,
		kVoice,
		kEffects,
		kMusic,
		kRadio,
		kVATSCamera,
	};

	virtual void				Destroy(bool doFree);

	struct AudioManagerStruct
	{
		UInt32 type;
		UInt32 unk04;
		UInt32 unk08;
		AudioManagerStruct* ptr0C;
		UInt32 unk10;
		NiPoint3 pos;
	};

	struct UnkC
	{
		UInt32 ptr00;
		UInt32 unk04;
		UInt32 count08;
	};

	struct Unk24
	{
		UInt32 count00;
		UInt8 isInitialized;
		UInt8 gap04[3];
		AudioManagerStruct* ptr08;
		UInt32 time0C;
	};

	enum AudioFlags
	{
		kAudioFlags_2D = 0x1,
		kAudioFlags_3D = 0x2,
		kAudioFlags_UseMissingVoiceAssetIfMissing = 0x4,
		kAudioFlags8 = 0x8,
		kAudioFlags_Loop = 0x10,
		kAudioFlags20 = 0x20,
		kAudioFlags_RandomFrequencyShift = 0x40,
		kAudioFlags80 = 0x80,
		kAudioFlags100 = 0x100,
		kAudioFlags800 = 0x800,
		kAudioFlagsMaybeUnderwater = 0x2000,
		kAudioFlags4000 = 0x4000,
		kAudioFlags10000 = 0x10000,
		kAudioFlags20000 = 0x20000,
		kAudioFlags40000 = 0x40000,
		kAudioFlags_Radio100000 = 0x100000,
		kAudioFlags200000 = 0x200000,
		kAudioFlags_Radio400000 = 0x400000,
		kAudioFlags_Modulated = 0x80000,
		kAudioFlags1000000 = 0x1000000,
		kAudioFlags_EnvelopeFast = 0x2000000,
		kAudioFlags_EnvelopeSlow = 0x4000000,
		kAudioFlags_2DRadius = 0x8000000,
		kAudioFlags20000000 = 0x20000000,
		kAudioFlags40000000 = 0x40000000,
	};

	UInt32 unk004;
	UInt32 unk008;
	BSAudioManager::UnkC unk00C;
	BSAudioManager::UnkC unk018;
	BSAudioManager::Unk24 unk024;
	BSAudioManager::Unk24 unk034;
	BSAudioManager::Unk24 unk044;
	NiTPointerMap<BSGameSound>	playingSounds;		// 054
	NiTPointerMap<BSSoundInfo>	playingSoundInfos1;	// 064
	NiTPointerMap<BSSoundInfo>	playingSoundInfos2;	// 074
	NiTPointerMap<NiAVObject>	soundPlayingObjects;// 084
	DList<BSGameSound>			gameSounds;			// 094
	UInt32						unk0A0;				// 0A0
	UInt32						unk0A4;				// 0A4
	float						flt0A8;				// 0A8
	_RTL_CRITICAL_SECTION criticalSectionAC;
	_RTL_CRITICAL_SECTION criticalSectionC4;
	_RTL_CRITICAL_SECTION criticalSectionDC;
	_RTL_CRITICAL_SECTION criticalSectionF4;
	_RTL_CRITICAL_SECTION criticalSection10C;
	UInt32 unk124;
	UInt32 unk128;
	UInt32 unk12C;
	UInt32 time130;
	UInt8 byte134;
	UInt8 byte135;
	UInt8 gap136[2];
	UInt32 threadID;
	BSAudioManagerThread* audioMgrThread;	// 13C
	float						volumes[12];		// 140
	//	0	Master
	//	1	Foot
	//	2	Voice
	//	3	Effects
	//	4	Music
	//	5	Radio
	UInt32						unk170[4];			// 170
	UInt32						nextMapKey;			// 180
	UInt8 ignoreTimescale;
	UInt8 byte185;
	UInt8 byte186;
	UInt8 byte187;

	static BSAudioManager* GetSingleton() { return (BSAudioManager*)0x11F6EF0; };
};
STATIC_ASSERT(sizeof(BSAudioManager) == 0x188);


class BSWin32Audio
{
public:
	BSWin32Audio();
	~BSWin32Audio();

	virtual void	Destroy(bool doFree);
	virtual void	Unk_01(void);
	virtual void	Unk_02(void);
	virtual void	Unk_03(void);
	virtual void	Unk_04(void);
	virtual BSGameSound* CreateGameSound(const char* filePath);
	virtual void	Unk_06(void);
	virtual void	Unk_07(void);

	UInt32					unk004[3];		// 004
	BSWin32AudioListener* listener;		// 010
	UInt32					unk014[3];		// 014
	bool					(*sub_82D150)(UInt32*, UInt32*, UInt32*, UInt32*);	// 020
	bool					(*sub_82D280)(UInt32*, UInt32*, UInt32*, UInt32*);	// 024
	bool					(*sub_5E3630)(UInt32*);	// 028
	UInt32(*sub_82D400)(UInt32*, TESSound*, UInt32*);	// 02C
	void					(*sub_832C40)(void);	// 030
	void					(*sub_832C80)(void);	// 034

	static BSWin32Audio* GetSingleton() { return *(BSWin32Audio * *)0x11F6D98; };
};

// 0C
struct Sound
{
	UInt32 unk00;
	UInt8 byte04;
	UInt8 pad05[3];
	UInt32 unk08;

	Sound() : unk00(0xFFFFFFFF), byte04(0), unk08(0) {}


	Sound(const char* soundPath, UInt32 flags)
	{
		ThisCall(0xAD7550, BSWin32Audio::GetSingleton(), this, soundPath, flags);
	}

	void Play()
	{
		ThisCall(0xAD8830, this, 0);
	}

	void PlayDelayed(int delayMS, int unused)
	{
		ThisCall(0xAD8870, this, delayMS, unused);
	}

	Sound& operator= (Sound other)
	{
		unk00 = other.unk00;
		byte04 = other.byte04;
		unk08 = other.unk08;
	}
};

// 018
class ExtraSound : public BSExtraData
{
public:
	ExtraSound();
	~ExtraSound();

	Sound	sound; // 00C
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
	Tile* parentTile;	// 0C
	Tile* selected;		// 10
	Tile* scrollBar;		// 14
	const char* templateName;	// 18
	UInt16			itemCount;		// 1C
	UInt16			pad1E;			// 1E
	float			unk20;			// 20
	float			storedListIndex;// 24
	float			storedCurrentValue;	// 28
	UInt16			unk2C;			// 2C
	UInt16			pad2E;			// 2E

	Item* GetSelected()
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
	}

	void Clear()
	{
		ListNode<ListBoxItem<Item>>* iter = list.Head();
		ListBoxItem<Item>* item;
		do
		{
			item = iter->data;
			if (!item) continue;
			if (item->tile)
				item->tile->Destroy(true);
			GameHeapFree(item);
		} while (iter = iter->next);
		list.RemoveAll();
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
	MenuItemEntryList*	currentItems;	// 0F8
	UInt32				unk0FC;			// 0FC
	Sound				menuSound;		// 100

	static ContainerMenu* GetSingleton() { return *(ContainerMenu**)(0x11D93F8); }
	static ContChangesEntry* GetSelection() { return *(ContChangesEntry **)(0x11D93FC); }
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
		kActionPoints =			0x1,
		kHitPoints =			0x2,
		kRadiationMeter =		0x4,
		kEnemyHealth =			0x8,
		kQuestReminder =		0x10,
		kRegionLocation =		0x20,
		kReticleCenter =		0x40,
		kSneakMeter =			0x80,
		kMessages =				0x100,
		kInfo =					0x200,
		kSubtitles =			0x400,
		kHotkeys =				0x800,
		kXpMeter =				0x1000,
		kBreathMeter =			0x2000,
		kExplosivePositioning = 0x4000,
		kCrippledLimbIndicator = 0x8000,
		kHardcoreMode =			0x10000,
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
	SubtitleData				*currentSubtitle;	// 1A8 
	Sound						radiationSound;		// 1AC
	TESObjectREFR				*crosshairRef;		// 1B8
	UInt32						unk1BC;				// 1BC
	Bitfield32					visibilityOverrides;// 1C0
	UInt32						stage;				// 1C4
	HotKeyWheel					hotkeyWheel;		// 1C8	Hokeys\hotkey_selector
	UInt8					isUsingScope;			// 1FC
	UInt8					byte1FD;				// 1FD
	UInt8					byte1FE;				// 1FE
	UInt8					byte1FF;				// 1FF
	NiControllerSequence	*niContSeq;				// 200
	UInt8					isLevelUpQueued;		// 204
	UInt8					autoDisplayObjectives;	// 205
	UInt8					pad206[2];				// 206
	UInt32					unk208;					// 208
	UInt32					questTargets;			// 20C
	UInt32					unk210;					// 210
	UInt32					compassWidth;			// 214
	UInt32					maxCompassAngle;		// 218
	Actor					*healthTarget;			// 21C
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

	static LoadingMenu* GetSingleton() { return *(LoadingMenu * *)0x11DA0C0; };
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
	static StatsMenu* GetSingleton() { return *(StatsMenu * *)0x11DACE0; };
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
	static CompanionWheelMenu* GetSingleton() { return *(CompanionWheelMenu * *)0x11D92B8; };
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

class Sky;
class BSTempNodeManager;
// 1C
class ImageSpaceModifierInstance : public NiObject
{
public:
	ImageSpaceModifierInstance();
	~ImageSpaceModifierInstance();

	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(UInt32 arg);

	UInt8					hidden;			// 08
	UInt8					pad09[3];		// 09
	float					percent;		// 0C
	NiObject* obj10;			// 10
	float					flt14;			// 14
	UInt32					unk18;			// 18
};

// 30
class ImageSpaceModifierInstanceForm : public ImageSpaceModifierInstance
{
public:
	ImageSpaceModifierInstanceForm();
	~ImageSpaceModifierInstanceForm();

	TESImageSpaceModifier* imageSpace;	// 1C
	void* ptr20;			// 20
	float					flt24;			// 24
	NiObject* obj28;			// 28
	float					flt2C;			// 2C
};

// 30
class ImageSpaceModifierInstanceDOF : public ImageSpaceModifierInstance
{
public:
	ImageSpaceModifierInstanceDOF();
	~ImageSpaceModifierInstanceDOF();

	float					flt1C;			// 1C
	float					flt20;			// 20
	float					flt24;			// 24
	float					flt28;			// 28
	UInt32					unk2C;			// 2C
};

// 44
class ImageSpaceModifierInstanceDRB : public ImageSpaceModifierInstance
{
public:
	ImageSpaceModifierInstanceDRB();
	~ImageSpaceModifierInstanceDRB();

	float					flt1C;			// 1C
	float					flt20;			// 20
	float					flt24;			// 24
	float					flt28;			// 28
	float					flt2C;			// 2C
	UInt32					unk30;			// 30
	UInt32					unk34;			// 34
	float					flt38;			// 38
	float					flt3C;			// 3C
	UInt32					unk40;			// 40
};

// 08
class SkyObject
{
public:
	SkyObject();
	~SkyObject();

	virtual SkyObject* Destroy(bool doFree);
	virtual void		Fn_01(void);
	virtual void		Init(NiNode* niNode);
	virtual void		Update(Sky* sky, float value);

	NiNode* rootNode;	// 04
};

// 1C
class Atmosphere : public SkyObject
{
public:
	Atmosphere();
	~Atmosphere();

	virtual void		InitFog(NiNode* niNode, BSFogProperty* _fogProp);

	NiNode* node08;	// 08
	BSFogProperty* fogProp;	// 0C	Same as *0x11DEB00
	NiRefObject* object10;	// 10
	NiRefObject* object14;	// 14
	UInt8				byte18;		// 18
	UInt8				pad19[3];	// 19
};

// 10
class Stars : public SkyObject
{
public:
	Stars();
	~Stars();

	NiNode* node08;	// 08
	float			flt0C;		// 0C
};

// 2C
class Sun : public SkyObject
{
public:
	Sun();
	~Sun();

	NiBillboardNode* node08;		// 08
	NiBillboardNode* node0C;		// 0C
	NiTriShape* shape10;		// 10
	NiTriShape* shape14;		// 14
	UInt32				unk18;			// 18
	NiDirectionalLight* sunLight;		// 1C	Same as g_TES->directionalLight
	float				flt20;			// 20
	UInt8				byte24;			// 24
	UInt8				byte25;			// 25
	UInt8				byte26;			// 26
	UInt8				byte27;			// 27
	BSShaderAccumulator* shaderAccum;	// 28
};

// 5C
class Clouds : public SkyObject
{
public:
	Clouds();
	~Clouds();

	NiTriStrips* layers[4];		// 08
	NiSourceTexture* textures[4];	// 18
	NiVector3			layerPos[4];	// 28
	UInt16				numLayers;		// 58
	UInt8				forceUpdate;	// 5A
	UInt8				byte5B;			// 5B
};

// 7C
class Moon : public SkyObject
{
public:
	Moon();
	~Moon();

	virtual void	Refresh(NiNode* niNode, const char* moonStr);

	NiNode* node08;			// 08
	NiNode* node0C;			// 0C
	NiTriShape* shape10;			// 10
	NiTriShape* shape14;			// 14
	String			moonTexture[8];		// 18
					//	0	Full Moon
					//	1	Three Wan
					//	2	Half Wan
					//	3	One Wan
					//	4	No Moon
					//	5	One Wax
					//	6	Half Wax
					//	7	Three Wax
	float			flt58;				// 58
	float			flt5C;				// 5C
	float			flt60;				// 60
	float			flt64;				// 64
	float			flt68;				// 68
	UInt32			unk6C;				// 6C
	UInt32			unk70;				// 70
	float			flt74;				// 74
	float			flt78;				// 78
};

// 18
class Precipitation
{
public:
	Precipitation();
	~Precipitation();

	virtual Precipitation* Destroy(bool doFree);

	NiNode* node04;	// 04
	NiNode* node08;	// 08
	UInt32		unk0C;		// 0C
	float		unk10;		// 10
	UInt32		unk14;		// 14
};

// 138
class Sky
{
public:
	Sky();
	~Sky();

	virtual Sky* Destructor(bool doFree);

	struct SkySound
	{
		UInt32		unk00;		// 00
		UInt32		unk04;		// 04
		UInt32		unk08;		// 08
		TESWeather* weather;	// 0C
		UInt32		type;		// 10
		UInt32		soundID;	// 14
	};

	NiNode* rootNode;			// 004
	NiNode* niNode008;			// 008
	TESClimate* currClimate;		// 00C
	TESWeather* currWeather;		// 010
	TESWeather* transWeather;		// 014	Previous weather, gradually fading, on weather transition
	TESWeather* defaultWeather;	// 018	Picked from currClimate weathers list. currClimate is set to this unless there's a regional weather
	TESWeather* overrideWeather;	// 01C
	Atmosphere* atmosphere;		// 020
	Stars* stars;				// 024
	Sun* sun;				// 028
	Clouds* clouds;			// 02C
	Moon* masserMoon;		// 030
	Moon* secundaMoon;		// 034
	Precipitation* precipitation;		// 038
	NiVector3						vector03C;			// 03C
	NiColor							waterReflection;	// 048
	NiVector3						vector054;			// 054
	NiColor							sunAmbient;			// 060
	NiColor							sunDirectional;		// 06C
	NiVector3						vector078;			// 078
	NiVector3						vector084;			// 084
	NiVector3						vector090;			// 090
	NiVector3						vector09C;			// 09C
	NiVector3						vector0A8;			// 0A8
	NiVector3						vector0B4;			// 0B4
	NiColor							sunFog;				// 0C0
	float							windSpeed;			// 0CC
	float							windDirection;		// 0D0
	UInt32							unk0D4[6];			// 0D4
	float							gameHour;			// 0EC
	float							lastUpdateHour;		// 0F0
	float							weatherPercent;		// 0F4
	UInt32							unk0F8;				// 0F8
	tList<SkySound>* skySounds;			// 0FC
	float							lightningFxPerc;	// 100
	UInt32							unk104;				// 104
	float							flt108;				// 108
	float							flt10C;				// 10C
	float							flt110;				// 110
	UInt32							unk114;				// 114
	UInt32							flags;				// 118
	ImageSpaceModifierInstanceForm* currFadeInIMOD;	// 11C
	ImageSpaceModifierInstanceForm* currFadeOutIMOD;	// 120
	ImageSpaceModifierInstanceForm* transFadeInIMOD;	// 124	On weather transition, set to the previuos weather fadeIn/OutIMODs
	ImageSpaceModifierInstanceForm* transFadeOutIMOD;	// 128		"			"
	float							flt12C;				// 12C	Always 12.0
	float							flt130;				// 130	Always 23.99
	float							flt134;				// 134	Always 0

	void Advance(float time) { ThisCall(0x63AC70, this, time); }

	static Sky* GetSingleton() { return *(Sky * *)0x11CCB78; };
};
STATIC_ASSERT(sizeof(Sky) == 0x138);

// 04
class GridArray
{
public:
	GridArray();
	~GridArray();

	virtual void* Destroy(bool doFree);
	virtual void	Fn_01(void);
	virtual void	Fn_02(void);
	virtual void	Fn_03(void);
	virtual bool	Fn_04(UInt32 arg1, UInt32 arg2);
	virtual void	Fn_05(UInt32 arg1, UInt32 arg2);
};

// 28
class GridCellArray : public GridArray
{
public:
	GridCellArray();
	~GridCellArray();

	virtual void	UnloadCellAtGridXY(UInt32 gridX, UInt32 gridY);
	virtual void	SetGridAtXYToNull(UInt32 gridX, UInt32 gridY);
	virtual void	CopyCellAtGridXYTo(UInt32 gridX1, UInt32 gridY1, UInt32 gridX2, UInt32 gridY2);
	virtual void	SwapCellsAtGridXYs(UInt32 gridX1, UInt32 gridY1, UInt32 gridX2, UInt32 gridY2);

	SInt32			worldX;			// 04	X coord of current cell within worldspace
	SInt32			worldY;			// 08	Y coord "
	UInt32			gridSize;		// 0C	Init'd to uGridsToLoad
	TESObjectCELL** gridCells;	// 10	Size is gridSize^2
	float			posX;			// 14	worldX * 4096
	float			posY;			// 18	worldY * 4096
	UInt32			unk1C;			// 1C
	UInt8			byte20;			// 20
	UInt8			pad21[3];		// 21
	UInt32			unk24;			// 24
};

// 44
class LoadedAreaBound : public NiRefObject
{
public:
	LoadedAreaBound();
	~LoadedAreaBound();

	bhkPhantom* phantoms[6];	// 08	Seen bhkAabbPhantom
	TESObjectCELL* cell;			// 20
	NiTMapBase<bhkRigidBody*, UInt32>	boundsMap;		// 24
	float								flt34;			// 34
	float								flt38;			// 38
	float								flt3C;			// 3C
	float								flt40;			// 40
};
STATIC_ASSERT(sizeof(LoadedAreaBound) == 0x44);

// A0
struct WaterSurfaceManager
{
	// 30
	struct WaterGroup
	{
		TESWaterForm* waterForm;		// 00
		UInt32					unk04;			// 04
		UInt32					unk08;			// 08
		float					flt0C;			// 0C	Always 1.0 ?
		float					waterHeight;	// 10
		UInt32					unk14;			// 14
		UInt32					unk18;			// 18
		float					flt1C;			// 1C	-flt0C
		float					flt20;			// 20	-waterHeight
		DList<TESObjectREFR>	waterPlanes;	// 24
	};

	struct Struct8C
	{
		UInt32		unk00;
		UInt32		unk04;
		UInt32		unk08;
	};

	UInt32								unk00;			// 00
	UInt32								unk04;			// 04
	NiObject* object08;		// 08
	NiObject* object0C;		// 0C
	NiObject* object10;		// 10
	NiObject* object14;		// 14
	NiObject* object18;		// 18
	NiObject* object1C;		// 1C	Seen NiSourceTexture
	NiObject* object20;		// 20
	UInt32								unk24;			// 24
	UInt32								unk28;			// 28
	UInt32								unk2C;			// 2C
	UInt32								unk30;			// 30
	UInt32								unk34;			// 34
	UInt32								unk38;			// 38
	DList<WaterGroup>					waterGroups;	// 3C
	WaterGroup* waterLOD;		// 48	(Assumed)
	NiTPointerMap<TESObjectREFR>		map4C;			// 4C
	NiTPointerMap<TESObjectREFR>		map5C;			// 5C
	NiTPointerMap<TESWaterForm>			map6C;			// 6C
	NiTMapBase<TESObjectREFR*, void*>	map7C;			// 7C
	Struct8C							unk8C;			// 8C
	float								flt98;			// 98
	UInt32								unk9C;			// 9C
};
STATIC_ASSERT(sizeof(WaterSurfaceManager) == 0xA0);

// C4
class TES
{
public:
	TES();
	~TES();

	virtual void Fn_00(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, UInt32 arg5);

	UInt32								unk04;				// 04
	GridCellArray* gridCellArray;		// 08
	NiNode* niNode0C;			// 0C
	NiNode* niNode10;			// 10
	NiNode* niNode14;			// 14
	BSTempNodeManager* tempNodeMgr;		// 18
	NiDirectionalLight* directionalLight;	// 1C
	void* ptr20;				// 20
	SInt32								extGridX;			// 24
	SInt32								extGridY;			// 28
	SInt32								extCoordX;			// 2C
	SInt32								extCoordY;			// 30
	TESObjectCELL* currentInterior;	// 34
	TESObjectCELL** interiorsBuffer;	// 38
	TESObjectCELL** exteriorsBuffer;	// 3C
	UInt32								unk40[9];			// 40
	WaterSurfaceManager* waterManager;		// 64
	Sky* sky;				// 68
	tList<ImageSpaceModifierInstance>	activeIMODs;		// 6C
	UInt32								unk74[3];			// 74
	float								flt80;				// 80	Abs X distance from centre of grid.
	float								flt84;				// 84	Abs Y distance from centre of grid.
	TESWorldSpace* currentWrldspc;	// 88
	tList<UInt32>						list8C;				// 8C
	tList<UInt32>						list94;				// 94
	tList<UInt32>						list9C;				// 9C
	QueuedFile* unkA4;				// A4
	NiSourceTexture* unkA8;				// A8
	QueuedFile* unkAC;				// AC
	void* ptrB0;				// B0
	UInt32								unkB4[2];			// B4
	NavMeshInfoMap* navMeshInfoMap;	// BC
	LoadedAreaBound* areaBound;			// C0
	
	static TES* GetSingleton() { return *(TES**)0x11DEA10; }
};
STATIC_ASSERT(sizeof(TES) == 0xC4);


enum ActorValueCode
{
	kAVCode_Aggression,
	kAVCode_Confidence,
	kAVCode_Energy,
	kAVCode_Responsibility,
	kAVCode_Mood,
	kAVCode_Strength,
	kAVCode_Perception,
	kAVCode_Endurance,
	kAVCode_Charisma,
	kAVCode_Intelligence,
	kAVCode_Agility,
	kAVCode_Luck,
	kAVCode_ActionPoints,
	kAVCode_CarryWeight,
	kAVCode_CritChance,
	kAVCode_HealRate,
	kAVCode_Health,
	kAVCode_MeleeDamage,
	kAVCode_DamageResist,
	kAVCode_PoisonResist,
	kAVCode_RadResist,
	kAVCode_SpeedMult,
	kAVCode_Fatigue,
	kAVCode_Karma,
	kAVCode_XP,
	kAVCode_PerceptionCondition,
	kAVCode_EnduranceCondition,
	kAVCode_LeftAttackCondition,
	kAVCode_RightAttackCondition,
	kAVCode_LeftMobilityCondition,
	kAVCode_RightMobilityCondition,
	kAVCode_BrainCondition,
	kAVCode_Barter,
	kAVCode_BigGuns,
	kAVCode_EnergyWeapons,
	kAVCode_Explosives,
	kAVCode_Lockpick,
	kAVCode_Medicine,
	kAVCode_MeleeWeapons,
	kAVCode_Repair,
	kAVCode_Science,
	kAVCode_Guns,
	kAVCode_Sneak,
	kAVCode_Speech,
	kAVCode_Survival,
	kAVCode_Unarmed,
	kAVCode_InventoryWeight,
	kAVCode_Paralysis,
	kAVCode_Invisibility,
	kAVCode_Chameleon,
	kAVCode_NightEye,
	kAVCode_Turbo,
	kAVCode_FireResist,
	kAVCode_WaterBreathing,
	kAVCode_RadiationRads,
	kAVCode_BloodyMess,
	kAVCode_UnarmedDamage,
	kAVCode_Assistance,
	kAVCode_ElectricResist,
	kAVCode_FrostResist,
	kAVCode_EnergyResist,
	kAVCode_EmpResist,
	kAVCode_Variable01,
	kAVCode_Variable02,
	kAVCode_Variable03,
	kAVCode_Variable04,
	kAVCode_Variable05,
	kAVCode_Variable06,
	kAVCode_Variable07,
	kAVCode_Variable08,
	kAVCode_Variable09,
	kAVCode_Variable10,
	kAVCode_IgnoreCrippledLimbs,
	kAVCode_Dehydration,
	kAVCode_Hunger,
	kAVCode_SleepDeprivation,
	kAVCode_DamageThreshold,
	kAVCode_Max
};

enum PerkEntryPointID
{
	kPerkEntry_CalculateWeaponDamage,
	kPerkEntry_CalculateMyCriticalHitChance,
	kPerkEntry_CalculateMyCriticalHitDamage,
	kPerkEntry_CalculateWeaponAttackAPCost,
	kPerkEntry_CalculateMineExplodeChance,
	kPerkEntry_AdjustRangePenalty,
	kPerkEntry_AdjustLimbDamage,
	kPerkEntry_CalculateWeaponRange,
	kPerkEntry_CalculateToHitChance,
	kPerkEntry_AdjustExperiencePoints,
	kPerkEntry_AdjustGainedSkillPoints,
	kPerkEntry_AdjustBookSkillPoints,
	kPerkEntry_ModifyRecoveredHealth,
	kPerkEntry_CalculateInventoryAPCost,
	kPerkEntry_GetDisposition,
	kPerkEntry_GetShouldAttack,
	kPerkEntry_GetShouldAssist,
	kPerkEntry_CalculateBuyPrice,
	kPerkEntry_GetBadKarma,
	kPerkEntry_GetGoodKarma,
	kPerkEntry_IgnoreLockedTerminal,
	kPerkEntry_AddLeveledListOnDeath,
	kPerkEntry_GetMaxCarryWeight,
	kPerkEntry_ModifyAddictionChance,
	kPerkEntry_ModifyAddictionDuration,
	kPerkEntry_ModifyPositiveChemDuration,
	kPerkEntry_AdjustDrinkingRadiation,
	kPerkEntry_Activate,
	kPerkEntry_MysteriousStranger,
	kPerkEntry_HasParalyzingPalm,
	kPerkEntry_HackingScienceBonus,
	kPerkEntry_IgnoreRunningDuringDetection,
	kPerkEntry_IgnoreBrokenLock,
	kPerkEntry_HasConcentratedFire,
	kPerkEntry_CalculateGunSpread,
	kPerkEntry_PlayerKillAPReward,
	kPerkEntry_ModifyEnemyCriticalHitChance,
	kPerkEntry_ReloadSpeed,
	kPerkEntry_EquipSpeed,
	kPerkEntry_ActionPointRegen,
	kPerkEntry_ActionPointCost,
	kPerkEntry_MissFortune,
	kPerkEntry_ModifyRunSpeed,
	kPerkEntry_ModifyAttackSpeed,
	kPerkEntry_ModifyRadiationConsumed,
	kPerkEntry_HasPipHacker,
	kPerkEntry_HasMeltdown,
	kPerkEntry_SeeEnemyHealth,
	kPerkEntry_HasJuryRigging,
	kPerkEntry_ModifyThreatRange,
	kPerkEntry_ModifyThread,
	kPerkEntry_HasFastTravelAlways,
	kPerkEntry_KnockdownChance,
	kPerkEntry_ModifyWeaponStrengthReq,
	kPerkEntry_ModifyAimingMoveSpeed,
	kPerkEntry_ModifyLightItems,
	kPerkEntry_ModifyDamageThresholdDefender,
	kPerkEntry_ModifyChanceforAmmoItem,
	kPerkEntry_ModifyDamageThresholdAttacker,
	kPerkEntry_ModifyThrowingVelocity,
	kPerkEntry_ChanceforItemonFire,
	kPerkEntry_HasUnarmedForwardPowerAttack,
	kPerkEntry_HasUnarmedBackPowerAttack,
	kPerkEntry_HasUnarmedCrouchedPowerAttack,
	kPerkEntry_HasUnarmedCounterAttack,
	kPerkEntry_HasUnarmedLeftPowerAttack,
	kPerkEntry_HasUnarmedRightPowerAttack,
	kPerkEntry_VATSHelperChance,
	kPerkEntry_ModifyItemDamage,
	kPerkEntry_HasImprovedDetection,
	kPerkEntry_HasImprovedSpotting,
	kPerkEntry_HasImprovedItemDetection,
	kPerkEntry_AdjustExplosionRadius,
	kPerkEntry_AdjustHeavyWeaponWeight
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

enum MovementFlags
{
	kForward = 0x1,
	kBackward = 0x2,
	kLeft = 0x4,
	kRight = 0x8,
	kTurnLeft = 0x10,
	kTurnRight = 0x20,
	kNonController = 0x40,
	kWalking = 0x100,
	kRunning = 0x200,
	kSneaking = 0x400,
	kSwimming = 0x800,

	kNE = kForward | kRight,
	kSE = kBackward | kRight,
	kSW = kBackward | kLeft,
	kNW = kForward | kLeft,

	kMoving = kForward | kBackward | kLeft | kRight,
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
	kAddr_EquipItem = 0x88C650,
	kAddr_UnequipItem = 0x88C790,
	kAddr_GetItemHealthPerc = 0x4BCDB0,
	kAddr_GetItemValue = 0x4BD400,
	kAddr_ReadXML = 0xA01B00,
	kAddr_TileFromTemplate = 0xA1DDB0,
	kAddr_EvaluateConditions = 0x680C60,
	kAddr_EvaluateCondition = 0x681600,
};


enum KeyState
{
	isHeld = 0x0,
	isPressed = 0x1,
	isDepressed = 0x2,
	isChanged = 0x3,
};

enum ControlCode
{
	Forward = 0x0,
	Backward = 0x1,
	Left = 0x2,
	Right = 0x3,
	Attack = 0x4,
	Activate = 0x5,
	Aim = 0x6,
	ReadyItem = 0x7,
	Crouch = 0x8,
	Run = 0x9,
	AlwaysRun = 0xA,
	AutoMove = 0xB,
	Jump = 0xC,
	TogglePOV = 0xD,
	MenuMode = 0xE,
	Rest = 0xF,
	VATS_ = 0x10,
	Hotkey1 = 0x11,
	AmmoSwap = 0x12,
	Hotkey3 = 0x13,
	Hotkey4 = 0x14,
	Hotkey5 = 0x15,
	Hotkey6 = 0x16,
	Hotkey7 = 0x17,
	Hotkey8 = 0x18,
	QuickSave = 0x19,
	QuickLoad = 0x1A,
	Grab = 0x1B,
	Escape_ = 0x1C,
	Console = 0x1D,
	Screenshot = 0x1E,
	kControlCode_NONE = 255,
};


__declspec(naked) void Actor::EquipItemAlt(ContChangesEntry *itemEntry, bool noUnequip, bool noMessage)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		mov		[ebp-4], ecx
		mov		ecx, [ebp+8]
		mov		eax, [ecx+8]
		test	eax, eax
		jz		done
		mov		dl, [eax+4]
		cmp		dl, kFormType_TESObjectARMO
		jz		countOne
		cmp		dl, kFormType_TESObjectBOOK
		jz		countOne
		cmp		dl, kFormType_AlchemyItem
		jz		countOne
		cmp		dl, kFormType_TESAmmo
		jz		countMax
		cmp		dl, kFormType_TESObjectWEAP
		jnz		done
		cmp		byte ptr [eax+0xF4], 0xA
		jb		countOne
	countMax:
		mov		edx, [ecx+4]
		jmp		doEquip
	countOne:
		mov		edx, 1
	doEquip:
		movzx	ecx, byte ptr [ebp+0x10]
		push	ecx
		movzx	ecx, byte ptr [ebp+0xC]
		push	ecx
		push	1
		mov		ecx, [ebp+8]
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		noXData
		mov		ecx, [ecx]
	noXData:
		push	ecx
		push	edx
		push	eax
		mov		ecx, [ebp-4]
		mov		eax, kAddr_EquipItem
		call	eax
	done:
		mov		esp, ebp
		pop		ebp
		retn	0xC
	}
}

__declspec(naked) ExtraContainerChanges::EntryDataList* TESObjectREFR::GetContainerChangesList()
{
	__asm
	{
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax + 0xC]
		test	eax, eax
		jz		done
		mov		eax, [eax]
		done:
		retn
	}
}

__declspec(naked) ContChangesEntry *ExtraContainerChanges::EntryDataList::FindForItem(TESForm *item)
{
	__asm
	{
		mov		edx, [esp+4]
	listIter:
		mov		eax, [ecx]
		test	eax, eax
		jz		listNext
		cmp		[eax+8], edx
		jz		done
	listNext:
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		listIter
		xor		eax, eax
	done:
		retn	4
	}
}

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

	TileRect			*tile028;		// 028	IM_Headline_PlayerCapsInfo
	TileRect			*tile02C;		// 02C	IM_Headline_PlayerHPInfo
	TileRect			*tile030;		// 030	IM_Headline_PlayerDRInfo
	TileRect			*tile034;		// 034	IM_Headline_PlayerWGInfo
	TileImage			*tile038;		// 038	IM_InventoryList
	TileRect			*tile03C;		// 03C	IM_HotKeyWheel
	TileImage			*tile040;		// 040	IM_EquipButton
	TileImage			*tile044;		// 044	IM_DropButton
	TileImage			*tile048;		// 048	IM_RepairButton
	TileImage			*tile04C;		// 04C	IM_HotkeyButton
	TileImage			*tile050;		// 050	IM_CancelButton
	TileImage			*tile054;		// 054	IM_ItemIcon
	TileRect			*tile058;		// 058	IM_ItemInfoRect
	TileRect			*tile05C;		// 05C	IM_Tabline
	TileRect			*tile060;		// 060	DAMInfo
	TileRect			*tile064;		// 064	DPSInfo
	TileRect			*tile068;		// 068	StrengthReqInfo
	TileRect			*tile06C;		// 06C	DamageResistInfo
	TileRect			*tile070;		// 070	DamageThresholdInfo
	TileImage			*tile074;		// 074	IM_ModButton
	TileImage			*tile078;		// 078	IM_ItemIconBadge
	TileRect			*tile07C;		// 07C	IM_Headline_PlayerDTInfo
	TileText			*tile080;		// 080	IM_StrReq
	UInt32				filter;			// 084
	ScrollPos	tabScrollPositions[6];	// 088, the scroll index for Weapons, Apparel, Aid, Misc, Ammo and the Keyring
	MenuItemEntryList	itemsList;		// 0B8
	HotKeyWheel			hotkeyWheel;	// 0E8
	tList<ContChangesEntry> changedItemsList; // 11C

	static InventoryMenu* GetSingleton() { return *(InventoryMenu * *)0x11D9EA4; };
	static ContChangesEntry* GetSelection() { return *(ContChangesEntry**)0x11D9EA8; };
	bool IsKeyringOpen() const;
};
STATIC_ASSERT(sizeof(InventoryMenu) == 0x124);

bool InventoryMenu::IsKeyringOpen() const
{
	return this->tile->GetValueFloat(*(UInt32*)0x11D9EB8); // Trait_KeyringOpen
}

/* RefreshItemsList calls itemsList->FreeAllTiles() and then allocates the memory for all the tiles again
   instead skip the calls that free/allocate */
void QuickRefreshItemList()
{
	SafeWriteBuf(0x78319C, "\xE8\x2F\xCE\xFF\xFF\xEB\x70", 7); // InventoryMenu, call SaveCurrentTabScrollPosition and skip reallocating tiles
	SafeWrite16(0x75C3F5, 0); // ContainerMenu, nop JE by changing dest
	SafeWrite16(0x75C443, 0x60EB); // ContainerMenu

	SafeWrite16(0x72DC9C, 0); // BarterMenu
	SafeWrite32(0x72DCD3, 0x0004B2E9); // BarterMenu, jump over code recalculating the selected items for trading

	RefreshItemListBox();

	SafeWriteBuf(0x78319C, "\x6A\x00\x6A\x00\x68\x50\x28", 7); // InventoryMenu
	SafeWrite16(0x75C3F5, 0x0182); // ContainerMenu
	SafeWrite16(0x75C443, 0x4D8B); // ContainerMenu

	SafeWrite16(0x72DC9C, 0x01F6);
	SafeWrite32(0x72DCD3, 0x00D1840F); // jump over code recalculating the selected items for trading
}

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

	static BarterMenu* GetSingleton() { return *(BarterMenu * *)0x11D8FA4; }
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
enum
{
	kAddr_TileSetFloat = 0xA012D0,
	kAddr_TileSetString = 0xA01350,
};

__declspec(naked) void Tile::SetFloat(UInt32 id, float fltVal, bool bPropagate)
{
	static const UInt32 procAddr = kAddr_TileSetFloat;
	__asm	jmp		procAddr
}

__declspec(naked) void Tile::SetString(UInt32 id, const char* strVal, bool bPropagate)
{
	static const UInt32 procAddr = kAddr_TileSetString;
	__asm	jmp		procAddr
}

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
		kFailed=3,
		kSuccess=4
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

	static HackingMenu* GetSingleton() { return *(HackingMenu **)(0x11D95B8); }
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


UInt16 Actor::GetLevel()
{
	return ThisCall<UInt16>(0x87F9F0, this);
}

ConsoleManager::TextList* ConsoleManager::TextList::Append(ConsoleManager::TextNode* append)
{
	append->prev = 0;
	append->next = this->first;
	if (this->first)
	{
		this->first->prev = append;
	}
	else
	{
		this->last = append;
	}
	this->first = append;
	++this->count;
	return this;
}


void ConsoleManager::AppendToSentHistory(const char* str)
{
	// create text node
	ConsoleManager::TextNode* textNode = (ConsoleManager::TextNode*)GameHeapAlloc(sizeof(ConsoleManager::TextNode));
	textNode->next = 0;
	textNode->prev = 0;

	textNode->text.m_data = NULL;
	textNode->text.Init(0);
	textNode->text.Set(str);
	
	// append it to the input history
	this->inputHistory.Append(textNode);
}

__declspec(naked) void Actor::EquipItem(TESForm* objType, UInt32 equipCount, ExtraDataList* itemExtraList, UInt32 unk3, bool lockEquip, UInt32 unk5)
{
	static const UInt32 procAddr = kAddr_EquipItem;
	__asm	jmp		procAddr
}

TESObjectWEAP* Actor::GetEquippedWeapon()
{
	if (baseProcess)
	{
		ContChangesEntry* weaponInfo = baseProcess->GetWeaponInfo();
		if (weaponInfo) return (TESObjectWEAP*)weaponInfo->type;
	}
	return NULL;
}

TESAmmo* Actor::GetEquippedAmmo()
{
	if (baseProcess)
	{
		ContChangesEntry* ammoInfo = baseProcess->GetAmmoInfo();
		if (ammoInfo) return (TESAmmo*)ammoInfo->type;
	}
	return NULL;
}

_declspec(naked) bool IsControllerConnected()
{
	_asm
	{
		cmp byte ptr ds : [0x11D8C50] , 1 // isControllerDisabled
		je done
		cmp byte ptr ds : [0x11D8A84] , 0 // isControllerConnected
	done:
		setne al
		ret
	}
}

bool IsControllerConnectedOrMouseNotMoved()
{
	return IsControllerConnected() || !InterfaceManager::GetSingleton()->hasMouseMoved;
}

__declspec(naked) SInt32 TESObjectREFR::GetItemCount(TESForm *form)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0xC
		push	kExtraData_ContainerChanges
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
		test	eax, eax
		jz		done
		mov		[ebp-4], eax
		mov		ecx, [ebp+8]
		cmp		byte ptr [ecx+4], kFormType_BGSListForm
		jz		itemList
		push	ecx
		mov		ecx, eax
		mov		eax, 0x4C8F30
		call	eax
		jmp		done
	itemList:
		mov		dword ptr [ebp-8], 0
		add		ecx, 0x18
	loopHead:
		mov		[ebp-0xC], ecx
		mov		ecx, [ecx]
		test	ecx, ecx
		jz		nullItem
		push	ecx
		mov		ecx, [ebp-4]
		mov		eax, 0x4C8F30
		call	eax
		add		dword ptr [ebp-8], eax
	nullItem:
		mov		ecx, [ebp-0xC]
		mov		ecx, [ecx+4]
		test	ecx, ecx
		jnz		loopHead
		mov		eax, [ebp-8]
	done:
		mov		esp, ebp
		pop		ebp
		retn	4
	}
}

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
	static DialogMenu* GetSingleton() { return *(DialogMenu**) 0x11D9510; };
};
STATIC_ASSERT(sizeof(DialogMenu) == 0x13C);

void nullsub(void) {};

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
	TileImage*						tileWorldMap; // 084
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

	static MapMenu* GetSingleton() { return *(MapMenu * *)0x11DA368; };
	static tList<TESObjectREFR> GetAvailableRadioStations() { return *(tList<TESObjectREFR>*)0x11DD59C; };
	static TESObjectREFR* GetCurrentRadioStation() { return *(TESObjectREFR * *)0x11DD42C; };

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
	TESRecipeCategory*			category;		// 064
	UInt32						menuSoundID;	// 068
	ListBox<TESRecipe>			recipeList;		// 06C
	ListBox<TESRecipe>*			currentItems;	// 09C
	ListBox<RecipeComponent>	componentList;	// 0A0
	ListBox<Condition>			conditionList;	// 0D0
	UInt8 byte100;			// 100
	UInt8 pad101;			// 101
	UInt8 pad102;			// 102
	UInt8 pad103;			// 103

	static RecipeMenu* GetSingleton() { return *(RecipeMenu**)0x11D8E90; };
};

enum
{
	kAddr_TileGetFloat = 0xA011B0,
};

__declspec(naked) float Tile::GetValueFloat(UInt32 id)
{
	static const UInt32 procAddr = kAddr_TileGetFloat;
	__asm	jmp		procAddr
}

bool DialogMenu::IsNPCTalking()
{
	return tile->GetValueFloat(*(UInt32*)0x11D9500); // g_dialogMenu_TraitShowingText
}

bool DIHookControl::IsKeyHeld(UInt32 keycode)
{
	if (keycode >= kMaxMacros) return false;

	return m_keys[keycode].hold;
}

bool WeaponHasNthMod(ContChangesEntry* weaponInfo, TESObjectWEAP* weap, UInt32 mod)
{
	ExtraDataList* xData = weaponInfo->extendData ? weaponInfo->extendData->GetFirstItem() : NULL;
	if (!xData) return 0;
	ExtraWeaponModFlags* xModFlags = GetExtraType((*xData), WeaponModFlags);
	if (!xModFlags) return 0;
	UInt8 modFlags = xModFlags->flags, idx = 3;
	while (idx--) if ((modFlags & (1 << idx)) && (weap->effectMods[idx] == mod)) return 1;
	return 0;
}

/* 
  returns the mod effect bonuses assuming the weapon has the effect
*/
float TESObjectWEAP::GetModBonuses(UInt32 effectID)
{
	float result = 0;
	for (UInt32 idx = 0; idx < 3; idx++)
		if (effectMods[idx] == effectID)
			result += value1Mod[idx];
	return result;
}

int GetWeaponInfoClipSize(ContChangesEntry* weaponInfo)
{
	TESObjectWEAP* weap = (TESObjectWEAP*)weaponInfo->type;
	int maxRounds = weap->clipRounds.clipRounds;

	if (WeaponHasNthMod(weaponInfo, weap, TESObjectWEAP::kWeaponModEffect_IncreaseClipCapacity))
	{
		maxRounds += weap->GetModBonuses(TESObjectWEAP::kWeaponModEffect_IncreaseClipCapacity);
	}
	return maxRounds;
}

bool __fastcall GetPlayerHasFullClip(bool WeaponHasMod, TESObjectWEAP* weap)
{
	ExtraContainerChanges::EntryData* weaponInfo = g_thePlayer->baseProcess->GetWeaponInfo();
	if (!weaponInfo) return true;
	int maxRounds = GetWeaponInfoClipSize(weaponInfo);

	ContChangesEntry* ammoInfo = g_thePlayer->baseProcess->GetAmmoInfo();
	int currentRounds = ammoInfo ? ammoInfo->countDelta : 0;

	return currentRounds == maxRounds;
}

bool __fastcall GetPlayerHasEmptyClip()
{
	ContChangesEntry* ammoInfo = g_thePlayer->baseProcess->GetAmmoInfo();
	int currentRounds = ammoInfo ? ammoInfo->countDelta : 0;
	return currentRounds == 0;
}

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

	static StartMenu* GetSingleton() { return *(StartMenu * *)0x11DAAC0; };
};
STATIC_ASSERT(sizeof(StartMenu) == 0x1D4);

bool IsInStartMenu()
{
	auto menu = StartMenu::GetSingleton();
	return menu && menu->flags & StartMenu::kInStartMenu;
}

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
	char* displayString;
	void (*callback)(void);	// 08
	UInt32 data;

	static StartMenuOption* Create(char* str, void (*callback)(void), UInt32 flags)
	{
		StartMenuOption* option = (StartMenuOption*)GameHeapAlloc(sizeof(StartMenuOption));
		option->vtable = (UInt32*)kVtbl_StartMenuOption;
		option->displayString = str;
		option->callback = callback;
		option->data = flags;

		return option;
	}
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
	void (__cdecl* onSelection)(StartMenuUserOption*);	// 2C

	StartMenuUserOption* Init(char* str, void (*callback)(StartMenuUserOption*), WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
	{
		return CdeclCall<StartMenuUserOption*>(0x7D6350, this, str, callback, flags, type, numOptions, buttonWidth, optionNames);
	}

	static StartMenuUserOption* Create(char* str, void (__cdecl *callback)(StartMenuUserOption*), WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
	{
		StartMenuUserOption* option = (StartMenuUserOption*)GameHeapAlloc(sizeof(StartMenuUserOption));
		return option->Init(str, callback, flags, type, numOptions, buttonWidth, optionNames);
	}

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

	static LockPickMenu* GetSingleton() { return *(LockPickMenu * *)0x11DA204; };
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

class EqualStringMatcher
{
	char* content;
public:
	EqualStringMatcher(char* str)
	{
		content = str;
	}
	bool Accept(char* item) { return strcmpi(item, content) == 0; };
};

struct ActorRefID
{
	UInt32 refID;
	UInt32 timeOfDeath;
};

class EqualRefIDMatcher
{
	UInt32 num;
public:
	EqualRefIDMatcher(UInt32 aNum)
	{
		num = aNum;
	}
	bool Accept(ActorRefID* toMatch) { return toMatch->refID == num; };
};

enum DefaultObject
{
	kStimpak = 0x0,
	kSuperStimpak = 0x1,
	kRadX = 0x2,
	kRadAway = 0x3,
	kMorphine = 0x4,
	kPerkParalysis = 0x5,
	kPlayerFaction = 0x6,
	kMysteriousStrangerNPC = 0x7,
	kMysteriousStrangerFaction = 0x8,
	kDefaultMusic = 0x9,
	kBattleMusic = 0xA,
	kDeathMusic = 0xB,
	kSuccessMusic = 0xC,
	kLevelUpMusic = 0xD,
	kPlayerVoice_Male = 0xE,
	kPlayerVoice_MaleChild = 0xF,
	kPlayerVoice_Female = 0x10,
	kPlayerVoice_FemaleChild = 0x11,
	kEatPackageDefaultFood = 0x12,
	kEveryActorAbility = 0x13,
	kDrugWearsOffImageSpace = 0x14,
	kDoctorsBag = 0x15,
	kMissFortuneNPC = 0x16,
	kMissFortuneFaction = 0x17,
	kMeltdownExplosion = 0x18,
	kUnarmedForwardPA = 0x19,
	kUnarmedBackwardPA = 0x1A,
	kUnarmedLeftPA = 0x1B,
	kUnarmedRightPA = 0x1C,
	kUnarmedCrouchPA = 0x1D,
	kUnarmedCounterPA = 0x1E,
	kSpotterEffect = 0x1F,
	kItemDetectedEffect = 0x20,
	kCateyeMobileEffect_NYI = 0x21,
};

enum FormID
{
	kThePlayer = 0x14,
	kCaucasianVanillaRace = 0x19,
	kNukaColaQuantum = 0x284F9,
	kNukaColaQuartz = 0xE2F6D,
	kNukaColaVictory = 0xE2F6C,
	kIceColdNukaCola = 0x40DCB,
	kNukaBottleDrinks = 0x663AD,
	kPurifiedWater = 0x151A3,
	kSunsetSarsaparillaStarBottleCap = 0x103B1C,
	kVCG04ActivatorScript = 0x1164A4,
	kCGTutorialScript = 0x59C86,
	kVCGTutorialScript = 0x104C01,
	kWorkbench = 0x75005,
	kCampfire = 0x13E3FA,
	kReloadingBench = 0x15361F,
	kPipboyLight640 = 0x2923E,
	kScrapMetal = 0x31944,
	kRoboticsExpert = 0x31DC2,
	kComputerWhiz = 0x31DC4,
	kPowerArmorTraining = 0x58FDF,
	kTestJasonSpeechScript = 0x8D40F,
	kTestMaskScript = 0xD7F48,
	kEasyPete = 0x104C80,
	kVictor = 0x1073E8,

	// assume dead money and OWB are in load order positions 01/03
	kElectricHotplateDeadMoney = 0x01011219, 
	kElectricHotplateOldWorldBlues = 0x0300C7DD,

	kTTW_IntMetroDefault01 = 0x9CFBD,
};

__declspec(naked) bool IsShiftHeld() 
{
	_asm 
	{
		mov ecx, g_DIHook
		cmp byte ptr[ecx + 0x12A], 0    // check left shift (DirectX scancode * 7 + 4)
		jne done
		cmp byte ptr[ecx + 0x17E], 0    // check right shift (DirectX scancode * 7 + 4)

	done:
		setne al
		ret
	}
}

_declspec(naked) bool IsAltHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp byte ptr[edx + 0x18C], 0    // check left alt (DirectX scancode * 7 + 4)
		setne al
		ret
	}
}

_declspec(naked) bool IsTabHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp byte ptr[edx + 0x6D], 0    // check tab (DirectX scancode * 7 + 4)
		setne al
		ret
	}
}

_declspec(naked) bool IsControlHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp	byte ptr[edx + 0xCF], 0
		jne	done
		cmp	byte ptr[edx + 0x44F], 0
	done:
		setne al
		ret
	}
}

bool MapMenuScrollNotes(MenuSpecialKeyboardInputCode direction)
{
	MapMenu* mapMenu = MapMenu::GetSingleton();
	Tile* tile = NULL;

	if (mapMenu->currentTab == MapMenu::MapMenuTabs::kQuests)
	{
		tile = mapMenu->objectiveList.scrollBar;
	}
	else if (mapMenu->currentTab == MapMenu::MapMenuTabs::kMisc)
	{
		tile = ThisCall<Tile*>(0xA03DA0, mapMenu->tile05C, "MM_TextScrollbar");
	}

	if (!tile) return false;

	int deltaScroll = direction == kMenu_UpArrow ? -1 : 1;
	if (IsShiftHeld()) deltaScroll *= 4;

	if (tile->GetValueFloat(kTileValue_visible))
	{
		int currentValueTrait = tile->TraitNameToID("_current_value");
		float currentScroll = tile->GetValueFloat(currentValueTrait);

		currentScroll += deltaScroll;
		tile->SetFloat(currentValueTrait, currentScroll, 0);
			
		if (currentScroll != tile->GetValueFloat(currentValueTrait))
		{
			int setInCodeTrait = tile->TraitNameToID("_SetInCode");
			tile->SetFloat(setInCodeTrait, 1.0, 1);
			tile->SetFloat(setInCodeTrait, 0.0, 1);
		}
	}
	return true;
}

bool StatsMenuNotesScroll(StatsMenu* statsMenu, MenuSpecialKeyboardInputCode direction)
{
	Tile* tile = NULL;

	tile = statsMenu->statusEffListBox.scrollBar;
	if (!tile) return false;

	Tile* lastChild = statsMenu->tile0F8->children.Tail()->data;

	if (ThisCall<bool>(0xA040A0, tile) || lastChild->GetValueFloat(kTileValue_visible))
	{
		int deltaScroll = direction == kMenu_UpArrow ? -1 : 1;
		if (IsShiftHeld()) deltaScroll *= 4;

		if (statsMenu->tile->GetValue(kTileValue_user0))
		{
			tile = lastChild;
		}

		int currentValueTrait = tile->TraitNameToID("_current_value");
		float currentScroll = tile->GetValueFloat(currentValueTrait);

		currentScroll += deltaScroll;
		tile->SetFloat(currentValueTrait, currentScroll, 0);

		if (currentScroll != tile->GetValueFloat(currentValueTrait))
		{
			int setInCodeTrait = tile->TraitNameToID("_SetInCode");
			tile->SetFloat(setInCodeTrait, 1.0, 1);
			tile->SetFloat(setInCodeTrait, 0.0, 1);
		}
	}
	return true;
}


__declspec(naked) void Actor::UnequipItem(TESForm* objType, UInt32 unk1, ExtraDataList* itemExtraList, UInt32 unk3, bool lockUnequip, UInt32 unk5)
{
	static const UInt32 procAddr = kAddr_UnequipItem;
	__asm	jmp		procAddr
}

__declspec(naked) float ExtraContainerChanges::EntryData::GetItemHealthPerc(bool arg1)
{
	static const UInt32 procAddr = kAddr_GetItemHealthPerc;
	__asm	jmp		procAddr
}

__declspec(naked) float ExtraContainerChanges::EntryData::GetValue()
{
	static const UInt32 procAddr = kAddr_GetItemValue;
	__asm jmp procAddr
}

enum QueuedMenuRequest
{
	kContainerMenu = 0x1,
	kHackingMenu = 0x2,
	kComputerMenu = 0x3,
	kDialogMenu = 0x4,
	kRaceSexMenu = 0x5,
	kStartMenu = 0x6,
	kMessageMenu = 0x7,
	kCompanionWheelMenu = 0x8,
	kPlayerNameMenu = 0x9,
};

struct LockData
{
	UInt32 lockLevel;
	TESKey* key;
	UInt8 flags;
	UInt8 pad[3];
	UInt32 unk0C;
	UInt32 unk10;

	bool IsLocked() { return flags & 1; };
};

LockData* TESObjectREFR::GetLockData()
{
	return ThisCall<LockData*>(0x569160, this);
}


class ImageSpaceModifierInstanceForm;
class ImageSpaceModifierInstanceRB;

struct VATSCameraData 
{
	tList<VATSTargetInfo>						targetsList;	// 00
	UInt32							mode;	// 8
	UInt32							unk0C;			// 0C
	BGSCameraShot* camShot;		// 10
	float							flt14;			// 14
	float							flt18;			// 18
	Projectile*						unk1C;			// 1C
	Projectile*						unk20;			// 20
	TESIdleForm*					attackAnim;		// 24
	ImageSpaceModifierInstanceForm* isModInstForm;	// 28
	ImageSpaceModifierInstanceRB* isModInstRB;	// 2C
	UInt32							unk30;			// 30
	NiPointLight*					pointLight;		// 34
	UInt8							byte38;			// 38
	UInt8							pad39[3];		// 39
	UInt32							numKills;		// 3C
	UInt32							unk40;			// 40
	UInt32							unk44;			// 44
};
STATIC_ASSERT(sizeof(VATSCameraData) == 0x48);

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
		NiNode*			node;       // 0C
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
	BSSimpleArray<sPrintData*> lines2;// 2270
	unsigned int unk2280;				// 2280
	unsigned int unk2284[3];			// 2284
	unsigned int unk2290[3];			// 2290

	static DebugText* GetSingleton() {	return ((DebugText * (*)(bool))0xA0D9E0)(true);	}
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

	String* GetDebugInput() {
		return &(GetDebugInputLine()->text);
	}
};
STATIC_ASSERT(sizeof(DebugText) == 0x229C);

UInt8 TESForm::GetOverridingModIdx()
{
	ModInfo* info = mods.GetLastItem();
	return info ? info->modIndex : 0xFF;
}

void NiMatrix33::Copy(NiMatrix33* toCopy)
{
	this->cr[0][0] = toCopy->cr[0][0];
	this->cr[0][1] = toCopy->cr[0][1];
	this->cr[0][2] = toCopy->cr[0][2];
	this->cr[1][0] = toCopy->cr[1][0];
	this->cr[1][1] = toCopy->cr[1][1];
	this->cr[1][2] = toCopy->cr[1][2];
	this->cr[2][0] = toCopy->cr[2][0];
	this->cr[2][1] = toCopy->cr[2][1];
	this->cr[2][2] = toCopy->cr[2][2];
}

float NiMatrix33::Determinant()
{
	return	this->cr[0][0] * (this->cr[1][1] * this->cr[2][2] - this->cr[1][2] * this->cr[2][1]) -
		this->cr[0][1] * (this->cr[1][0] * this->cr[2][2] - this->cr[1][2] * this->cr[2][0]) +
		this->cr[0][2] * (this->cr[1][0] * this->cr[2][1] - this->cr[1][1] * this->cr[2][0]);
}

void NiMatrix33::Inverse(NiMatrix33& in)
{
	float d = this->Determinant();
	this->cr[0][0] = (in.cr[1][1] * in.cr[2][2] - in.cr[1][2] * in.cr[2][1]) / d;
	this->cr[0][1] = (in.cr[0][2] * in.cr[2][1] - in.cr[0][1] * in.cr[2][2]) / d;
	this->cr[0][2] = (in.cr[0][1] * in.cr[1][2] - in.cr[0][2] * in.cr[1][1]) / d;
	this->cr[1][0] = (in.cr[1][2] * in.cr[2][0] - in.cr[1][0] * in.cr[2][2]) / d;
	this->cr[1][1] = (in.cr[0][0] * in.cr[2][2] - in.cr[0][2] * in.cr[2][0]) / d;
	this->cr[1][2] = (in.cr[0][2] * in.cr[1][0] - in.cr[0][0] * in.cr[1][2]) / d;
	this->cr[2][0] = (in.cr[1][0] * in.cr[2][1] - in.cr[1][1] * in.cr[2][0]) / d;
	this->cr[2][1] = (in.cr[0][1] * in.cr[2][0] - in.cr[0][0] * in.cr[2][1]) / d;
	this->cr[2][2] = (in.cr[0][0] * in.cr[1][1] - in.cr[0][1] * in.cr[1][0]) / d;
}


__declspec(naked) void NiMatrix33::RotationMatrix(float rotX, float rotY, float rotZ)
{
	__asm
	{
		push	ecx
		fld		dword ptr [esp+8]
		fsincos
		fstp	dword ptr [esp]
		movd	xmm1, dword ptr [esp]
		fstp	dword ptr [esp]
		movd	xmm0, dword ptr [esp]
		fld		dword ptr [esp+0xC]
		fsincos
		fstp	dword ptr [esp]
		movd	xmm3, dword ptr [esp]
		fstp	dword ptr [esp]
		movd	xmm2, dword ptr [esp]
		fld		dword ptr [esp+0x10]
		fsincos
		fstp	dword ptr [esp]
		movd	xmm5, dword ptr [esp]
		fstp	dword ptr [esp]
		movd	xmm4, dword ptr [esp]
		movss	xmm6, xmm3
		mulss	xmm6, xmm5
		movd	[ecx], xmm6
		movss	xmm6, xmm3
		mulss	xmm6, xmm4
		movd	[ecx+4], xmm6
		xorps	xmm6, xmm6
		subss	xmm6, xmm2
		movd	[ecx+8], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm2
		mulss	xmm6, xmm5
		movss	xmm7, xmm1
		mulss	xmm7, xmm4
		subss	xmm6, xmm7
		movd	[ecx+0xC], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm5
		movss	xmm7, xmm0
		mulss	xmm7, xmm2
		mulss	xmm7, xmm4
		addss	xmm6, xmm7
		movd	[ecx+0x10], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm3
		movd	[ecx+0x14], xmm6
		movss	xmm6, xmm0
		mulss	xmm6, xmm4
		movss	xmm7, xmm1
		mulss	xmm7, xmm2
		mulss	xmm7, xmm5
		addss	xmm6, xmm7
		movd	[ecx+0x18], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm2
		mulss	xmm6, xmm4
		movss	xmm7, xmm0
		mulss	xmm7, xmm5
		subss	xmm6, xmm7
		movd	[ecx+0x1C], xmm6
		movss	xmm6, xmm1
		mulss	xmm6, xmm3
		movd	[ecx+0x20], xmm6
		pop		ecx
		retn	0xC
	}
}

__declspec(naked) void NiMatrix33::Rotate(float rotX, float rotY, float rotZ)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0x4C
		mov		[ebp-4], ecx
		lea		eax, [ebp-0x28]
		movdqu	xmm0, xmmword ptr [ecx]
		movdqu	xmmword ptr [eax], xmm0
		movdqu	xmm0, xmmword ptr [ecx+0x10]
		movdqu	xmmword ptr [eax+0x10], xmm0
		mov		edx, [ecx+0x20]
		mov		[eax+0x20], edx
		push	dword ptr [ebp+0x10]
		push	dword ptr [ebp+0xC]
		push	dword ptr [ebp+8]
		lea		ecx, [ebp-0x4C]
		call	NiMatrix33::RotationMatrix
		lea		eax, [ebp-0x4C]
		push	eax
		lea		eax, [ebp-0x28]
		push	eax
		mov		ecx, [ebp-4]
		call	NiMatrix33::MultiplyMatrices
		mov		esp, ebp
		pop		ebp
		retn	0xC
	}
}

__declspec(naked) void NiMatrix33::MultiplyMatrices(NiMatrix33 &matA, NiMatrix33 &matB)
{
	__asm
	{
		mov		eax, [esp+4]
		mov		edx, [esp+8]
		movd	xmm0, [eax]
		mulss	xmm0, [edx]
		movd	xmm1, [eax+4]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movd	xmm1, [eax+8]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movd	[ecx], xmm0
		movd	xmm0, [eax+0xC]
		mulss	xmm0, [edx]
		movd	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movd	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movd	[ecx+0xC], xmm0
		movd	xmm0, [eax+0x18]
		mulss	xmm0, [edx]
		movd	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0xC]
		addss	xmm0, xmm1
		movd	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x18]
		addss	xmm0, xmm1
		movd	[ecx+0x18], xmm0
		movd	xmm0, [eax]
		mulss	xmm0, [edx+4]
		movd	xmm1, [eax+4]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movd	xmm1, [eax+8]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movd	[ecx+4], xmm0
		movd	xmm0, [eax+0xC]
		mulss	xmm0, [edx+4]
		movd	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movd	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movd	[ecx+0x10], xmm0
		movd	xmm0, [eax+0x18]
		mulss	xmm0, [edx+4]
		movd	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0x10]
		addss	xmm0, xmm1
		movd	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x1C]
		addss	xmm0, xmm1
		movd	[ecx+0x1C], xmm0
		movd	xmm0, [eax]
		mulss	xmm0, [edx+8]
		movd	xmm1, [eax+4]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movd	xmm1, [eax+8]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movd	[ecx+8], xmm0
		movd	xmm0, [eax+0xC]
		mulss	xmm0, [edx+8]
		movd	xmm1, [eax+0x10]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movd	xmm1, [eax+0x14]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movd	[ecx+0x14], xmm0
		movd	xmm0, [eax+0x18]
		mulss	xmm0, [edx+8]
		movd	xmm1, [eax+0x1C]
		mulss	xmm1, [edx+0x14]
		addss	xmm0, xmm1
		movd	xmm1, [eax+0x20]
		mulss	xmm1, [edx+0x20]
		addss	xmm0, xmm1
		movd	[ecx+0x20], xmm0
		retn	8
	}
}

enum KarmaTier
{
	Good = 0x0,
	NeutralKarma = 0x1,
	Evil = 0x2,
	VeryGood = 0x3,
	VeryEvil = 0x4,
};

KarmaTier (__cdecl* GetKarmaTier)(float karma) = (KarmaTier(__cdecl*)(float))0x47E040;


double ModifyLightItems(double itemWeight)
{
	float perkMod = 0;
	ApplyPerkModifiers(kPerkEntry_ModifyLightItems, g_thePlayer, &perkMod);
	if (perkMod > 0.0)
	{
		float fPackRatThreshold = *(float*)0x11C647C;
		if (itemWeight <= fPackRatThreshold)
		{
			float fPackRatModifier = *(float*)0x11C64AC;
			itemWeight = itemWeight * fPackRatModifier;
		}
	}
	return itemWeight;
}

float GetWeaponModdedWeight(TESObjectWEAP* weap, ContChangesEntry* entry)
{
	if (!weap || !entry) return 0.0F;

	float weight = weap->weight.weight;
	if (WeaponHasNthMod(entry, weap, TESObjectWEAP::kWeaponModEffect_DecreaseWeight))
	{
		for (int i = 0; i < 3; ++i)
		{
			if (weap->effectMods[i] == TESObjectWEAP::kWeaponModEffect_DecreaseWeight)
			{
				weight += weap->value1Mod[i];
			}
		}
	}
	return weight;
}

float GetPlayerWeaponWeight(TESObjectWEAP* weap)
{
	auto weaponInfo = g_thePlayer->baseProcess->GetWeaponInfo();

	float weight = GetWeaponModdedWeight(weap, weaponInfo);
	return ModifyLightItems(weight);
}

float TESForm::GetWeight()
{
	TESWeightForm* weightForm = DYNAMIC_CAST(this, TESForm, TESWeightForm);
	if (weightForm)
	{
		return weightForm->weight;
	}
	else {
		TESAmmo* pAmmo = DYNAMIC_CAST(this, TESForm, TESAmmo);
		if (pAmmo) 
		{
			return pAmmo->weight;
		}
	}
	return 0.0F;
}

float TESForm::GetModifiedWeight()
{
	float weight = GetItemWeight(this, g_thePlayer->isHardcore);
	return ModifyLightItems(weight);
}

UInt32 GetRandomIntBelow(int max)
{
	UInt32* rng = StdCall<UInt32*>(0x476C00);
	return ThisCall<UInt32>(0xAA5230, rng, max);
}

struct DetectionData
{
	Actor* actor;			// 00
	UInt8		detectionLevel;	// 04
	UInt8		byte05;			// 05
	UInt8		byte06;			// 06
	UInt8		byte07;			// 07
	SInt32		detectionValue;	// 08
	UInt32		unk0C;			// 0C
	UInt32		unk10;			// 10
	UInt32		unk14;			// 14
	UInt32		unk18;			// 18
	UInt8		byte1C;			// 1C
	UInt8		byte1D;			// 1D
	bool		inLOS;			// 1E
	UInt8		byte1F;			// 1F
};

int Actor::GetDetectionValue(Actor* detected)
{
	if (baseProcess && !baseProcess->processLevel)
	{
		DetectionData* data = baseProcess->GetDetectionData(detected, 0);
		if (data) return data->detectionValue;
	}
	return -100;
}

struct MemoryHeap
{
	void* vtable; // AbstractHeap
	UInt32 unk04[0x835];
	CRITICAL_SECTION criticalSection;
};

struct Heaps
{
	MemoryHeap* defaultHeap;
	MemoryHeap* staticHeap;
	MemoryHeap* fileHeap;
};

struct HeapManager
{
	UInt16 pad00;
	UInt16 unk02;
	UInt32 unk04; // 04
	Heaps* memHeaps; // 08
	static HeapManager* GetSingleton() { return (HeapManager*)0x11F6238; }
};

void DisableScript(Script* script)
{
	if (script)
	{
		script->info.dataLength = 0;
	}
}

_declspec(naked) bool __thiscall Actor::IsDoingAttackAnimation()
{
	_asm
	{
		mov eax, 0x894900
		jmp eax
	}
}

__declspec(naked) Tile* Tile::ReadXML(const char* xmlPath)
{
	static const UInt32 procAddr = kAddr_ReadXML;
	__asm	jmp		procAddr
}

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

	static LoveTesterMenu* GetSingleton() { return *(LoveTesterMenu**)0x11DA2C8; };
};
STATIC_ASSERT(sizeof(LoveTesterMenu) == 0x230);


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

	static SPECIALBookMenu* GetSingleton() { return *(SPECIALBookMenu**)0x11DAA38; };
};

UInt32 TESForm::GetItemValue()
{
	if (typeID == kFormType_AlchemyItem) return ((AlchemyItem*)this)->value;
	TESValueForm* valForm = DYNAMIC_CAST(this, TESForm, TESValueForm);
	return valForm ? valForm->value : 0;
}

float __fastcall GetExtraCount(TESObjectREFR* crosshairRef) 
{
	if (crosshairRef) 
	{
		ExtraCount* pXCount = GetExtraType(crosshairRef->extraDataList, Count);
		if (pXCount) return pXCount->count;
	}
	return 1.0F;
}

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

class Setting;
struct SystemColorManager
{
	enum SystemColorNodes
	{
		kHUDMain = 0x1,
		kHUDAlt = 0x2,
		kTerminal = 0x3,
		kPipBoy = 0x4,
		kStartMenu = 0x5,
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

	static SystemColorManager* GetSingleton() { return *(SystemColorManager * *)0x11D8A88; };
	int GetColor(SystemColorNodes node) { return ThisCall<int>(0x7190A0, this, node); };
	void SetColor(SystemColorNodes node, UInt32 color) { ThisCall(0x719120, this, node, color); };
};

__declspec(naked) void SystemColorManager::SystemColor::SetColorRGB(UInt32 r, UInt32 g, UInt32 b)
{
	__asm
	{
		xor eax, eax
		mov		ah, [esp + 4]
		mov		al, [esp + 8]
		shl		eax, 0x10
		mov		ah, [esp + 0xC]
		mov		al, 0xFF
		push	eax
		mov		eax, [ecx]
		call	dword ptr[eax + 8]
		retn	0xC
	}
}

BSSimpleArray<StartMenuUserOption*>* g_settingsMenuOptions = (BSSimpleArray<StartMenuUserOption*>*)0x11DAB50;

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

class VATSDebugBuffer
{
public:
	~VATSDebugBuffer() {};
	VATSDebugBuffer() {};
	virtual void Func_000() {};
	virtual void Func_001() {};
	virtual void Func_002() {};
	virtual void Func_003() {};
	virtual void Func_004() {};
	virtual void Print(void* a1, void* a2, char* fmt, ...);

};

void VATSDebugBuffer::Print(void* a1, void* a2, char* format, ...)
{
	va_list args;
	va_start(args, format);

	char buf[1000];
	vsprintf(buf, format, args);

	Console_Print("%s", buf);
	va_end(args);
}

enum PromptType
{
	kTake = 0x1,
	kOpen = 0x2,
	kSit = 0x3,
	kActivate = 0x4,
	kSleep = 0x5,
	kRead = 0x6,
	kTalk = 0x7,
	kOpenDoor = 0x8,
	kHorse = 0x9,
	kCrown = 0xA,
	kVampire = 0xB,
	kEquip = 0xC,
	kUnequip = 0xD,
	kDrink = 0xE,
	kEat = 0xF,
	kRecharge = 0x10,
	kBrew = 0x11,
	kApply = 0x12,
	kRepair = 0x13,
};


enum InterfaceManagerHelps
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

class RaceSexMenu : public Menu
{
	~RaceSexMenu();
	RaceSexMenu();
	enum
	{
		kFlagHasRandomizePopupBeenShown = 1 << 7,
	};

	enum
	{
		kSex = 0,
		kRace,
		kFace,
		kHair,
		kCustomize,
		kHairStyle,
		kHairColor,
		kFacialHair,
		kEyeColor,
		kShape,
		kGeneral,
		kForehead,
		kBrow,
		kEyes,
		kNose,
		kMouth,
		kCheeks,
		kJaw,
		kChin,
		kTone,
		kMale = 0x15,
		kFemale,
		kRace2 = 0x17,
		kPreset = 0x18,
		kRandomize = 0x19,
		kAge = 0x1A,
		kHairColor2 = 0x1D,
	};

	struct Struct20
	{
		Tile* tile00;
		UInt32 id;
		UInt32 unk08;
	};

	struct RaceSexMenuTileTemplate
	{
		UInt32 unk00;
		UInt32 unk04;
		UInt32 unk08;
		UInt32 unk0C;
		UInt32 unk10;
		Tile* listItemTemplate;
		float height;
		UInt32 onClickCallback;
		Struct20* unk20;
	};

public:
	RaceSexMenuTileTemplate* sex;
	RaceSexMenuTileTemplate* race;
	RaceSexMenuTileTemplate* face;
	RaceSexMenuTileTemplate* hair;
	RaceSexMenuTileTemplate* customize;
	RaceSexMenuTileTemplate* hairStyle;
	RaceSexMenuTileTemplate* hairColor;
	RaceSexMenuTileTemplate* facialHair;
	RaceSexMenuTileTemplate* eyeColor;
	RaceSexMenuTileTemplate* shape;
	RaceSexMenuTileTemplate* general;
	RaceSexMenuTileTemplate* forehead;
	RaceSexMenuTileTemplate* brow;
	RaceSexMenuTileTemplate* eyes;
	RaceSexMenuTileTemplate* nose;
	RaceSexMenuTileTemplate* mouth;
	RaceSexMenuTileTemplate* cheeks;
	RaceSexMenuTileTemplate* jaw;
	RaceSexMenuTileTemplate* chin;
	RaceSexMenuTileTemplate* tone;
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
	float unk0B8;
	UInt32 unk0BC;
	Tile* someClickedTile;
	Struct20* currentSubMenu;
	float currentZoom;
	float somethingZoom;
	float startingZoom;
	UInt32 unk0D4;
	TESNPC* npc;
	BSSimpleArray<void> array0DC;
	UInt32 flags;
	Sound OBJComputerTerminal01LP;
	Sound QSTBirthRoomLP;
	Sound UIFaceGenZoomLP;
	Sound UIFaceGenRotateLP;
	UInt32 finalPageIndex;
	UInt32 unk0124;
	UInt32 unk0128;
	UInt32 unk012C;
	UInt32 unk0130;
	UInt32 unk0134;
	UInt32 unk0138;
	UInt32 unk013C;
	UInt32 unk0140;
	UInt32 unk0144;
	UInt32 unk0148;
	UInt32 unk014C;
	UInt32 unk0150;
	UInt32 unk0154;
	UInt32 unk0158;
	UInt32 unk015C;
	UInt32 unk0160;
	UInt32 unk0164;
	UInt32 unk0168;
	UInt32 unk016C;
};
STATIC_ASSERT(sizeof(RaceSexMenu) == 0x170);

struct CombatGroup;
class CombatCoverMap;
class CombatThreatMap;
class CombatTargetMap;
typedef NiTMapBase<Actor*, UInt64> ActorMap;

struct CombatMusicTime
{
	float lastUpdateTime, gamesetting;
};

struct CombatManager
{
	void* vtable;
	CombatGroup* combatGroups;
	UInt16 capacity;
	UInt16 firstFreeEntry;
	UInt16 numObjs;
	UInt16 growSize;
	ActorMap* actorMap;
	CombatThreatMap* combatThreatMap;
	CombatCoverMap* combatCoverMap;
	CombatTargetMap* combatTargetMap;
	float musicStartTime;
	CombatMusicTime stopTime;
	CombatMusicTime updateTime;
	UInt32 numCombatGroups;
	NiRefObject* ref38;
};

// C4
class MediaSet : public TESForm
{
	~MediaSet();
	MediaSet();

	struct MediaSetNameAndDB
	{
		String name;
		float decibels;
		float percent;
	};

public:
	TESFullName name;
	UInt32 unk24[3];
	UInt8 byte30[8];
	TESFullName name2;
	UInt32 type; // 'Battle Set', 'Location Set', 'Dungeon Set', 'Incidental Set'
	MediaSetNameAndDB loopBattleDayOuter;
	MediaSetNameAndDB exploreDayMiddle;
	MediaSetNameAndDB suspenseDayInner;
	MediaSetNameAndDB nightOuter;
	MediaSetNameAndDB nightMiddle;
	MediaSetNameAndDB nightInner;
	UInt32 enableFlags;
	float waitTime;
	float loopFadeOut;
	float recoveryTime;
	float nightTimeMax;
	TESSound* sound1;
	TESSound* sound2;
};
STATIC_ASSERT(sizeof(MediaSet) == 0xC4);

// 288
struct PlayingMusic
{
	enum Type
	{
		kPlayMusicCommand = 6,
		kRadio = 7,
		kMainMenu = 8,
		kNone = 9,
	};

	char					track1Path[MAX_PATH];	// 000
	void* ptr104;				// 104
	char					track2Path[MAX_PATH];	// 108
	UInt32					unk20C;					// 20C
	float					flt210;					// 210
	float					flt214;					// 214
	float					flt218;					// 218
	float					track1Volume;			// 21C
	UInt8					track1Flags;			// 220
	UInt8					track2Flags;			// 221
	UInt8					pad222[2];				// 222
	UInt32					track1Type;				// 224
	UInt32					unk228[8];				// 228
	float					flt248;					// 248
	float					flt24C;					// 24C
	float					flt250;					// 250
	float					track2Volume;			// 254
	UInt32					track2Type;				// 258
	UInt32					unk25C[8];				// 25C
	UInt32					track1Active;			// 27C
	UInt32					unk280;					// 280
	MediaLocationController* medLocCtrl;			// 284
};
STATIC_ASSERT(sizeof(PlayingMusic) == 0x288);

UInt32 Actor::GetBuySellServices()
{
	TESActorBase* actorBase = (TESActorBase*)this->baseForm;
	return actorBase->ai.buySellsAndServices;
}

enum BuySellFlags
{
	kWeapons = 0x1,
	kArmor = 0x2,
	kAlcohol = 0x4,
	kBooks = 0x8,
	kFood = 0x10,
	kChems = 0x20,
	kStimpaks = 0x40,
	kMiscellaneous = 0x400,
};

// 18
class BSTempEffect : public NiObject
{
public:
	BSTempEffect();
	~BSTempEffect();

	virtual void	Unk_23(void);
	virtual void	Unk_24(void);
	virtual void	Unk_25(void);
	virtual void	Unk_26(void);
	virtual void	Unk_27(void);
	virtual void	Unk_28(void);
	virtual void	Unk_29(void);
	virtual void	Unk_2A(void);
	virtual void	Unk_2B(void);
	virtual void	Unk_2C(void);
	virtual void	Unk_2D(void);
	virtual void	Unk_2E(void);
	virtual void	Unk_2F(void);
	virtual void	Unk_30(void);

	float			duration;	// 08
	TESObjectCELL* cell;		// 0C
	float			unk10;		// 10
	UInt8			unk14;		// 14
	UInt8			pad15[3];	// 15
};

// 160
struct ProcessManager
{
	UInt32					unk000;				// 000
	NiTArray<MobileObject*>	objects;			// 004
	UInt32					beginOffsets[4];	// 014	0: High, 1: Mid-High, 2: Mid-Low, 3: Low
	UInt32					endOffsets[4];		// 024
	UInt32					unk034[11];			// 034
	tList<BSTempEffect>		tempEffects;		// 060
	UInt32					unk068[6];			// 068
	tList<Actor>			highActors;			// 080
	UInt32					unk088[54];			// 088

	static ProcessManager* GetSingleton() {	return (ProcessManager*)0x11E0E80; };
	int GetTotalDetectionValue(Actor* actor, bool arg2 = false);
};

double Actor::GetHealthPercent()
{
	return ThisCall<double>(0x893590, this);
}

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
	static CaravanMenu* GetSingleton() { return *(CaravanMenu * *)0x11D917C; };
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

	static TraitMenu* GetSingleton() { return *(TraitMenu * *)0x11DAF74; };
};


void InterfaceManager::VATSHighlightData::AddRef(TESObjectREFR* ref)
{
	if (ref->GetNiNode())
	{
		ThisCall(0x800D50, this, ref);
		this->mode = 1;
	}
};

void InterfaceManager::VATSHighlightData::AddRefAndSetFlashing(TESObjectREFR* ref)
{
	if (ref->GetNiNode())
	{
		ThisCall(0x800E50, this, ref);
		this->mode = 1;
	}
};

void InterfaceManager::VATSHighlightData::ResetRefs()
{
	ThisCall(0x800ED0, this);
}

struct FontHeightData
{
	float		heightBase;
	float		heightwGap;
}
s_fontHeightDatas[90];

// 54
struct FontInfo
{
	struct BufferData
	{
		float			lineHeight;		// 0000
		UInt32			unk0004[73];	// 0004
		UInt32			unk0128[458];	// 0128
		float			baseHeight;		// 0850
		float			flt0854;		// 0854
		float			flt0858;		// 0858
	};

	struct ButtonIcon;

	UInt8						isLoaded;	// 00
	UInt8						pad01[3];	// 01
	char						*filePath;	// 04
	UInt8						fontID;		// 08
	UInt8						pad09[3];	// 09
	NiTexturingProperty			*texProp;	// 0C
	UInt32						unk10[7];	// 10
	float						flt2C;		// 2C
	float						flt30;		// 30
	UInt32						unk34;		// 34
	BufferData					*bufferData;// 38
	UInt32						unk3C[2];	// 3C
	BSSimpleArray<ButtonIcon>	arr44;		// 44
};
STATIC_ASSERT(sizeof(FontInfo) == 0x54);

// 164 (24)
class FontManager
{
public:
	FontManager();
	~FontManager();

	FontInfo		*fontInfos[8];		// 00
	UInt8			byte20;				// 20
	UInt8			pad21[3];			// 21
	FontInfo		*extraFonts[80];	// 24

	//	outDims.x := width (pxl); outDims.y := height (pxl); outDims.z := numLines
	NiVector3 *GetStringDimensions(NiVector3 *outDims, const char *srcString, UInt32 fontID, UInt32 maxFlt = 0x7F7FFFFF, UInt32 startIdx = 0);
	static FontManager* GetSingleton() { return *(FontManager * *)0x11F33F8; };
};

struct FontTextReplaced;
void(__thiscall* Font__CheckForVariablesInText)(FontInfo*, const char* input, FontTextReplaced* a3) = (void(__thiscall*)(FontInfo*, const char*, FontTextReplaced*))0xA12FB0;
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

	FontTextReplaced()
	{
		str.m_bufLen = 0;
		str.m_data = 0;
		str.m_dataLen = 0;
		wrapWidth = 0;
		unk00C = 0;
		unk010 = 0;
		unk014 = 0;
		unk018 = 0;
		byte1C = 0;
		list20.Init();
	};

	~FontTextReplaced()
	{
		str.Set(NULL);
		list20.RemoveAll();
	}

	void GetVariableEscapedText(const char* input) { Font__CheckForVariablesInText(FontManager::GetSingleton()->fontInfos[0], input, this); }
};

STATIC_ASSERT(sizeof(FontTextReplaced) == 0x28);

__declspec(naked) Tile* Menu::AddTileFromTemplate(Tile* destTile, const char* templateName, UInt32 arg3)
{
	static const UInt32 procAddr = kAddr_TileFromTemplate;
	__asm	jmp		procAddr
}

class BGSCameraPath : public TESForm 
{
public:
	BGSCameraPath();
	~BGSCameraPath();

	enum Flags
	{
		kFirstPerson = 8,
	};

	tList<void> list18;
	tList<BGSCameraShot> cameraShots;
	UInt8 flags;
	UInt8 gap29[3];
	UInt32 ptr2C;
	BGSCameraPath* parentCameraPath;
	BGSCameraPath* previousSiblingCameraPath;
};
STATIC_ASSERT(sizeof(BGSCameraPath) == 0x38);

float GetPlayerWeaponDamage()
{
	float equippedDamage = 0;

	ContChangesEntry* entry = g_thePlayer->baseProcess->GetWeaponInfo();
	if (entry)
	{
		TESObjectWEAP* weap = (TESObjectWEAP*)entry->type;
		float condition = entry->GetItemHealthPerc() / 100.0F;

		equippedDamage = GetWeaponDamage(weap, condition, entry, NULL);
	}

	return equippedDamage;
}

float GetPlayerWeaponDPS()
{
	float equippedDPS = 0;

	ContChangesEntry* entry = g_thePlayer->baseProcess->GetWeaponInfo();
	if (entry)
	{
		TESObjectWEAP* weap = (TESObjectWEAP*)entry->type;
		float condition = entry->GetItemHealthPerc() / 100.0F;

		equippedDPS = GetWeaponDPS(&g_thePlayer->avOwner, weap, condition, 1, entry, 0, 0, -1, 0.0F, 0.0F, 0, 0, NULL);
	}

	return equippedDPS;
}

struct XINPUT_GAMEPAD_EX
{
	UInt32		eventCount;		// 00
	UInt16		wButtons;		// 04
	UInt8		bLeftTrigger;	// 06
	UInt8		bRightTrigger;	// 07
	SInt16		sThumbLX;		// 08
	SInt16		sThumbLY;		// 0A
	SInt16		sThumbRX;		// 0C
	SInt16		sThumbRY;		// 0E

	static XINPUT_GAMEPAD_EX* GetCurrent() { return (XINPUT_GAMEPAD_EX*)0x11F35A8; };
	static XINPUT_GAMEPAD_EX* GetPrevious() { return (XINPUT_GAMEPAD_EX*)0x11F35B8; };
};

bool __fastcall GetXIControlPressed(UInt32 ctrlID)
{
	XINPUT_GAMEPAD_EX* curr = XINPUT_GAMEPAD_EX::GetCurrent();
	XINPUT_GAMEPAD_EX* prev = XINPUT_GAMEPAD_EX::GetPrevious();

	switch (ctrlID)
	{
	case kXboxCtrl_DPAD_UP:
		return (curr->wButtons & 1) && !(prev->wButtons & 1);
	case kXboxCtrl_DPAD_DOWN:
		return (curr->wButtons & 2) && !(prev->wButtons & 2);
	case kXboxCtrl_DPAD_RIGHT:
		return (curr->wButtons & 8) && !(prev->wButtons & 8);
	case kXboxCtrl_DPAD_LEFT:
		return (curr->wButtons & 4) && !(prev->wButtons & 4);
	case kXboxCtrl_BACK:
		return (curr->wButtons & 0x20) && !(prev->wButtons & 0x20);
	default:
		return false;
	}
}

bool __fastcall GetXIControlHeld(UInt32 ctrlID)
{
	XINPUT_GAMEPAD_EX* curr = XINPUT_GAMEPAD_EX::GetCurrent();

	switch (ctrlID)
	{
	case kXboxCtrl_DPAD_UP:
		return (curr->wButtons & 1);
	case kXboxCtrl_DPAD_DOWN:
		return (curr->wButtons & 2);
	case kXboxCtrl_DPAD_RIGHT:
		return (curr->wButtons & 8);
	case kXboxCtrl_DPAD_LEFT:
		return (curr->wButtons & 4);
	default:
		return false;
	}
}

bool TESObjectWEAP::IsMeleeWeapon()
{
	return (this->eWeaponType <= EWeaponType::kWeapType_TwoHandMelee);
}

void TESObjectREFR::SetScale(float scale)
{
	ThisCall(0x567490, this, scale);
}

__declspec(naked) NiAVObject *NiNode::GetBlock(const char *blockName)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		push	ecx
		push	esi
		mov		esi, ecx
		push	dword ptr [ebp+8]
		mov		eax, 0xA5B690
		call	eax
		pop		ecx
		lea		edx, [ebp-4]
		mov		[edx], eax
		push	edx
		mov		ecx, esi
		mov		eax, [ecx]
		call	dword ptr [eax+0x9C]
		mov		esi, eax
		lea		edx, [ebp-4]
		push	edx
		mov		eax, 0x4381D0
		call	eax
		pop		ecx
		mov		eax, esi
		pop		esi
		mov		esp, ebp
		pop		ebp
		retn	4
	}
}

NiNode *NiNode::GetNode(const char *nodeName)
{
	NiAVObject *found = GetBlock(nodeName);
	return found ? found->GetNiNode() : NULL;
}

NiAVObject* TESObjectREFR::GetNiBlock(const char* blockName)
{
	NiNode* rootNode = GetNiNode();
	return rootNode ? rootNode->GetBlock(blockName) : NULL;
}

enum AnimAction
{
	kNone = 0xFFFFFFFF,
	kEquip_Weapon = 0x0,
	kUnequip_Weapon = 0x1,
	kAttack = 0x2,
	kAttack_Eject = 0x3,
	kAttack_Follow_Through = 0x4,
	kAttack_Throw = 0x5,
	kAttack_Throw_Attach = 0x6,
	kBlock = 0x7,
	kRecoil = 0x8,
	kReload = 0x9,
	kStagger = 0xA,
	kDodge = 0xB,
	kWait_For_Lower_Body_Anim = 0xC,
	kWait_For_Special_Idle = 0xD,
	kForce_Script_Anim = 0xE,
	kReloadLoopStart = 0xF,
	kReloadLoopEnd = 0x10,
	kReloadLoop = 0x11,
};

enum KnockState
{
	kNormal = 0x0,
	kExplode = 0x1,
	kExplodeLeadIn = 0x2,
	kKnockedOut = 0x3,
	kKnockOutLeadIn = 0x4,
	kQueued = 0x5,
	kGettingUp = 0x6,
};

struct MovementParameters
{
	float deltaTime;
	NiPoint3 rotation;
	NiPoint3 velocity;
	UInt32 moveFlags;
	float unkSpeed;
	UInt8 byte24;
};

enum LifeStates
{
	kAlive = 0x0,
	kDying = 0x1,
	kDead = 0x2,
	kUnconscious = 0x3,
	kReanimate = 0x4,
	kRestrained = 0x5,
	kEssentialUnconscious = 0x6,
};

EquipData FindEquipped(TESObjectREFR* thisObj, FormMatcher& matcher)
{
	ExtraContainerChanges* xChanges = GetExtraType(thisObj->extraDataList, ContainerChanges);
	return xChanges ? xChanges->FindEquipped(matcher) : EquipData();
}

class MatchBySlot : public FormMatcher
{
	UInt32	m_slotMask;

public:
	MatchBySlot(UInt32 slot) : m_slotMask(TESBipedModelForm::MaskForSlot(slot)) {}

	bool Matches(TESForm* form) const
	{
		UInt32 formMask = 0;
		if (form)
		{
			if IS_TYPE(form, TESObjectWEAP)
				formMask = TESBipedModelForm::eSlot_Weapon;
			else if IS_TYPE(form, TESObjectARMO)
				formMask = ((TESObjectARMO*)form)->bipedModel.partMask;
		}
		return (formMask & m_slotMask) != 0;
	}
};

bool Actor::IsWearingPowerArmorTorso()
{
	MatchBySlot matcher(TESBipedModelForm::ePart_UpperBody);
	EquipData equipD = FindEquipped(this, matcher);
	TESForm* pFound = equipD.pForm;
	pFound = pFound->TryGetREFRParent();
	TESBipedModelForm* pBiped = DYNAMIC_CAST(pFound, TESForm, TESBipedModelForm);
	if (pBiped)
	{
		return (pBiped->IsPowerArmor());
	}
	return false;
}

bool Actor::IsWearingPowerArmorHelmet()
{
	MatchBySlot matcher(TESBipedModelForm::ePart_Hat);
	EquipData equipD = FindEquipped(this, matcher);
	TESForm* pFound = equipD.pForm;
	pFound = pFound->TryGetREFRParent();
	TESBipedModelForm* pBiped = DYNAMIC_CAST(pFound, TESForm, TESBipedModelForm);
	if (pBiped)
	{
		return (pBiped->IsPowerArmor());
	}
	return false;
}

void PlayGameSound(const char* soundPath)
{
	Sound sound(soundPath, 0x121);
	sound.Play();
}

bool ReloadCurrentSave()
{
	return ThisCall<bool>(0x8512F0, SaveGameManager::GetSingleton());
}

/* creates a save game, passing NULL as name generate a name based on player location */
bool SaveGame(const char* name)
{
	return ThisCall<bool>(0x8503B0, SaveGameManager::GetSingleton(), name, -1, 0);
}

void CreateNamedSaveAndCloseStartMenu()
{
	SaveGame(NULL);
	((void (*)(void))0x7D0440)(); // MenuButton_Continue
}

void QueueAutoSave(byte delay)
{
	SaveGameManager* mgr = SaveGameManager::GetSingleton();
	mgr->autoSaveTimer = delay;
}

// 1C
struct DetectionEvent
{
	UInt32			soundLevel;		// 00
	float			posX;			// 04
	float			posY;			// 08
	float			posZ;			// 0C
	float			timeStamp;		// 10
	UInt32			unk14;			// 14
	TESObjectREFR* locationRef;	// 18
};

// 20
struct ProjectileData
{
	UInt8				byte00;			// 00
	UInt8				byte01;			// 01
	UInt8				byte02;			// 02
	UInt8				byte03;			// 03
	float				flt04;			// 04
	float				flashDuration;	// 08
	NiNode* muzzleFlash;	// 0C
	NiPointLight* flashLight;	// 10
	BGSProjectile* baseProj;		// 14
	TESObjectWEAP* sourceWeap;	// 18
	TESObjectREFR* refr;			// 1C
};

// 8
struct TimePair
{
	float startTime;
	float cooldownTime;
};

// 10
struct PosAndCell
{
	NiPoint3 pos;
	TESObjectCELL* cell;
};

// 68
struct CombatTarget
{
	Actor* target;
	UInt32 unk04;
	UInt32 unk08;
	UInt32 unk0C;
	UInt32 unk10;
	UInt32 unk14;
	PosAndCell posAndCell;
	UInt32 unk28;
	UInt32 unk2C;
	UInt32 unk30;
	UInt32 unk34;
	UInt32 unk38;
	UInt32 unk3C;
	UInt32 unk40;
	UInt32 unk44;
	UInt32 unk48;
	UInt32 unk4C;
	TimePair unk50;
	UInt32 unk58;
	UInt32 unk5C;
	UInt32 unk60;
	UInt32 unk64;
};

class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;

// 46C
class HighProcess : public MiddleHighProcess
{
public:
	HighProcess();
	~HighProcess();

	enum
	{
		kAnimAction_None = -1,
		kAnimAction_Equip_Weapon,
		kAnimAction_Unequip_Weapon,
		kAnimAction_Attack,
		kAnimAction_Attack_Follow_Through,
		kAnimAction_Attack_Latency,
		kAnimAction_Attack_Throw_Attach,
		kAnimAction_Attack_Throw_Release,
		kAnimAction_Block,
		kAnimAction_Stagger,
		kAnimAction_Reload,
		kAnimAction_Dodge,
		kAnimAction_Wait_For_Lower_Body_Anim,
		kAnimAction_Wait_For_Special_Idle,
		kAnimAction_Force_Script_Anim
	};

	enum
	{
		kSitSleepState_Normal = 0x0,
		kSitSleepState_LoadSitIdle = 0x1,
		kSitSleepState_WantToSit = 0x2,
		kSitSleepState_WaitingForSitAnim = 0x3,
		kSitSleepState_Sitting = 0x4,
		kSitSleepState_WantToStand = 0x5,
		kSitSleepState_LoadingSleepIdle = 0x6,
		kSitSleepState_WantToSleep = 0x7,
		kSitSleepState_WaitingForSleepAnim = 0x8,
		kSitSleepState_Sleeping = 0x9,
		kSitSleepState_WantToWake = 0xA,
	};

	struct ActorValueArray
	{
		struct Value
		{
			UInt8 isModified;
			UInt8 gap01[3];
			float value;
		};

		ActorValueArray::Value avs[77];
	};

	tList<DetectionData>* detectedActors;	// 25C
	tList<DetectionData>* detectingActors;	// 260
	void* ptr264;			// 264
	void* ptr268;			// 268
	void* ptr26C;			// 26C
	UInt32								unk270;				// 270
	tList<CombatTarget>					list274;			// 274
	tList<void>							list27C;			// 27C
	tList<void>							list284;			// 284
	tList<void>							list28C;			// 28C
	float								flt294;				// 294
	float								flt298;				// 298
	UInt32								unk29C;				// 29C
	float								flt2A0;				// 2A0
	UInt32								unk2A4;				// 2A4
	float								flt2A8;				// 2A8
	UInt32								unk2AC;				// 2AC
	float								alpha;				// 2B0
	float								flt2B4;				// 2B4
	float								flt2B8;				// 2B8
	float								flt2BC;				// 2BC
	UInt32								unk2C0;				// 2C0
	UInt8								byte2C4;			// 2C4
	UInt8								byte2C5;			// 2C5
	UInt8								byte2C6;			// 2C6
	UInt8								byte2C7;			// 2C7
	float								flt2C8;				// 2C8
	UInt32								unk2CC;				// 2CC
	float								flt2D0;				// 2D0
	float								flt2D4;				// 2D4
	float								flt2D8;				// 2D8
	UInt32								unk2DC;				// 2DC
	float								flt2E0;				// 2E0
	NiBSBoneLODController* ptr2E4;			// 2E4
	UInt32								unk2E8;				// 2E8
	SInt16								currentAction;		// 2EC
	UInt8								pad2EE[2];			// 2EE
	BSAnimGroupSequence* currentSequence;	// 2F0
	UInt8 forceFireWeapon;
	UInt8 gap2F5[3];
	float								flt2F8;				// 2F8
	UInt32								unk2FC[5];			// 2FC
	float								flt310;				// 310
	UInt32								unk314[7];			// 314
	float								flt330;				// 330
	float								flt334;				// 334
	float								flt338;				// 338
	float								diveBreath;			// 33C
	UInt32								unk340;				// 340
	float								flt344;				// 344
	UInt32								unk348;				// 348
	float								flt34C;				// 34C
	TESIdleForm* idleForm350;		// 350
	UInt32								unk354[4];			// 354
	NiBSplineCompTransformInterpolator** ptr364;			// 364
	UInt32 unk368;
	UInt32 unk36C;
	UInt32 unk370;
	UInt8 isContinuingPackagePCNear;
	UInt8 gap375[3];
	float								flt378;				// 378
	float								flt37C;				// 37C
	UInt32								unk380;				// 380
	float								flt384;				// 384
	float								flt388;				// 388
	tList<void>							list38C;			// 38C
	tList<void>							list394;			// 394
	UInt32								unk39C;				// 39C
	UInt32								unk3A0;				// 3A0
	float								flt3A4;				// 3A4
	UInt32								unk3A8[5];			// 3A8
	float								flt3BC;				// 3BC
	float								flt3C0;				// 3C0
	float								lightAmount;		// 3C4
	float								flt3C8;				// 3C8
	UInt32								unk3CC;				// 3CC
	UInt32								unk3D0;				// 3D0
	ProjectileData* projData;			// 3D4
	UInt32								unk3D8;				// 3D8
	DetectionEvent* detectionEvent;	// 3DC
	UInt32								unk3E0;				// 3E0
	UInt32								unk3E4;				// 3E4
	UInt32								fadeType;			// 3E8
	float								delayTime;			// 3EC
	UInt32								unk3F0;				// 3F0
	UInt32								unk3F4;				// 3F4
	UInt32								unk3F8[3];			// 3F8
	Actor* combatTarget;		// 404
	UInt32								unk408[4];			// 408
	float								flt418;				// 418
	TESObjectREFR* packageTarget;		// 41C
	UInt32								unk420;				// 420
	UInt32								queuedIdleFlags;	// 424
	UInt32								unk428;				// 428
	float								flt42C;				// 42C
	UInt32								unk430;				// 430
	bhkShapePhantom* ptr434;			// 434
	UInt32								unk438;				// 438
	float								unk43C;				// 43C
	float								radsSec440;			// 440
	UInt8								plantedExplosive;	// 444
	UInt8								pad445[3];			// 445
	float								flt448;				// 448
	UInt32								unk44C;				// 44C
	float								flt450;				// 450
	UInt32								unk454[6];			// 454
};
STATIC_ASSERT(sizeof(HighProcess) == 0x46C);

ExtraDataList* ExtraContainerChanges::EntryData::GetEquippedExtra()
{
	if (extendData)
	{
		ExtraDataList* xData;
		ListNode<ExtraDataList>* xdlIter = extendData->Head();
		do
		{
			xData = xdlIter->data;
			if (xData && xData->HasType(kExtraData_Worn))
				return xData;
		} while (xdlIter = xdlIter->next);
	}
	return NULL;
}

class ExtraPoison : public BSExtraData
{
public:
	ExtraPoison();
	~ExtraPoison();

	AlchemyItem* poisonEffect;	// 0C
};

class ExtraCharge : public BSExtraData
{
public:
	float		charge;		// 0C

	static ExtraCharge* Create(float _charge);
};

ExtraCharge* ExtraCharge::Create(float _charge)
{
	UInt32* dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraCharge));
	dataPtr[0] = kVtbl_ExtraCharge;
	dataPtr[1] = kExtraData_Charge;
	dataPtr[2] = 0;
	ExtraCharge* xCharge = (ExtraCharge*)dataPtr;
	xCharge->charge = _charge;
	return xCharge;
}

ExtraAmmo* ExtraAmmo::Create(TESAmmo* ammo, UInt32 count)
{
	UInt32* dataPtr = (UInt32*)GameHeapAlloc(sizeof(ExtraAmmo));
	dataPtr[0] = kVtbl_ExtraAmmo;
	dataPtr[1] = kExtraData_Ammo;
	dataPtr[2] = 0;
	ExtraAmmo* xAmmo = (ExtraAmmo*)dataPtr;
	xAmmo->ammo = ammo;
	xAmmo->count = count;
	return xAmmo;
}

TESActorBase* Actor::GetActorBase()
{
	ExtraLeveledCreature* xLvlCre = GetExtraType(extraDataList, LeveledCreature);
	return (xLvlCre && xLvlCre->form) ? (TESActorBase*)xLvlCre->form : (TESActorBase*)baseForm;
}

bool Actor::IsRobot()
{
	TESCreature* creature = (TESCreature*)this->GetActorBase();
	if IS_TYPE(creature, TESCreature)
	{
		return creature->type == TESCreature::kRobot;
	}
	return false;
}

bool Actor::HasCrippledLimb()
{
	ActorValueOwner* avOwner = &this->avOwner;
	if (!avOwner->GetActorValue(kAVCode_IgnoreCrippledLimbs))
	{
		for (int avCode = kAVCode_PerceptionCondition; avCode <= kAVCode_RightMobilityCondition; avCode++)
		{
			if (avOwner->GetActorValue(avCode) <= 0) return true;
		}
	}
	return false;
}

// A0
class BGSDefaultObjectManager : TESForm
{
public:
	BGSDefaultObjectManager();
	~BGSDefaultObjectManager();

	enum DefaultObjects
	{
		kStimpak = 0x0,
		kSuperStimpak = 0x1,
		kRadX = 0x2,
		kRadAway = 0x3,
		kMorphine = 0x4,
		kPerkParalysis = 0x5,
		kPlayerFaction = 0x6,
		kMysteriousStrangerNPC = 0x7,
		kMysteriousStrangerFaction = 0x8,
		kDefaultMusic = 0x9,
		kBattleMusic = 0xA,
		kDeathMusic = 0xB,
		kSuccessMusic = 0xC,
		kLevelUpMusic = 0xD,
		kPlayerVoice_Male = 0xE,
		kPlayerVoice_MaleChild = 0xF,
		kPlayerVoice_Female = 0x10,
		kPlayerVoice_FemaleChild = 0x11,
		kEatPackageDefaultFood = 0x12,
		kEveryActorAbility = 0x13,
		kDrugWearsOffImageSpace = 0x14,
		kDoctorsBag = 0x15,
		kMissFortuneNPC = 0x16,
		kMissFortuneFaction = 0x17,
		kMeltdownExplosion = 0x18,
		kUnarmedForwardPA = 0x19,
		kUnarmedBackwardPA = 0x1A,
		kUnarmedLeftPA = 0x1B,
		kUnarmedRightPA = 0x1C,
		kUnarmedCrouchPA = 0x1D,
		kUnarmedCounterPA = 0x1E,
		kSpotterEffect = 0x1F,
		kItemDetectedEffect = 0x20,
		kCateyeMobileEffect_NYI = 0x21,
	};

	TESForm* forms[34];

	static BGSDefaultObjectManager* GetSingleton() { return *(BGSDefaultObjectManager * *)0x11CA80C; };
};

struct ScriptRunners
{
	ScriptRunner* runner1;
	ScriptRunner* runner2;

	static ScriptRunners* GetSingleton() { return (ScriptRunners*)0x11CAED0; };
};

__declspec(naked) void Actor::PlayIdle(TESIdleForm *idleAnim)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		sub		esp, 0xC
		mov		[ebp-4], ecx
		mov		eax, [ecx]
		call	dword ptr [eax+0x1E4]
		test	eax, eax
		jz		jmpAddr1
		mov		[ebp-8], eax
		mov		ecx, eax
		mov		eax, 0x4985F0
		call	eax
		test	al, al
		setz	dl
		mov		[ebp-9], dl
		mov		ecx, [ebp-8]
		mov		ecx, [ecx+0x128]
		test	ecx, ecx
		jz		jmpAddr2
		mov		ecx, [ecx+0x2C]
		cmp		ecx, [ebp+8]
		jz		jmpAddr1
	jmpAddr2:
		cmp		byte ptr [ebp-9], 0
		jz		jmpAddr4
		mov		ecx, [ebp-8]
		mov		ecx, [ecx+0x124]
		test	ecx, ecx
		jz		jmpAddr4
		mov		eax, [ecx+0xC]
		cmp		eax, 3
		jnz		jmpAddr3
		mov		ecx, [ecx+0x18]
		test	ecx, ecx
		jz		jmpAddr4
		mov		ecx, [ecx+0x24]
		test	ecx, ecx
		jnz		jmpAddr4
	jmpAddr3:
		mov		[ebp-9], 0
	jmpAddr4:
		cmp		byte ptr [ebp-9], 0
		jnz		jmpAddr1
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x68]
		test	ecx, ecx
		jz		jmpAddr1
		push	dword ptr [ebp+8]
		mov		edx, [ecx]
		mov		edx, [edx+0x71C]
		call	edx
		mov		ecx, [ebp-4]
		mov		ecx, [ecx+0x68]
		push	0x80
		mov		eax, [ecx]
		call	dword ptr [eax+0x614]
	jmpAddr1:
		mov		esp, ebp
		pop		ebp
		retn	4
	}
}

bool __thiscall Actor::IsEssential()
{
	static const UInt32 procAddr = 0x87F3D0;
	_asm
	{
		jmp procAddr
	}
}

bool NoArgsReturnTrue()
{
	return true;
}

class DecalVertex;
class BSTempEffectSimpleDecal : BSTempEffect
{
public:
	BSTempEffectSimpleDecal();
	~BSTempEffectSimpleDecal();

	UInt8 byte018;
	UInt8 byte019;
	UInt8 debugOcclusionQuery;
	UInt8 debugOcclusionQuery_isFirst;
	UInt32 unk01C;
	UInt32 unk020;
	UInt32 unk024;
	NiNode* node028;
	UInt8 unk02C[4];
	UInt32 unk030;
	NiPoint3 pt034;
	NiPoint3 pt040;
	UInt32 unk04C;
	UInt32 unk050;
	UInt32 unk054;
	UInt32 unk058;
	UInt32 unk05C;
	UInt32 unk060;
	UInt32 unk064;
	UInt32 unk068;
	UInt32 unk06C;
	UInt32 unk070;
	UInt32 unk074;
	UInt32 unk078;
	UInt32 unk07C;
	UInt32 unk080;
	UInt32 unk084;
	UInt32 unk088;
	UInt32 unk08C;
	UInt32 unk090;
	UInt32 unk094;
	UInt32 unk098;
	UInt32 unk09C;
	UInt32 unk0A0;
	UInt32 unk0A4;
	UInt32 unk0A8;
	UInt32 unk0AC;
	UInt32 unk0B0;
	UInt32 unk0B4;
	UInt32 unk0B8;
	UInt32 unk0BC;
	UInt32 unk0C0;
	UInt32 unk0C4;
	UInt32 unk0C8;
	UInt32 unk0CC;
	UInt32 unk0D0;
	UInt32 unk0D4;
	UInt32 unk0D8;
	UInt32 unk0DC;
	UInt32 unk0E0;
	UInt32 unk0E4;
	UInt32 unk0E8;
	UInt32 unk0EC;
	UInt32 unk0F0;
	UInt32 unk0F4;
	BSSimpleArray<DecalVertex> decalVertexArray;
	UInt32 unk108;
	NiNode* node10C;
	UInt32 unk110;
	float unk114;
	float unk118;
	float unk11C;
	float unk120;
	UInt8 unk124[4];
	float unk128;
	UInt8 unk12C[4];
	float unk130;
	NiPoint3 pt134;
	UInt8 unk140[4];
};

struct Decal
{
	enum Type
	{
		kDecalType_Simple = 0x1,
		kDecalType_Skinned = 0x2,
		kDecalType_Other = 0x4,
	};

	Decal(TESObjectREFR* ref, NiPoint3* pos, NiPoint3* rotation, BGSImpactData* impactData)
	{
		ThisCall(0x4A37B0, this); // Decal::Decal
		auto camera = SceneGraph::GetSingleton()->camera;
		auto node = ref->GetNiNode()->GetFadeNode();

		this->worldPos = camera->m_worldTranslate;
		this->parentCell = ref->parentCell;

		this->count34 = -1;
		this->rng44 = CdeclCall<float>(0x4A4240);
		this->rng_whichUVQuadrant = GetRandomIntBelow(4);
		this->textureSet = impactData->textureSet;

		auto minWidth = impactData->decalMinWidth;
		auto maxWidth = impactData->decalMaxWidth;
		auto randomInRange = CdeclCall<float>(0x476B70, minWidth, maxWidth);
		this->unk38_betweenMinAndMaxImpactWidth = randomInRange;
		this->unk3C_betweenMinAndMaxImpactWidth = randomInRange;
		
		this->depth = 1000;
		this->angleThreshold = 90;

		bool isActor = ref->IsActor();
		if (isActor || node->unkBC)
		{
			if (isActor)
			{
				this->hitLocationFlags = 0x7FFF;
			}

			this->rotation.x = camera->m_worldRotate.cr[0][0];
			this->rotation.y = camera->m_worldRotate.cr[0][1];
			this->rotation.z = camera->m_worldRotate.cr[0][2];
			this->actor = (Actor*)ref;
		}
		else
		{
			this->node = node;
			this->rotation.x = -camera->m_worldRotate.cr[0][0];
			this->rotation.y = -camera->m_worldRotate.cr[0][1];
			this->rotation.z = -camera->m_worldRotate.cr[0][2];
		}
	}

	NiVector3 worldPos;
	NiPoint3 rotation;
	NiPoint3 pt18;
	Actor* actor;
	BSFadeNode* node;
	UInt32 unk2C;
	BGSTextureSet* textureSet;
	SInt32 count34;
	float unk38_betweenMinAndMaxImpactWidth;
	float unk3C_betweenMinAndMaxImpactWidth;
	float depth;
	float rng44;
	TESObjectCELL* parentCell;
	float parallaxScale;
	NiNode* node50;
	float shininess;
	float angleThreshold;
	float placementRadius;
	NiColor color;
	UInt32 hitLocationFlags;
	UInt8 rng_whichUVQuadrant;
	UInt8 byte71;
	UInt8 byte72;
	UInt8 isParallax;
	UInt8 isAlphaTest;
	UInt8 isAlphaBlend;
	UInt8 parallaxPasses;
	UInt8 ignoreDistToPlayer;
	UInt8 byte78;
	UInt8 isImpactMaterialGlass;
	UInt8 gap7A[2];
};
STATIC_ASSERT(sizeof(Decal) == 0x7C);


class BSTempEffectParticle;
struct DecalManager
{
	struct List14Item
	{
		UInt32 sevrDecalCount;
		UInt8 byte04[4];
		BGSImpactData* impactData;
		BSTempEffectParticle* bsTempEffectParticle;
	};

	struct List08Item
	{
		BSTempEffectSimpleDecal* decal;
	};

	UInt32 unk00;
	UInt8 byte04;
	UInt8 gap05[3];
	DList<List08Item> dList08;
	DList<List14Item> dList14;
	NiNode* node20;
	NiCamera* camera24;

	static DecalManager* GetSingleton() { return *(DecalManager**)0x11C57F8; };
	void AddGeometryDecal(Decal* decal, Decal::Type decalType, bool ignoreDistToPlayer) { ThisCall(0x4A10D0, this, decal, decalType, ignoreDistToPlayer); }
};
STATIC_ASSERT(sizeof(DecalManager) == 0x28);

const char* TESForm::GetDescriptionText()
{
	if (typeID == kFormType_BGSNote)
	{
		BGSNote* note = (BGSNote*)this;
		return ((note->noteType == 1) && note->noteText) ? note->noteText->Get(this, 'MANT') : "";
	}
	TESDescription* description = DYNAMIC_CAST(this, TESForm, TESDescription);
	return description ? description->Get(this, 'CSED') : "";
}

struct PCMiscStat
{
	const char* name;
	UInt32		level;
};

// 150
class Projectile : public MobileObject
{
public:
	Projectile();
	~Projectile();

	enum
	{
		kProjType_Beam =	1,
		kProjType_Flame,
		kProjType_Grenade,
		kProjType_Missile,
		kProjType_ContinuousBeam
	};

	virtual UInt32	GetProjectileType();
	virtual void	Unk_C2(void);
	virtual void	Unk_C3(void);
	virtual void	Unk_C4(void);
	virtual bool	ProcessImpact();
	virtual bool	IsProximityTriggered();
	virtual void	Unk_C7(void);
	virtual bool	DisarmPlacedExplosives(TESObjectREFR *refr, bool unk);
	virtual void	Unk_C9(void);
	virtual void	Unk_CA(void);
	virtual void	Unk_CB(void);

	enum
	{
		kProjFlag_Bit00Unk =			0x1,
		kProjFlag_Bit01Unk =			0x2,
		kProjFlag_Bit02Unk =			0x4,
		kProjFlag_Bit03Unk =			0x8,
		kProjFlag_Bit04Unk =			0x10,
		kProjFlag_Bit05Unk =			0x20,
		kProjFlag_Bit06Unk =			0x40,
		kProjFlag_Bit07Unk =			0x80,
		kProjFlag_Bit08Unk =			0x100,
		kProjFlag_MineDisarmed =		0x200,
		kProjFlag_Bit0AUnk =			0x400,
		kProjFlag_Bit0BUnk =			0x800,
		kProjFlag_Bit0CUnk =			0x1000,
		kProjFlag_Bit0DUnk =			0x2000,
		kProjFlag_Bit0EUnk =			0x4000,
		kProjFlag_Bit0FUnk =			0x8000,		// Don't apply source-weapon's damage upon impact
		kProjFlag_Bit10Unk =			0x10000,
		kProjFlag_Bit11Unk =			0x20000,
		kProjFlag_Bit12Unk =			0x40000,
		kProjFlag_Bit13Unk =			0x80000,
		kProjFlag_Bit14Unk =			0x100000,
	};

	struct ImpactData
	{
		TESObjectREFR	*refr;			// 00
		float			posX;			// 04
		float			posY;			// 08
		float			posZ;			// 0C
		float			flt10;			// 10
		float			flt14;			// 14
		float			flt18;			// 18
		hkpRigidBody	*rigidBody;		// 1C
		UInt32			materialType;	// 20
		SInt32			hitLocation;	// 24
		UInt32			unk28;			// 28
		UInt32			unk2C;			// 2C

		UInt8 GetMaterial() const
		{
			static const UInt8 kMaterialConvert[] = {0, 7, 1, 3, 2, 4, 6, 6, 8, 5, 0, 4, 5, 4, 4, 4, 9, 9, 1, 0, 4, 4, 9, 9, 3, 9, 4, 4, 9, 9, 5, 5};
			return (materialType <= 31) ? kMaterialConvert[materialType] : 0;
		}
	};

	struct Struct128
	{
		UInt32			unk00;
		UInt8			byte04;
		UInt8			pad05[3];
		UInt32			status;		//	0 - Not triggered, 1 - Triggered, 2 - Disarmed
	};

	tList<ImpactData>	impactDataList;	// 088
	UInt8				hasImpacted;	// 090
	UInt8				pad091[3];		// 091
	float				unk094[13];		// 094
	UInt32				projFlags;		// 0C8
	float				speedMult1;		// 0CC
	float				speedMult2;		// 0D0
	float				flt0D4;			// 0D4
	float				elapsedTime;	// 0D8
	float				hitDamage;		// 0DC
	float				flt0E0;			// 0E0
	float				detonationTime;	// 0E4
	float				flt0E8;			// 0E8
	float				flt0EC;			// 0EC
	float				flt0F0;			// 0F0
	float				wpnHealthPerc;	// 0F4
	TESObjectWEAP		*sourceWeap;	// 0F8
	TESObjectREFR		*sourceRef;		// 0FC
	UInt32				unk100;			// 100
	float				flt104;			// 104
	float				flt108;			// 108
	float				flt10C;			// 10C
	float				distTravelled;	// 110
	NiRefObject			*object114;		// 114
	UInt8				byte118;		// 118
	UInt8				pad119[3];		// 119
	NiNode				*node11C;		// 11C
	UInt32				unk120;			// 120
	float				flt124;			// 124
	Struct128			unk128;			// 128
	Struct128			unk134;			// 134
	UInt32				unk140;			// 140
	ContChangesEntry*	rockItEntry;	// 144
	UInt8				byte148;		// 148
	UInt8				pad149[3];		// 149
	float				range;			// 14C
};
STATIC_ASSERT(sizeof(Projectile) == 0x150);

// 1C
struct VoiceEntry
{
	struct Response
	{
		String str;
		char* fileName;
	};

	tList<void> list00;
	Response* response;
	TESTopicInfo* topicInfo;
	TESTopic* topic;
	TESQuest* quest;
	Actor* actor;
};


namespace Radio
{
	void (*SetEnabled)(bool toggleON) = (void(__cdecl*)(bool))0x8324E0;
	void (*SetActiveStation)(TESObjectREFR* station, bool toggleON) = (void(__cdecl*)(TESObjectREFR*, bool))0x832240;
	bool GetEnabled() { return *(UInt8*)0x11DD434; }

	struct RadioEntry
	{

		struct Struct04
		{

			struct Struct00
			{
				tList<VoiceEntry> voiceEntries;
				Struct00* next;
			};

			Struct00* unk00;
			UInt32 unk04;
			UInt32 rng08;
			UInt32 soundTimeRemaining0C;
			UInt8 byte10;
			UInt8 byte11;
			UInt8 gap12[2];
			UInt32 flags;
			tList<void> list18;
		};

		
		TESObjectREFR* radioRef;
		Struct04 unk04;

		static RadioEntry* GetSingleton() { return *(RadioEntry * *)0x11DD42C; }
	};

	TESObjectREFR* GetCurrentStation()
	{
		TESObjectREFR* station = nullptr;
		if(RadioEntry::GetSingleton())
		{
			station = RadioEntry::GetSingleton()->radioRef;
		}

		return station;
	}
	typedef RadioEntry::Struct04::Struct00 VoiceEntryList;
}

signed int EffectItem::GetSkillCode()
{
	return ThisCall<signed int>(0x403EA0, this);
}

float GetPlayerHealthEffectsSum()
{
	auto effects = g_thePlayer->magicTarget.GetEffectList();
	float sum = 0;
	for (auto iter = effects->Begin(); !iter.End(); ++iter)
	{
		auto effect = iter.Get();
		EffectItem* effectItem = effect->effectItem;
		if (!effect->bApplied || effect->duration <= 0 || !effectItem || effectItem->GetSkillCode() != kAVCode_Health || !effectItem->setting) continue;
		
		sum += effectItem->magnitude * (effect->duration - effect->timeElapsed);
	}

	float playerMedicineSkillMult = g_thePlayer->cvOwner.GetMedicineSkillMult();
	sum *= playerMedicineSkillMult;

	ApplyPerkModifiers(kPerkEntry_ModifyRecoveredHealth, g_thePlayer, &sum);

	return sum;
}

bool Actor::IsInReloadAnim() const
{
	auto action = this->baseProcess->GetCurrentAnimAction();
	return action == kReload || action == kReloadLoop || action == kReloadLoopEnd || action == kReloadLoopStart;
}

double GetVectorAngle2D(NiPoint3* pt)
{
	double angle;
	if(pt->y == 0)
	{
		if(pt->x <= 0)
		{
			angle = kDblPIx3d2;
		}
		else
		{
			angle = kDblPId2;
		}
	}
	else
	{
		double ratio = pt->x / pt->y;
		angle = dAtan(ratio);
		if(pt->y < 0.0)
		{
			angle += kDblPI;
		}
	}
	
	return angle;
}

double GetAngleBetweenPoints(NiPoint3* actorPos, NiPoint3* playerPos, float offset)
{
	NiPoint3 diff;
	diff.Init(actorPos);
	diff.Subtract(playerPos);

	double angle = GetVectorAngle2D(&diff) - offset;
	if (angle > -kDblPI)
	{
		if (angle > kDblPI)
		{
			angle = kDblPIx2 - angle;
		}
	}
	else
	{
		angle += kDblPIx2;
	}
	return angle * kDblRad2Deg;
}

__declspec(naked) TESObjectCELL *TESObjectREFR::GetParentCell()
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		done
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	done:
		retn
	}
}

__declspec(naked) TESWorldSpace *TESObjectREFR::GetParentWorld()
{
	__asm
	{
		mov		eax, [ecx+0x40]
		test	eax, eax
		jnz		getWorld
		push	kExtraData_PersistentCell
		add		ecx, 0x44
		call	BaseExtraList::GetByType
		test	eax, eax
		jz		done
		mov		eax, [eax+0xC]
	getWorld:
		mov		eax, [eax+0xC0]
	done:
		retn
	}
}

__declspec(naked) bool TESObjectREFR::GetDisabled()
{
	__asm
	{
		push	ecx
		test	dword ptr [ecx+8], 0x800
		jz		notPerm
		mov		eax, 0x5AA680
		call	eax
		xor		al, 1
		jnz		done
	notPerm:
		mov		eax, 0x5AA630
		call	eax
	done:
		pop		ecx
		retn
	}
}

struct GameTimeGlobals
{
	TESGlobal* year;
	TESGlobal* month;
	TESGlobal* day;
	TESGlobal* hour;
	TESGlobal* daysPassed;
	TESGlobal* timeScale;
	UInt32 unk18;
	bool gameLoaded;
	UInt8 pad1D[3];
	UInt32 unk20;
	UInt32 unk24;
	UInt32 unk28;
	float lastUpdHour;
	UInt32 initialized;
};

// 10
class ExtraAshPileRef : public BSExtraData
{
public:
	ExtraAshPileRef();
	~ExtraAshPileRef();

	TESObjectREFR* sourceRef;		// 0C
};

/*
void PlayHolotape(BGSNote* note)
{
	if (note->noteType == BGSNote::kVoice && note->voice && note->voiceNPC)
	{
		auto character = (Character*)GameHeapAlloc(sizeof(Character));
		aThsStdCallAlt(0x8D1F40, character, 0); // Character::Init

		aThsStdCallAlt(0x484490, character);

		TESNPC* npc = note->voiceNPC;
		aThsStdCallAlt(0x575690, character, npc); // TESObjectREFR__InitBaseForm

		auto voiceEntryList = (Radio::VoiceEntryList*)GameHeapAlloc(0xC);
		aThsStdCallAlt(0x83B850, voiceEntryList, character, g_thePlayer, note->voice);
		voiceEntryList->next = voiceEntryList;
		
//		if (VoiceEntry::GetNext(voiceEntry_))
		{
//			auto v10 = voiceEntry->next;


//			if (sub_83C7B0(v10))
			{

				auto map = MapMenu::GetSingleton();

				Radio::VoiceEntryList* nextEntry;
				VoiceEntry* voiceEntry;
				do
				{
					VoiceEntry*nextEntry = (VoiceEntry*)voiceEntryList->next;
					aThsStdCallAlt(0x7A1AC0, &map->arr0A8, &nextEntry->response->str);

					auto topicInfo = nextEntry->topicInfo;
					aThsStdCallAlt(0x61F170, topicInfo, 0, character);
					char* fileName = nextEntry->response->fileName;

					Sound sound;
					aThsStdCallAlt(0xAD7480, NULL, &sound, fileName, BSAudioManager::kAudioFlags100 | BSAudioManager::kAudioFlags_UseMissingVoiceAssetIfMissing | BSAudioManager::kAudioFlags_2D, 0);
					aThsStdCallAlt(0xAD89E0, &sound, 0.89999998);
					
					aThsStdCallAlt(0x7A19A0, map->someSoundsList, &sound);
					
					aThsStdCallAlt(0x61F170, topicInfo, 1, character);
					map->isHolotapeVoicePlaying = 1;
				} while (aThsStdCallAlt_B(0x83C7E0, nextEntry));
			}
		}
		
		aThsStdCallAlt(0x798450, voiceEntryList, true); // VoiceEntry::Destroy
		character->Destroy(true);
	}
}
*/
_declspec(naked) void __HackyPlayHolotapeEnd()
{
	static const char* originalCode = "\x8B\x8D\x08\xFE\xFF";
	_asm
	{
		push 5
		push originalCode
		push 0x79761F
		call SafeWriteBuf
		mov esp, ebp
		pop ebp
		ret
	}
}

_declspec(naked) void __fastcall HackyPlayHolotape(BGSNote* note)
{
	// this function plays a holotape by setting up the stack frame for a vanilla function, jumping halfway into it and then jumping out again
	_asm
	{
		cmp [ecx + 0x6C], 0
		je noAudio
		cmp [ecx + 0x70], 0
		je noNPC

		push ebp
		mov esp, ebp
		sub esp, 0x264
		mov [ebp - 0x14], ecx // note
		mov edx, dword ptr ds : [0x11DA368] // g_mapMenu
		mov [ebp - 0x1F8], edx
		push __HackyPlayHolotapeEnd
		push 0x79761F
		push 0x797372 // retnAddr
		jmp WriteRelJump

	noAudio:
	noNPC:
		ret
	}
}

bool __fastcall MagicTargetHasTempNegativeHealthEffect(MagicTarget* target)
{
	ActiveEffectList* effList = target->GetEffectList();
	
	for (auto iter = effList->Begin(); !iter.End(); ++iter)
	{
		ActiveEffect* effect = iter.Get();
		auto effectItem = effect->effectItem;
		auto effectSetting = effectItem->setting;
		if (ThisCall<bool>(0x403C60, effectSetting))
		{
			if (effectItem->GetSkillCode() == kAVCode_Health)
			{
				if (!(effectSetting->effectFlags & EffectSetting::kDetrimental)) continue;
				if (!effect->bApplied || effect->duration <= 0) continue;
				return true;
			}
		}
	}
	return false;
}

enum CriticalStages
{
	kCriticalStage_None = 0x0,
	kCriticalStage_GooStart = 0x1,
	kCriticalStage_GooEnd = 0x2,
	kCriticalStage_DisintegrateStart = 0x3,
	kCriticalStage_DisintegrateEnd = 0x4,
};

bool Actor::IsMoving() const
{
	return this->actorMover->GetMovementFlags() & (MovementFlags::kMoving);
}

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

	static RepairServicesMenu* GetSingleton() { return *(RepairServicesMenu * *)0x11DA7F0; };
	static Actor* GetActor() { return *(Actor * *)0x11DA7F4; };
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

struct Semaphore
{
	UInt32 count;
	HANDLE semaphore;
	UInt32 max;
};

class BGSMoviePlayer
{
public:
	virtual void    Destroy(bool doFree);
	virtual void    Func0001();
	virtual void    Func0002();
	virtual void    Func0003();
	virtual void    Func0004();
	virtual void    Func0005();
	virtual void    Func0006();
	virtual void    Func0007();
	virtual void    Func0008();
	virtual void    Func0009();
	virtual int		GetWidth();
	virtual int    GetHeight();
	virtual void    Func000C();
	virtual void    Func000D();
	virtual void    Func000E();
	virtual int		GetMainThreadID();
	virtual void    Update();

	UInt32 unk04;
	HANDLE* thread;
	UInt32 ptr0C;
	UInt32 unk10;
	UInt32 unk14;
	UInt8 byte18;
	UInt8 byte19;
	UInt8 gap1A[2];
	UInt32 unk1C;
	UInt8 byte20;
	UInt8 byte21;
	UInt8 byte22;
	UInt8 byte23;
	UInt8 byte24;
	UInt8 gap25[3];
	UInt32 unk28;
	UInt32 unk2C;
	Semaphore semaphor;
	UInt32 unk3C;
	NiTMapBase<const char*, char*> map;
	UInt32 unk50;

	static BGSMoviePlayer* GetSingleton() { return *(BGSMoviePlayer * *)0x126FAC4; };
};
STATIC_ASSERT(sizeof(BGSMoviePlayer) == 0x54);

void OSInputGlobals::ResetControlState(UInt8 code)
{
	UInt8 keyBind = this->keyBinds[code];
	if (keyBind != kControlCode_NONE)
	{
		this->currKeyStates[keyBind] = 0;
	}

	keyBind = this->mouseBinds[code];
	if (keyBind != kControlCode_NONE)
	{
		this->currButtonStates[keyBind] = 0;
	}
}

__declspec(naked) bool ConditionList::Evaluate(TESObjectREFR* runOnRef, TESForm* arg2, bool* result, bool arg4)
{
	static const UInt32 procAddr = kAddr_EvaluateConditions;
	__asm	jmp		procAddr
}

bool AlchemyItem::RestoresAV(int avCode)
{
	auto iter = this->magicItem.list.list.Begin();
	for (; !iter.End(); ++iter)
	{
		auto effect = iter.Get();
		if (effect->GetSkillCode() == avCode)
		{
			auto setting = effect->setting;
			if (setting && !(setting->effectFlags & EffectSetting::kDetrimental))
			{
				// check if player meets the conditions for the item
				bool eval;
				if (effect->conditions.Evaluate(g_thePlayer, nullptr, &eval, false)) return true;
			}
		}
	}
	return false;
}

NiProperty* NiAVObject::GetProperty(UInt32 propID)
{
	NiProperty* niProp;
	for (DListNode<NiProperty>* propNode = m_propertyList.Head(); propNode; propNode = propNode->next)
	{
		niProp = propNode->data;
		if (niProp && (niProp->GetPropertyType() == propID))
			return niProp;
	}
	return NULL;
}

void NiObjectNET::DumpExtraData()
{
	gLog.Indent();
	NiExtraData* xData;
	for (UInt32 iter = 0; iter < m_extraDataListLen; iter++)
	{
		xData = m_extraDataList[iter];
		if (xData)
			_MESSAGE("(X) %08X\t%08X\t%s", xData, *(UInt32*)xData, xData->GetType()->name);
	}
	gLog.Outdent();
}

void NiAVObject::DumpProperties()
{
	gLog.Indent();
	NiProperty* niProp;
	for (DListNode<NiProperty>* traverse = m_propertyList.Head(); traverse; traverse = traverse->next)
	{
		niProp = traverse->data;
		if (niProp)
		{
			_MESSAGE("(P) %08X\t%08X\t%s", niProp, *(UInt32*)niProp, niProp->GetType()->name);
			niProp->DumpExtraData();
		}
	}
	gLog.Outdent();
}

void NiNode::Dump()
{
	_MESSAGE("(N) %08X\t%s\t%s\t%08X", this, GetType()->name, m_blockName ? m_blockName : "(NO NAME)", m_flags);
	DumpExtraData();
	DumpProperties();
	gLog.Indent();
	if (m_collisionObject)
		_MESSAGE("(C) %08X\t%s", m_collisionObject, m_collisionObject->GetType()->name);

	NiAVObject* block;
	for (NiTArray<NiAVObject*>::Iterator iter(m_children); !iter.End(); ++iter)
	{
		block = *iter;
		if (!block) continue;
		if (block->GetNiNode()) ((NiNode*)block)->Dump();
		else
		{
			_MESSAGE("(B) %08X\t%s\t%s\t%08X", block, block->GetType()->name, block->m_blockName ? block->m_blockName : "(NO NAME)", block->m_flags);
			block->DumpExtraData();
			block->DumpProperties();
		}
	}
	gLog.Outdent();
}

bool TESObjectWEAP::CanEquipAmmo(TESAmmo* toEquip)
{
	if (!ammo.ammo) return false;
	if IS_ID(ammo.ammo, BGSListForm)
	{
		return ((BGSListForm*)ammo.ammo)->list.GetIndexOf((TESForm*)toEquip) != -1;
	}
	else
	{
		return (TESAmmo*)ammo.ammo == toEquip;
	}
}

enum AnimKeyTypes
{
	kAnimKeyType_ClampSequence = 0x0,
	kAnimKeyType_LoopingSequence = 0x1,
	kAnimKeyType_SpecialIdle = 0x2,
	kAnimKeyType_Equip = 0x3,
	kAnimKeyType_Unequip = 0x4,
	kAnimKeyType_Attack = 0x5,
	kAnimKeyType_PowerAttackOrPipboy = 0x6,
	kAnimKeyType_AttackThrow = 0x7,
	kAnimKeyType_PlaceMine = 0x8,
	kAnimKeyType_SpinAttack = 0x9,
	kAnimKeyType_LoopingReload = 0xA,
};

enum AnimHandTypes
{
	kAnim_H2H = 0,
	kAnim_1HM,
	kAnim_2HM,
	kAnim_1HP,
	kAnim_2HR,
	kAnim_2HA,
	kAnim_2HH,
	kAnim_2HL,
	kAnim_1GT,
	kAnim_1MD,
	kAnim_1LM,
	kAnim_Max,
};

struct AnimGroupInfo
{
	const char* name;
	UInt32 hasVariants;
	BSAnimGroupSequence::eAnimSequence sequenceType;
	AnimKeyTypes keyType;
	UInt32 unk10;
	UInt32 unk14[4];
};
STATIC_ASSERT(sizeof(AnimGroupInfo) == 0x24);

// 104
class Explosion : public MobileObject
{
public:
	virtual void	Unk_C1(void);

	float			unk088;			// 088
	float			unk08C;			// 08C
	float			unk090;			// 090
	float			unk094;			// 094
	float			unk098;			// 098
	float			unk09C;			// 09C
	NiRefObject		*object0A0;		// 0A0
	tList<void>		list0A4;		// 0A4
	UInt32			unk0AC[6];		// 0AC
	NiPointLight	*pointLight;	// 0C4
	UInt32			unk0C8[2];		// 0C8
	NiRefObject		*object0D0;		// 0D0
	UInt32			unk0D4[11];		// 0D4
	float			unk100;			// 100
};
STATIC_ASSERT(sizeof(Explosion) == 0x104);

// checks if a target is in an actor's combat targets list
__declspec(naked) bool Actor::IsInCombatWith(Actor *target)
{
	__asm
	{
		mov		eax, [ecx+0x12C] // combatTargets
		test	eax, eax
		jz		done
		mov		ecx, [eax+4] // iter
		mov		eax, [eax+8] // numTargets
		mov		edx, [esp+4] // target
	iterHead:
		test	eax, eax
		jz		done
		cmp		[ecx], edx
		jz		rtnTrue
		add		ecx, 4
		dec		eax
		jnz		iterHead
		jmp		done
	rtnTrue:
		mov		al, 1
	done:
		retn	4
	}
}

__declspec(naked) bool TESForm::IsReference()
{
	__asm
	{
		mov		eax, [ecx]
		cmp		eax, kVtbl_TESObjectREFR
		jz		done
		cmp		eax, kVtbl_PlayerCharacter
		jz		done
		cmp		eax, kVtbl_Character
		jz		done
		cmp		eax, kVtbl_Creature
		jz		done
		cmp		eax, kVtbl_GrenadeProjectile
	done:
		setz	al
		retn
	}
}

struct TLSData
{
	UInt32			unk000;				// 000
	UInt32			unk004;				// 004
	ExtraDataList* lastXtraList;		// 008
	UInt32			unk00C;				// 00C
	BSExtraData* xDatas[kExtraData_Max];	// 010
	UInt32			unk25C;				// 25C
	NiNode* lastNiNode;		// 260
	TESObjectREFR* lastNiNodeREFR;	// 264
	UInt8			consoleMode;		// 268
	UInt8			pad269[3];			// 269
	UInt32			unk26C[10];			// 26C
	UInt32			flags294;			// 294
	UInt32			unk298[7];			// 298
	UInt32			heapIndex;			// 2B4
	UInt32			unk2B8;				// 2B8
	UInt32			unk2BC;				// 2BC
	// 25C is used as do not head track the player , 2B8 is used to init QueudFile::unk0018
};
STATIC_ASSERT(sizeof(TLSData) == 0x2C0);

__declspec(naked) TLSData* GetTLSData()
{
	__asm
	{
		mov		eax, ds: [0x126FD98]
		mov		edx, fs : [0x2C]
		mov		eax, [edx + eax * 4]
		retn
	}
}

class LODNode;
class BGSTerrainChunkLoadTask;
struct __declspec(align(4)) BGSTerrainChunk
{
	LODNode* parentLODNode;
	UInt32 unk004;
	UInt32 unk008;
	UInt32 unk00C;
	BSMultiBoundNode* multiBoundNode10;
	BSMultiBoundNode* multiBoundNode14;
	BSMultiBoundNode* multiBoundNode18;
	UInt32 unk01C;
	UInt32 unk020;
	BGSTerrainChunkLoadTask* chunkLoadTask;
	UInt8 byte028;
	UInt8 byte029;
	UInt8 byte02A;
	UInt8 gap02B;
	UInt32 unk02C;
};

struct NavMeshEdgeLocation
{
	UInt32 unk00;
	UInt32 unk04;
	UInt32 unk08;
	NiPoint3 pos;
};
STATIC_ASSERT(sizeof(NavMeshEdgeLocation) == 0x18);

class PathingSearchRayCast
{
public:
	
	virtual void Destroy(bool doFree);

	PathingLocation path04;
	PathingLocation path02C;
	UInt32 ptr054;
	UInt32 unk058;
	NiPoint3 pt05C;
	NiPoint3 pt068;
	float unk074;
	UInt32 unk078;
	UInt32 unk07C;
	UInt32 unk080;
	UInt32 unk084;
	UInt32 unk088;
	float jumpFallHeightMin;
	float unk090;
};

bool IsConsoleOpen()
{
	return *(byte*)0x11DEA2E;
}

struct NavMeshClosedDoorInfo;
typedef NiPoint3 NavMeshVertex;

// 108
class NavMesh : public TESForm
{
public:
	virtual void		Unk_4E(void);

	TESChildCell								childCell;			// 018
	NiRefObject									refObject;			// 01C
	TESObjectCELL* parentCell;		// 024
	BSSimpleArray<NavMeshVertex>				vertexArr;			// 028
	BSSimpleArray<NavMeshTriangle>				triangleArr;		// 038
	BSSimpleArray<EdgeExtraInfo>				edgeInfoArr;		// 048
	BSSimpleArray<NavMeshTriangleDoorPortal>	doorPortalArr;		// 058
	BSSimpleArray<NavMeshClosedDoorInfo>		closedDorrArr;		// 068
	BSSimpleArray<UInt16>						unk078Arr;			// 078
	NiTMapBase<UInt16, NavMeshPOVData*>			povDataMap;			// 088
	BSSimpleArray<UInt16>						unk098Arr;			// 098
	UInt32										unk0A8;				// 0A8
	float										unk0AC[8];			// 0AC
	BSSimpleArray<UInt16>* arrPtr0CC;			// 0CC
	BSSimpleArray<ObstacleUndoData>				obstacleUndoArr;	// 0D0
	NiTMapBase<UInt16, ObstacleData*>* obstacleDataMap;	// 0E0
	BSSimpleArray<UInt16>						unk0E4Arr;			// 0E4
	BSSimpleArray<NavMeshStaticAvoidNode>		avoidNodeArr;		// 0F4
	UInt32* ptr104;			// 104
};
STATIC_ASSERT(sizeof(NavMesh) == 0x108);

 template <typename T_Key, typename T_Data> struct UnkLockFreeMapPointer
{
	LockFreeMap<T_Key, T_Data>* lockFreeMap;
	UInt32 unk004;
	UInt32 unk008;
	UInt32 unk00C;
	UInt32 unk010;
	UInt32 unk014;
	UInt32 unk018;
	UInt32 unk01C;
	UInt32 unk020;
};

 class BSArchiveHeader
 {
 public:
 };

 // 70
 class BSArchive : public BSArchiveHeader
 {
 public:
	 UInt32		unk00;			// 00	160
	 UInt32		unk04;			// 04	164
	 UInt32		unk08;			// 08	168
	 UInt32		unk0C;			// 0C	16C
	 UInt32		unk10;			// 10	170
	 UInt32		unk14;			// 14	174
	 UInt32		unk18;			// 18	178
	 UInt32		unk1C;			// 1C	17C
	 UInt16		fileTypesMask;	// 20	180
	 UInt16		word22;			// 22	182
	 UInt32		unk24[19];		// 24	184
 };
 STATIC_ASSERT(sizeof(BSArchive) == 0x70);

 // 1D0
 class Archive : public BSFile
 {
 public:
	 NiRefObject			refObject;		// 158
	 BSArchive			archive;		// 160
 };
 STATIC_ASSERT(sizeof(Archive) == 0x1D0);

 // 160
 class ArchiveFile : public BSFile
 {
 public:
	 UInt32			unk158;		// 158
	 UInt32			unk15C;		// 15C
 };
 STATIC_ASSERT(sizeof(ArchiveFile) == 0x160);

 void OSInputGlobals::ResetMouseButtons()
 {
	 for (int i = 0; i < 8; ++i)	this->currButtonStates[i] = 0;
	 for (int i = 0; i < 8; ++i)	this->lastButtonStates[i] = 0;
 }

 bool GetIsKeyTapped(int key)
 {
	 if (!key) return false;
	 return OSInputGlobals::GetSingleton()->GetKeyState(key, isPressed);
 }

 bool GetIsMouseButtonPressed(int buttonID, KeyState state)
 {
	 return OSInputGlobals::GetSingleton()->GetMouseState(buttonID, state);
 }

 enum TESTopic_Type : __int8
 {
	 kTESTopicType_Topic = 0x0,
	 kTESTopicType_Conversation = 0x1,
	 kTESTopicType_Combat = 0x2,
	 kTESTopicType_Persuasion = 0x3,
	 kTESTopicType_Detection = 0x4,
	 kTESTopicType_Service = 0x5,
	 kTESTopicType_Miscellaneous = 0x6,
	 kTESTopicType_Radio = 0x7,
 };

 bool __stdcall isPlayerInDanger()
 {
	 return g_thePlayer->pcInCombat && !g_thePlayer->pcUnseen;
 }

 enum FollowerTopics
 {
	 kFollowerTopic_KOed = 0x0,
	 kFollowerTopic_KnockedBack = 0x1,
	 kFollowerTopic_Crippled = 0x2,
	 kFollowerTopic_HealthHalf = 0x4,
	 kFollowerTopic_HealthQuarter = 0x5,
	 kFollowerTopic_Crippled2 = 0x6,
	 kFollowerTopic_NoAmmo = 0x7,
	 kFollowerTopic_WeaponBroke = 0x8,
	 kFollowerTopic_WeaponIneffective = 0x9,
	 kFollowerTopic_UsedDoctorBag = 0xA,
	 kFollowerTopic_Wake = 0xB,
	 kFollowerTopic_FollowersStealthing = 0xC,
	 kFollowerTopic_Regenerating = 0xD,
 };
