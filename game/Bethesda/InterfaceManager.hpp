#pragma once
#include "NiTArray.hpp"
#include "NiPoint2.hpp"
#include "BGSMenuPacker.hpp"
#include "NiRefObject.hpp"
#include "NiSmartPointer.hpp"
#include "FOPipboyManager.hpp"
#include "BSGraphics.hpp"

// maps need defined type
#include "BSRenderedTexture.hpp"

class bhkSimpleShapePhantom;

class NiAlphaProperty;

class Menu;
class FORenderedMenu;

class TileMenu;
class TileImage;
class SceneGraph;
class ImageSpaceShaderParam;
class ShadowSceneNode;
class BSRenderedTexture;

enum SpecialInputCode : UInt32
{
	kInputCode_Backspace	= 0x80000000,
	kInputCode_ArrowLeft	= 0x80000001,
	kInputCode_ArrowRight	= 0x80000002,
	kInputCode_ArrowUp		= 0x80000003,
	kInputCode_ArrowDown	= 0x80000004,
	kInputCode_Home			= 0x80000005,
	kInputCode_End			= 0x80000006,
	kInputCode_Delete		= 0x80000007,
	kInputCode_Enter		= 0x80000008,
	kInputCode_PageUp		= 0x80000009,
	kInputCode_PageDown		= 0x8000000A
};

struct PlayerCameraSphere
{
	bhkSimpleShapePhantom* unk000;
	bhkSimpleShapePhantom* unk004;
};

// 0x580
class InterfaceManager
{
public:
	InterfaceManager();
	~InterfaceManager();

	// 0x178
	struct Struct0178
	{
		NiPointer<NiRefObject>	spUnk0;
		BGSMenuPacker			kPacker;
		UInt8					byte28;
		UInt8					byte29;
		UInt8					byte2A;
		UInt8					byte2B;
		Float32					fStartTime;
		NiPoint2				kDuration;
		NiPoint2				kIntensity;
		NiPoint2				kFrequency;
		NiPoint2				kUnk48;
		ImageSpaceShaderParam*	pImageSpaceEffectParam;
		bool					bIsFlyCamEnabled;
		UInt8					byte55;
		UInt8					byte56;
		UInt8					byte57;
		Float32					fBlurRadiusHUD;
		Float32					fScanlineFrequencyHUD;
		Float32					fBlurIntensityHUD;
	};

	struct TargetData
	{
		TESObjectREFR*		pRef;
		NiPointer<NiNode>	spkNode;
	};

	struct VATSHighlightData {
		enum QueryType
		{
			LIMB_QUERY_NONE = 0x0,
			LIMB_QUERY_FULL = 0x1,
			LIMB_QUERY_PARTIAL = 0x2,
		};

		UInt32							eMode;
		TargetData						kTarget;
		UInt32							uiNumHighlightedRefs;
		UInt32							uiFlashingRefIndex;
		TargetData						kHighlightedRefs[32];
		NiPointer<NiRefObject>			spUnk114;
		bool							bIsOcclusionEnabled;
		bool							unused119[16];
		bool							byte12B;
		BSGraphics::OcclusionQuery*		pPartialLimbQuery[16];
		BSGraphics::OcclusionQuery*		pEntireLimbQuery[16];
		bool							bHasExecutedQuery[16];
		UInt32							uiTotalArea[16];
		UInt32							uiVisibleArea[16];
		UInt32							uiSelectedLimbID;
		UInt32							uiNumHighlightedBodyParts;
		UInt32							uiHighLightedBodyParts[16];
		bool							byte284;
		Float32							time288;
		Float32							fDistortVertical;
		UInt32							fDistortDuration;
		Float32							fVATSDistortTime;
		bool							byte298;
		Float32							time29C;
		Float32							fBurstDuration;
		Float32							fBurstIntensity2;
		Float32							fBurstIntensity;
		Float32							fPulseIntensity;
		Float32							fPulseRadius;
		bool							byte2B4;
		bool							byte2B5;
		Float32							fUnk2B8;
		Float32							fUnk2BC;
		Float32							fVATSTargetPulseRate;
		NiPointer<NiTexture>			spkScanlineTexture;
		UInt32							uiVisibleAreaScale;
		NiPointer<BSShaderAccumulator>	spkAccumulator;

		static BSRenderedTexture* GetCurrentHighlightedNodeTexture() {
			return (*(NiPointer<BSRenderedTexture>*)0x11DEB38);
		}

		static void SetCurrentHighlightedNodeTexture(BSRenderedTexture* apTexture) {
			(*(NiPointer<BSRenderedTexture>*)0x11DEB38) = apTexture;
		}

		void HandleOcclusionQueries(BSRenderedTexture* apCurrentNodeTexture, QueryType aeQueryType, bool a4, int a5, NiRenderer::ClearFlags uiClearMode, bool abEndFrame);

		NiNode* GetTargetNode() const {
			return kTarget.spkNode.m_pObject;
		}

		void AddRef(TESObjectREFR* ref);
		void AddRefAndSetFlashing(TESObjectREFR* ref);
		void ResetRefs();
	};

