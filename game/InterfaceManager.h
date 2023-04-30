#pragma once
#include <NiTypes.h>
#include <Containers.h>

// can be passed to QueueUIMessage to determine Vaultboy icon displayed
enum eEmotion {
	happy = 0,
	sad = 1,
	neutral = 2,
	pain = 3
};

class ButtonIcon;

typedef void (*_ShowMessageBox_Callback)();
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

class bhkSimpleShapePhantom;

struct PlayerCameraSphere
{
	bhkSimpleShapePhantom* unk000;
	bhkSimpleShapePhantom* unk004;
};


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
	UInt8					byte010;
	UInt8					byte011;
	UInt8					statsMenuTab;
	UInt8					inventoryMenuTab;
	UInt8					byte14;
	UInt8					mapMenuTab;
	UInt8					gap16[2];
	UInt32					pickLength;			// 018
	UInt32					unk01C;				// 01C
	UInt8					byte020;			// 020
	UInt8					byte021;			// 021
	UInt8					byte022;			// 022
	UInt8					byte023;			// 023
	UInt32					unk024;				// 024
	TileImage*				cursor;				// 028
	Float32					flt02C;				// 02C
	Float32					flt030;				// 030
	Float32					flt034;				// 034
	Float32					cursorX;			// 038
	Float32					flt03C;				// 03C
	Float32					cursorY;			// 040
	Float32					mouseWheel;			// 044	-120.0 = down; 120.0 = up
	Float32					timeLeftClickHeld;	// 048 time in menus only
	Tile*					draggedTile;		// 04C
	SInt32					dragStartX;			// 050
	Float32					dragOldX;				// 054
	Float32					dragOldY;				// 058
	SInt32					dragStartY;			// 05C
	Float32					dragNewX;				// 060
	Float32					dragNewY;				// 064
	UInt32					unk068;				// 068
	UInt32					unk06C;				// 06C
	TList<TESObjectREFR>	selectableRefs;		// 070
	UInt32					selectedRefIndex;	// 078
	bool					debugText;			// 07C
	UInt8					isMouseVisible;		// 07D
	UInt8					byte07E;			// 07E
	UInt8					byte07F;			// 07F
	NiNode*					mainRootNode;			// 080
	NiNode*					cursorRootNode;			// 084
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
	UInt8					byte0B0;
	UInt8					byte0B1;
	UInt16					wrd0B2;
	UInt32					unk0B4;
	Tile*					safeZone;
	Tile*					activeTileAlt;		// 0BC
	UInt32					unk0C0;				// 0C0
	UInt32					unk0C4;				// 0C4
	UInt8					shouldHideAllPipboyMenus;			// 0C8
	UInt8					byte0C9;			// 0C9
	UInt8					byte0CA;			// 0CA
	UInt8					byte0CB;			// 0CB
	Tile*					activeTile;			// 0CC
	Menu*					activeMenu;			// 0D0
	Tile*					tile0D4;			// 0D4
	Menu*					menu0D8;			// 0D8
	UInt8					unk0DC;
	UInt8					IsFullHelp;
	UInt16					unk0DE;
	UInt32					unk0E0;
	UInt8					msgBoxButton;		// 0E4 -1 if no button pressed
	UInt8					byte0E5;			// 0E5
	UInt8					byte0E6;			// 0E6
	UInt8					byte0E7;			// 0E7
	UInt32					unk0E8;				// 0E8
	UInt8					isEmergencyTextureRelease;			// 0EC
	UInt8					hasMouseMoved;		// 0ED
	UInt8					byte0EE;			// 0EE
	UInt8					byte0EF;			// 0EF
	TESObjectREFR*			debugSelection;		// 0F0	compared to activated object during Activate
	UInt32					unk0F4;				// 0F4
	UInt32					unk0F8;				// 0F8
	TESObjectREFR*			crosshairRef;		// 0FC
	TESObjectREFR*			telekinesisTarget;
	NiPoint3				pt104;
	UInt8					byte110;			// 110
	UInt8					pad111[3];			// 111
	UInt32					menuStack[10];		// 114
	PlayerCameraSphere*		ptr13C;				// 13C	Points to a struct, possibly. First member is *bhkSimpleShapePhantom
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
	UInt32					unk15C;
	UInt32					time160;
	UInt32					ptr164;
	UInt8					isRenderedMenuSet;
	UInt8					gap169[3];
	FORenderedMenu*			renderedMenu;		// 16C
	UInt8					isMovedMouseInRenderedMenu;			// 170
	UInt8					byte171;			// 171
	UInt8					byte172;			// 172
	UInt8					byte173;			// 173
	FOPipboyManager*		pipboyManager;		// 174
	Struct0178				unk178;				// 178
	VATSHighlightData		vatsHighlightData;	// 1DC
	Float32					menuScaledCursorPosX;			// 4AC
	Float32					menuScaledCursorPosY;				// 4B0
	UInt8					isRenderedMenuOrPipboyManager;		// 4B4
	UInt8					byte4B5;			// 4B5
	UInt8					byte4B6;			// 4B6
	UInt8					byte4B7;			// 4B7
	UInt32					queuedPipboyTabToSwitchTo;			// 4B8
	UInt32					pipBoyMode;			// 4BC
	void					(*onPipboyOpenCallback)();		// 4C0
	TList<void>				list4C4;			// 4C4
	UInt8					isDestroyAllMenus;			// 4CC
	UInt8					preventLevelUpTillContainerOpened;			// 4CD
	UInt8					pad4CE;				// 4CE
	UInt8					pad4CF;				// 4CF
	Float32					unk4D0;				// 4D0
	Tutorials				help;				// 4D4

	InterfaceManager();
	~InterfaceManager();

	static InterfaceManager*	GetSingleton() { return *reinterpret_cast<InterfaceManager**>(0x11D8A80); };

	UInt32						GetTopVisibleMenuID();
	Tile*						GetActiveTile();

	char GetMessageBoxResult()
	{
		auto button = this->msgBoxButton;
		this->msgBoxButton = -1;
		return button;
	};
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


	// 3928
	struct FontData
	{
		struct TexFileName
		{
			UInt32			unk00;
			char			fileName[0x20];
		};

		struct CharDimensions
		{
			float			flt00;		// 0
			float			flt04;		// 4
			float			flt08;		// 8
			float			flt0C;		// C
			float			flt10;		// 10
			float			flt14;		// 14
			float			flt18;		// 18
			float			flt1C;		// 1C
			float			flt20;		// 20
			float			width;		// 24
			float			height;		// 28
			float			flt2C;		// 2C
			float			widthMod;	// 30
			float			flt34;		// 34
		//	totalWidth = width + widthMod
		};
		static_assert(sizeof(CharDimensions) == 0x38);

		float			lineHeight;				// 0000
		UInt32			numTextures;			// 0004
		TexFileName		textures[8];			// 0008
		CharDimensions	charDimensions[256];	// 0128
	};
	static_assert(sizeof(FontData) == 0x3928);

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

	UInt8						isLoaded;		// 000	Init'd to 0, loaded successfully in OBSE (word bool ?)
	UInt8						pad01[3];		// 001
	char*						filePath;		// 004	Init'd to arg2, passed to OpenBSFile
	UInt32						id;				// 008	1 based, up to 8 apparently
	NiObject*					unk00C[8];		// 00C	in OBSE: NiTexturingProperty			* textureProperty
	float						unk02C;			// 02C	Those two values seem to be computed by looping through the characters in the font (max height/weight ?)
	float						unk030;			// 030
	UInt32						unk034;			// 038	in OBSE: NiD3DShaderConstantMapEntry	* unk34;
	FontData*					fontData;		// 038	Init'd to 0, might be the font content, at Unk004 we have the count of font texture
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
	FontInfo*			extraFonts[80];		// 24

	NiPoint3* GetStringDimensions(NiPoint3* outDims, const char* srcString, UInt32 fontID, Float32 wrapwidth = 0x7F7FFFFF, UInt32 startIdx = 0) { return ThisCall<NiPoint3*>(0xA1B020, this, outDims, srcString, fontID, wrapwidth, startIdx); };
	NiPoint3* GetStringDimensions(const char* srcString, UInt32 fontID, Float32 wrapwidth = 0x7F7FFFFF, UInt32 startIdx = 0)
	{
		NiPoint3 out;
		return GetStringDimensions(&out, srcString, fontID, wrapwidth, startIdx);
	}

	std::string StringShorten(const std::string& str, const UInt32 font, const Float32 max) const;

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
	virtual void    Unk_00();
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