#include "GameTypes.h"
#include <InterfaceTiles.h>

enum
{
	kMenuType_Min = 0x3E9,
	kMenuType_Message = kMenuType_Min,
	kMenuType_Inventory,
	kMenuType_Stats,
	kMenuType_HUDMain,
	kMenuType_Loading = 0x3EF,
	kMenuType_Container,
	kMenuType_Dialog,
	kMenuType_SleepWait = 0x3F4,
	kMenuType_Start,
	kMenuType_LockPick,
	kMenuType_Quantity = 0x3F8,
	kMenuType_Map = 0x3FF,
	kMenuType_Book = 0x402,
	kMenuType_LevelUp,
	kMenuType_Repair = 0x40B,
	kMenuType_RaceSex,
	kMenuType_Credits = 0x417,
	kMenuType_CharGen,
	kMenuType_TextEdit = 0x41B,
	kMenuType_Barter = 0x41D,
	kMenuType_Surgery,
	kMenuType_Hacking,
	kMenuType_VATS,
	kMenuType_Computers,
	kMenuType_RepairServices,
	kMenuType_Tutorial,
	kMenuType_SpecialBook,
	kMenuType_ItemMod,
	kMenuType_LoveTester = 0x432,
	kMenuType_CompanionWheel,
	kMenuType_TraitSelect,
	kMenuType_Recipe,
	kMenuType_SlotMachine = 0x438,
	kMenuType_Blackjack,
	kMenuType_Roulette,
	kMenuType_Caravan,
	kMenuType_Trait = 0x43C,
	kMenuType_Max = kMenuType_Trait,
};

// can be passed to QueueUIMessage to determine Vaultboy icon displayed
enum eEmotion {
	happy = 0,
	sad = 1,
	neutral = 2,
	pain = 3
};

class ButtonIcon;

typedef void (*_ShowMessageBox_Callback)(void);
extern const _ShowMessageBox_Callback ShowMessageBox_Callback;

typedef bool (*_ShowMessageBox)(const char* message, UInt32 unk1, UInt32 unk2, _ShowMessageBox_Callback callback, UInt32 unk4, UInt32 unk5, float unk6, float unk7, ...);
extern const _ShowMessageBox ShowMessageBox;

// set to scriptObj->refID after calling ShowMessageBox()
// GetButtonPressed checks this before returning a value, if it doesn't match it returns -1
typedef UInt32* _ShowMessageBox_pScriptRefID;
extern const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID;
typedef UInt8* _ShowMessageBox_button;
extern const _ShowMessageBox_button ShowMessageBox_button;

typedef bool (*_QueueUIMessage)(const char* msg, UInt32 emotion, const char* ddsPath, const char* soundName, float msgTime, bool maybeNextToDisplay);
extern const _QueueUIMessage QueueUIMessage;

const UInt32 kMaxMessageLength = 0x4000;

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
	Tile*					activeTileAlt;		// 0BC
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

	static InterfaceManager*	GetSingleton() { return *reinterpret_cast<InterfaceManager**>(0x11D8A80); };
	static bool					IsMenuVisible(const UInt32 menuType);
	static Menu*				GetMenuByType(const UInt32 menuType);
	static Menu*				TempMenuByType(const UInt32 menuType);
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


	FontInfo* fontInfos[8];		// 00
	UInt8				byte20;				// 20
	UInt8				pad21[3];			// 21
	FontInfo* extraFonts[80];	// 24

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

	FontTextReplaced() : gap1D{}
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

struct FontHeightData
{
	float		heightBase;
	float		heightwGap;
};
//s_fontHeightDatas[90];
void Debug_DumpFontNames();