	struct Tutorials
	{
		SInt32 tutorialFlags[41];
		UInt32 currentShownHelpID;
		UInt32 timeA8;
	};

	struct MenuFade
	{
		Menu*		menu;
		Float32		fadeTimer;
		Float32		fadeDuration;
		MenuFade*	prev;
		MenuFade*	next;
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

	UInt32							eFlags;				// 000
	NiPointer<SceneGraph>			spMenuRoot;			// 004
	NiPointer<SceneGraph>			spMenu3DRoot;		// 008
	UInt32							uiCurrentMode;		// 00C	1 = GameMode; 2 = MenuMode
	// checked for 1 at 0x70BA97
	// set to 2 at 0x70BA8D
	// set to 3 at 0x714E96, 0x714E20 - checked at 0x70B94E
	// set to 4 at 0x714D5D, 0x715177 (CloseAllMenus)
	// set to 5 at 0x70B972 - checked at 0x70BA84, 0x70CA14, 0x70CC7A
	UInt8							byte010;
	UInt8							byte011;
	UInt8							statsMenuTab;
	UInt8							inventoryMenuTab;
	UInt8							byte14;
	UInt8							mapMenuTab;
	UInt8							gap16[2];
	UInt32							uiPickLength;		// 018
	UInt32							unk01C;				// 01C
	UInt8							byte020;			// 020
	UInt8							byte021;			// 021
	UInt8							byte022;			// 022
	UInt8							byte023;			// 023
	UInt32							unk024;				// 024
	TileImage*						pkCursor;			// 028
	Float32							flt02C;				// 02C
	Float32							flt030;				// 030
	Float32							flt034;				// 034
	Float32							fCursorX;			// 038
	Float32							flt03C;				// 03C
	Float32							fCursorY;			// 040
	Float32							fMouseWheel;		// 044	-120.0 = down; 120.0 = up
	Float32							fTimeLeftClickHeld;	// 048 time in menus only
	Tile*							pkDraggedTile;		// 04C
	SInt32							dragStartX;			// 050
	Float32							dragOldX;			// 054
	Float32							dragOldY;			// 058
	SInt32							dragStartY;			// 05C
	Float32							dragNewX;			// 060
	Float32							dragNewY;			// 064
	UInt32							unk068[2];			// 068
	BSSimpleList<TESObjectREFR*>	kSelectableRefs;	// 070
	UInt32							uiSelectedRefIndex;	// 078
	bool							bDebugText;			// 07C
	UInt8							bMouseVisible;		// 07D
	UInt8							byte07E;			// 07E
	UInt8							byte07F;			// 07F
	NiNode*							pkMainRootNode;		// 080
	NiNode*							pkCursorRootNode;	// 084
	UInt32							unk088;				// 088
	NiPointer<BSShaderAccumulator>	spTileAccumulator;	// 08C
	NiPointer<BSShaderAccumulator>	sp3DAccumulator;	// 090
	ShadowSceneNode*				shadowScene094;		// 094
	ShadowSceneNode*				shadowScene098;		// 098
	Tile*							pkMenuRoot;			// 09C
	Tile*							pkGlobalsTile;		// 0A0	globals.xml
	NiNode*							pDebugTextRoot;		// 0A4
	UInt32							unk0A8;				// 0A8
	NiPointer<NiAlphaProperty>		spkAlphaProperty;	// 0AC
	UInt8							byte0B0;
	UInt8							byte0B1;
	UInt16							wrd0B2;
	UInt32							unk0B4;
	Tile*							safeZone;
	Tile*							pActiveTileAlt;		// 0BC
	UInt32							unk0C0;				// 0C0
	UInt32							unk0C4;				// 0C4
	bool							bShouldHideAllPipboyMenus;			// 0C8
	UInt8							byte0C9;			// 0C9
	UInt8							byte0CA;			// 0CA
	UInt8							byte0CB;			// 0CB
	Tile*							pkActiveTile;		// 0CC
	Menu*							pkActiveMenu;		// 0D0
	Tile*							pkTile0D4;			// 0D4
	Menu*							pkMenu0D8;			// 0D8
	UInt8							unk0DC;
	UInt8							IsFullHelp;
	UInt16							unk0DE;
	UInt32							unk0E0;
	SInt8							cMsgBoxButton;		// 0E4 -1 if no button pressed
	UInt8							byte0E5;			// 0E5
	UInt8							byte0E6;			// 0E6
	UInt8							byte0E7;			// 0E7
	UInt32							unk0E8;				// 0E8
	bool							bIsEmergencyTextureRelease; // 0EC
	bool							bHasMouseMoved;		// 0ED
	UInt8							byte0EE;			// 0EE
	UInt8							byte0EF;			// 0EF
	TESObjectREFR*					pkConsoleSelection;	// 0F0	compared to activated object during Activate
	UInt32							unk0F4;				// 0F4
	UInt32							unk0F8;				// 0F8
	TESObjectREFR*					pkCrosshairRef;		// 0FC
	TESObjectREFR*					pkTelekinesisTarget;
	NiPoint3						pt104;
	UInt8							byte110;			// 
	UInt8							pad111[3];			// 111
	UInt32							menuStack[10];		// 114
	PlayerCameraSphere*				pkPlayerCameraSphere;	// 13C ?
	UInt32							unk140;				// 140
	UInt32							unk144;				// 144
	UInt8							byte148;			// 148
	bool							bIsShift;			// 149
	UInt8							byte14A;			// 14A
	UInt8							byte14B;			// 14B
	KeyModifier						eKeyModifiers;		// 14C
	UInt32							uiCurrentKey;		// 150
	UInt32							uiKeyRepeatStartTime;	// 154
	UInt32							uiLastKeyRepeatTime;	// 158
	SpecialInputCode				eCurrentControllerKey; // 15C
	UInt32							uiNextControllerRepeatTime; // 160
	MenuFade*						pkMenuFades;		// 164
	bool							bIsRenderedMenuSet;	// 168
	FORenderedMenu*					pkRenderedMenu;		// 16C
	bool							bIsMovedMouseInRenderedMenu; // 170
	UInt8							byte171;			// 171
	UInt8							byte172;			// 172
	UInt8							byte173;			// 173
	FOPipboyManager*				pkPipboyManager;	// 174
	Struct0178						unk178;				// 178
	VATSHighlightData				kVATSHighlightData;	// 1DC
	Float32							fMenuScaledCursorPosX; // 4AC
	Float32							fMenuScaledCursorPosY; // 4B0
	bool							bIsRenderedMenuOrPipboyManager; // 4B4
	UInt8							byte4B5;			// 4B5
	UInt8							byte4B6;			// 4B6
	UInt8							byte4B7;			// 4B7
	UInt32							uiQueuedPipboyTabToSwitchTo; // 4B8
	UInt32							uiPipBoyMode;			// 4BC
	void							(*pfnOnPipboyOpenCallback)();	// 4C0
	BSSimpleList<UInt32>			list4C4;			// 4C4
	bool							bIsDestroyAllMenus;	// 4CC
	bool							bPreventLevelUpTillContainerOpened;	// 4CD
	UInt8							pad4CE;				// 4CE
	UInt8							pad4CF;				// 4CF
	Float32							fScale4D0;			// 4D0
	Tutorials						kHelp;				// 4D4

