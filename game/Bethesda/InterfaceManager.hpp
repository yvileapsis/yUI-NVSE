#pragma once

#include "BGSMenuPacker.hpp"
#include "BSGraphics.hpp"
#include "BSRenderedTexture.hpp"
#include "FOPipboyManager.hpp"
#include "Menu.hpp"
#include "TESObjectREFR.hpp"
#include "Tile.hpp"

class TileMenu;
class TileImage;
class SceneGraph;
class ImageSpaceShaderParam;

enum SpecialInputCode {
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

class InterfaceManager {
public:
	InterfaceManager();
	~InterfaceManager();

	struct Struct0178 {
		NiRefObjectPtr			spUnk0;
		BGSMenuPacker			kPacker;
		UInt8					byte28;
		UInt8					byte29;
		UInt8					byte2A;
		UInt8					byte2B;
		float					fStartTime;
		NiPoint2				kDuration;
		NiPoint2				kIntensity;
		NiPoint2				kFrequency;
		NiPoint2				kUnk48;
		ImageSpaceShaderParam*	pImageSpaceEffectParam;
		bool					bIsFlyCamEnabled;
		UInt8					byte55;
		UInt8					byte56;
		UInt8					byte57;
		float					fBlurRadiusHUD;
		float					fScanlineFrequencyHUD;
		float					fBlurIntensityHUD;
	};

	struct TargetData {
		TESObjectREFR*	pRef;
		NiNodePtr		spNode;
	};

	struct VATSHighlightData {
		enum QueryType {
			LIMB_QUERY_NONE		= 0,
			LIMB_QUERY_FULL		= 1,
			LIMB_QUERY_PARTIAL	= 2,
		};

		UInt32							eMode;
		TargetData						kTarget;
		UInt32							uiNumHighlightedRefs;
		UInt32							uiFlashingRefIndex;
		TargetData						kHighlightedRefs[32];
		NiRefObjectPtr					spUnk114;
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
		float							time288;
		float							fDistortVertical;
		float							fDistortDuration;
		float							fVATSDistortTime;
		bool							byte298;
		float							time29C;
		float							fBurstDuration;
		float							fBurstIntensity2;
		float							fBurstIntensity;
		float							fPulseIntensity;
		float							fPulseRadius;
		bool							byte2B4;
		bool							byte2B5;
		float							fUnk2B8;
		float							fUnk2BC;
		float							fVATSTargetPulseRate;
		NiTexturePtr					spScanlineTexture;
		UInt32							uiVisibleAreaScale;
		BSShaderAccumulatorPtr			spAccumulator;

		static BSRenderedTexture* GetCurrentHighlightedNodeTexture() {
			return (*(BSRenderedTexturePtr*)0x11DEB38);
		}

		static void SetCurrentHighlightedNodeTexture(BSRenderedTexture* apTexture) {
			(*(BSRenderedTexturePtr*)0x11DEB38) = apTexture;
		}

		void HandleOcclusionQueries(BSRenderedTexture* apCurrentNodeTexture, QueryType aeQueryType, bool a4, int a5, NiRenderer::ClearFlags uiClearMode, bool abEndFrame);

		NiNode* GetTargetNode() const {
			return kTarget.spNode.m_pObject;
		}

		void AddRef(TESObjectREFR* ref);
		void AddRefAndSetFlashing(TESObjectREFR* ref);
		void ResetRefs();
	};

	struct Tutorials {
		SInt32 tutorialFlags[41];
		UInt32 currentShownHelpID;
		UInt32 timeA8;
	};

	struct MenuFade {
		Menu*		pMenu;
		float		fFadeTimer;
		float		fFadeDuration;
		MenuFade*	pPrev;
		MenuFade*	pNext;
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

	Bitfield32						uiFlags;
	NiPointer<SceneGraph>			spMenuRoot;
	NiPointer<SceneGraph>			spMenu3DRoot;
	UInt32							uiCurrentMode;
	// checked for 1 at 0x70BA97
	// set to 2 at 0x70BA8D
	// set to 3 at 0x714E96, 0x714E20 - checked at 0x70B94E
	// set to 4 at 0x714D5D, 0x715177 (CloseAllMenus)
	// set to 5 at 0x70B972 - checked at 0x70BA84, 0x70CA14, 0x70CC7A
	UInt32							unk010;
	UInt32							unk014;
	UInt32							uiPickLength;
	UInt32							unk01C;
	UInt8							byte020;
	UInt8							byte021;
	UInt8							byte022;
	UInt8							byte023;
	UInt32							unk024;	
	TileImage*						pCursor;
	float							flt02C;	
	float							flt030;	
	float							flt034;	
	float							fCursorX;
	float							flt03C;
	float							fCursorY;
	float							fMouseWheel;		// -120.0 = down; 120.0 = up
	float							fTimeLeftClickHeld;	// Menus only
	Tile*							pDraggedTile;
	int								unk050;
	float							flt054;
	float							flt058;
	int								unk05C;
	int								unk060;
	int								unk064;
	UInt32							unk068[2];
	BSSimpleList<TESObjectREFR*>	kSelectableRefs;
	UInt32							uiSelectedRefIndex;
	bool							bDebugText;
	bool							bMouseVisible;
	UInt8							byte07E;
	UInt8							byte07F;
	NiNode*							pNode080;
	NiNode*							pCursorRootNode;
	UInt32							unk088;
	NiPointer<BSShaderAccumulator>	spTileAccumulator;
	NiPointer<BSShaderAccumulator>	sp3DAccumulator;
	ShadowSceneNode*				pObject3DScene;
	ShadowSceneNode*				pPlayer3DScene;
	Tile*							pMenuRoot;
	Tile*							pGlobalsTile;
	NiNode*							pDebugTextRoot;
	UInt32							unk0A8;
	NiPointer<NiAlphaProperty>		spAlphaProperty;
	UInt32							unk0B0[3];
	Tile*							pActiveTileAlt;
	UInt32							unk0C0;
	UInt32							unk0C4;
	UInt8							byte0C8;
	UInt8							byte0C9;
	UInt8							byte0CA;
	UInt8							byte0CB;
	Tile*							pActiveTile;
	Menu*							pActiveMenu;
	Tile*							pTile0D4;
	Menu*							pMenu0D8;
	UInt32							unk0DC[2];
	SInt8							msgBoxButton;		// -1 if no button pressed
	UInt8							byte0E5;
	UInt8							byte0E6;
	UInt8							byte0E7;
	UInt32							unk0E8;
	bool							bIsEmergencyTextureRelease;
	bool							bHasMouseMoved;
	UInt8							byte0EE;
	UInt8							byte0EF;
	TESObjectREFR*					pConsoleSelection;
	UInt32							unk0F4;
	UInt32							unk0F8;
	TESObjectREFR*					pCrosshairRef;
	UInt32							unk100;
	NiPoint3						unk104;
	UInt8							byte110;
	UInt32							menuStack[10];
	void*							pPlayerCameraSphere;
	UInt32							unk140;
	UInt32							unk144;
	UInt8							byte148;
	bool							bIsShift;
	UInt8							byte14A;
	UInt8							byte14B;
	KeyModifier						eKeyModifiers;
	UInt32							uiCurrentKey;
	UInt32							uiKeyRepeatStartTime;
	UInt32							uiLastKeyRepeatTime;
	SpecialInputCode				eCurrentControllerKey;
	UInt32							uiNextControllerRepeatTime;
	MenuFade*						pMenuFades;
	bool							bIsRenderedMenuSet;
	FORenderedMenu*					pRenderedMenu;
	UInt8							byte170;
	UInt8							byte171;
	UInt8							byte172;
	UInt8							byte173;
	FOPipboyManager*				pPipboyManager;
	Struct0178						unk178;
	VATSHighlightData				kVATSHighlightData;
	float							fScale4AC;
	float							fUnk4B0;
	UInt8							bIsRenderedMenuOrPipboyManager;
	UInt8							byte4B5;
	UInt8							byte4B6;
	UInt8							byte4B7;
	UInt32							queuedPipboyTabToSwitchTo;
	UInt32							uiPipBoyMode;
	void							(*onPipboyOpenCallback)(void);
	BSSimpleList<void*>				kUnk4C4;
	UInt8							byte4CC;
	UInt8							byte4CD;
	UInt8							pad4CE;
	UInt8							pad4CF;
	float							fScale4D0;
	Tutorials						kHelp;

	char GetMessageBoxResult() {
		auto button = msgBoxButton;
		msgBoxButton = -1;
		return button;
	};

	void ClosePipboy();

	static __forceinline InterfaceManager* GetSingleton() { return *(InterfaceManager**)(0x11D8A80); };
	static __forceinline VATSHighlightData* GetVATSHighlightData() {
		if (!GetSingleton() || !GetSingleton()->uiFlags.Get())
			return nullptr;
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
		if (!GetSingleton() || !GetSingleton()->uiFlags.Get())
			return false;
		return GetPipboyModeIs2() || GetPipboyModeIs3();
	};

	static __forceinline bool IsRenderedMenuOrPipboyOpen() {
		if (IsPipboyMode2or3() && !GetPipboyManager()->bIsInitialized) {
			return false;
		}
		return ThisStdCall(0x7079F0, GetSingleton());
	}

	static bool GetIsDebugText() {
		InterfaceManager* interfaceManager = GetSingleton();
		if (!interfaceManager || !&interfaceManager) 
			return false;
		return interfaceManager->bDebugText;
	}

	static bool IsPipBoyOpen() {
		return StdCall<bool>(0x7178A0);
	}

	static FORenderedMenu* GetRenderedMenuOrPipboyManager() {
		InterfaceManager* interfaceManager = GetSingleton();
		return IsPipBoyOpen() ? interfaceManager->pPipboyManager : interfaceManager->pRenderedMenu;
	}

	static FOPipboyManager* GetPipboyManager() {
		InterfaceManager* interfaceManager = GetSingleton();
		if (!interfaceManager || !&interfaceManager) 
			return nullptr;
		return interfaceManager->pPipboyManager;
	}

	static bool	 IsMenuVisible(UInt32 menuType);
	static Menu* GetMenuByType(UInt32 menuType);
	static Menu* TempMenuByType(UInt32 menuType);
	static TileMenu* GetMenuByPath(const char* componentPath, const char** slashPos);
	static Tile::Value* GetMenuComponentValue(const char* componentPath);
	static Tile* GetMenuComponentTile(const char* componentPath);
	static Tile::Value* GetMenuComponentValueAlt(const char* componentPath);
};

ASSERT_SIZE(InterfaceManager, 0x580);
ASSERT_SIZE(InterfaceManager::Struct0178, 0x64);