	void ClosePipboy();

	static __forceinline InterfaceManager* GetSingleton() { return *(InterfaceManager**)(0x11D8A80); };;
	static __forceinline VATSHighlightData* GetVATSHighlightData() {
		if (!GetSingleton() || !GetSingleton()->eFlags) return nullptr;
		return &GetSingleton()->kVATSHighlightData;
	};

	static __forceinline bool IsDialogMenuOpen() { return *(bool*)0x11D9514; };

	static __forceinline bool GetPipboyModeIs2() {
		return GetSingleton()->uiPipBoyMode == 2;
	};

	static __forceinline bool GetPipboyModeIs3() {
		return GetSingleton()->uiPipBoyMode == 3;
	};

	static __forceinline bool IsPipboyMode2or3() {
		if (!GetSingleton() || !GetSingleton()->eFlags) return false;
		return GetPipboyModeIs2() || GetPipboyModeIs3();
	};

	static __forceinline bool IsRenderedMenuOrPipboyOpen() {
		if (IsPipboyMode2or3()) {
			if(!GetPipboyManager()->bIsInitialized)
				return false;
		}
		return ThisStdCall(0x7079F0, GetSingleton());
	}

	static bool GetIsDebugText() {
		InterfaceManager* interfaceManager = GetSingleton();
		if (!interfaceManager || !&interfaceManager) { return false; }
		return interfaceManager->bDebugText;
	}

	static bool IsPipBoyOpen() {
		return StdCall<bool>(0x7178A0);
	}

	static FORenderedMenu* GetRenderedMenuOrPipboyManager() {
		InterfaceManager* interfaceManager = GetSingleton();
		if (IsPipBoyOpen()) {
			return (FORenderedMenu*) interfaceManager->pkPipboyManager;
		}
		else {
			return interfaceManager->pkRenderedMenu;
		}
	}

	static FOPipboyManager* GetPipboyManager() {
		InterfaceManager* interfaceManager = GetSingleton();
		if (!interfaceManager || !&interfaceManager) { return nullptr; }
		return interfaceManager->pkPipboyManager;
	}

	static bool	 IsMenuVisible(UInt32 menuType);
	static Menu* GetMenuByType(UInt32 menuType);
	static Menu* TempMenuByType(UInt32 menuType);
	static TileMenu* GetMenuByPath(const char* componentPath, const char** slashPos);
	static Tile* GetMenuComponentTile(const char* componentPath);

	UInt32						GetTopVisibleMenuID();
	Tile*						GetActiveTile();

	char GetMessageBoxResult()
	{
		auto button = this->cMsgBoxButton;
		this->cMsgBoxButton = -1;
		return button;
	};
};
static_assert(sizeof(InterfaceManager) == 0x580);
static_assert(sizeof(InterfaceManager::Struct0178) == 0x64);