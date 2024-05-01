#pragma once
#include "Menu.hpp"
#include "BSSoundHandle.hpp"

class Tile3D;
class TileText;
class TileImage;
class TileRect;
class TESLoadScreen;
class TESWorldSpace;
class NiTexture;
class BSShaderProperty;

class LoadingMenu : public Menu
{
public:
	LoadingMenu();
	~LoadingMenu();
	enum {
		LOADING_NIF = 0,
		LOADING_PLATENO_TEXT_2 = 1,
		LOADING_INFO_TEXT = 2,
		LOADING_CURROBJ_TEXT = 3,
		LOADING_STAT1_LABEL = 4,
		LOADING_STAT1_VAL = 5,
		LOADING_STAT2_LABEL = 6,
		LOADING_STAT2_VAL = 7,
		LOADING_STAT3_LABEL = 8,
		LOADING_STAT3_VAL = 9,
		LOADING_LEVELPROG_METER = 10,
		LOADING_PINWHEEL = 11,
		LOADING_WORLD_IMAGE = 12,
		LOADING_PLATENO_TEXT_1 = 13,
		LOADING_CURROBJ_LABEL = 14,
		LOADING_LEVELPROG_LABEL,
		LOADING_LEVELPROG_RECT,
		LOADING_STAT_LABEL_RECT,
		LOADING_STAT_VAL_RECT,
		LOADING_STAT4_LABEL,
		LOADING_STAT4_VAL,
		LOADING_STAT5_LABEL,
		LOADING_STAT5_VAL,
		LOADING_STAT6_LABEL,
		LOADING_STAT6_VAL,
		LOADING_STAT7_LABEL,
		LOADING_STAT7_VAL,
		LOADING_CURROBJ_RECT,
		LOADING_LEVELPROG_CURLEVEL,
		LOADING_LEVELPROG_NEXTLEVEL,
		LOADING_LEVELPROG_CHAR_MARKER,
		LOADING_STAT_RECT,
		LOADING_TILE_SLIDE_01,
		LOADING_TILE_SLIDE_02,
		HIDDEN_TIP_TEXT_01,
		TIP_TEXT_02,
		TIP_MATTE_01,
		TIP_MATTE_02,
		TILE_COUNT
	};

	enum Flags {
		IsNotInitialGameLoad = 0,
		IsForwardSequence = 1,
		maybeSomethingAudio = 2,
		Unk_3 = 3,
		Unk_4 = 4,
		IsMainMenu = 5,
		ShowScreenTipsStatsEtc = 6,
		Unk_7 = 7,
		Unk_8 = 8,
		IsInterior = 9,
		IsEnableLoadingMenuCommand = 10,
	};


	enum TipFlags {
		SETUP_TIP_1 = 1 << 0,
		SETUP_TIP_2 = 1 << 1,





	};

	struct TileData {
		UInt32	x;
		UInt32	y;
		UInt32	uiHeight;
		UInt32	uiWidth;
		UInt32	font;
		UInt8	justify;
	};

	union {
		struct {
			Tile3D* pLoading_nif;
			TileText* pLoading_plateno_text2;
			TileText* pLoading_info_text;
			TileText* pLoading_currobj_text;
			TileText* pLoading_stat1_label;
			TileText* pLoading_stat1_val;
			TileText* pLoading_stat2_label;
			TileText* pLoading_stat2_val;
			TileText* pLoading_stat3_label;
			TileText* pLoading_stat3_val;
			TileImage* pLoading_levelprog_meter;
			Tile3D* pLoading_pinwheel;
			TileImage* pLoading_world_image;
			TileText* pLoading_plateno_text1;
			TileText* pLoading_currobj_label;
			TileText* pLoading_levelprog_label;
			TileRect* pLoading_levelprog_rect;
			TileText* pLoading_stat_label_rect;
			TileRect* pLoading_stat_val_rect;
			TileText* pLoading_stat4_label;
			TileText* pLoading_stat4_val;
			TileText* pLoading_stat5_label;
			TileText* pLoading_stat5_val;
			TileText* pLoading_stat6_label;
			TileText* pLoading_stat6_val;
			TileText* pLoading_stat7_label;
			TileText* pLoading_stat7_val;
			TileRect* pLoading_currobj_rect;
			TileText* pLoading_levelprog_curlevel;
			TileText* pLoading_levelprog_nextlevel;
			TileText* pLoading_levelprog_char_marker;
			TileRect* pLoading_stat_rect;
			TileImage* pLoading_tile_slide_01;
			TileImage* pLoading_tile_slide_02;
			TileText* pHidden_tip_text_01;
			TileText* pTip_text_02;
			TileImage* pTip_matte_01;
			TileImage* pTip_matte_02;
		};
		Tile* pTiles[TILE_COUNT];
	};

	UInt32							iMinX;
	UInt32							iMinY;
	UInt32							iMaxX;
	UInt32							iMaxY;
	NiTList<Tile*>					kStatTiles;
	NiTList<Tile*>					kCurrObjTiles;
	NiTList<Tile*>					kLevelProgTiles;
	NiTList<Tile*>					kHiddenTipTiles;
	NiTList<Tile*>					kTipTiles;
	UInt32							uiMiscStatArray[43];
	BSStringT<char>					strScreenTexture;
	NiColorA						kBackgroundColor;
	BSSimpleList<TESLoadScreen*>	kLoadScreens;
	BSSimpleList<TESLoadScreen*>* pLoadScreenIter;
	TESLoadScreen* pCurrentLoadScreen;
	UInt32							uiNextLoadScreenUpdateTime;
	UInt32							uiTime1E4;
	TESWorldSpace* pWorldspace;
	BSShaderProperty* pShaderProperty;
	BSShaderProperty* pSlide02ShaderProperty;
	BSSoundHandle					hSound1F0;
	BSSoundHandle					hSound1FC;
	NiPointer<NiTexture>			spSrcTexture[4];
	UInt32							uiCurrTextureIndex;
	UInt16							usCurrMiscStatID;
	UInt16							usFlags;
	TileData						kTileDatas[38];
	UInt32							uiTime5B4;
	UInt32							uiTipChanceTimer; // % 2 to show Tip 2 or 1
	Bitfield32						uiTipFlags;

	void SetFlag(UInt32 uiFlag, bool abSet);

	struct FO3LoadingScreenData {
		static std::string strCurrentStatText;
	};

	static LoadingMenu* GetSingleton() { return *reinterpret_cast<LoadingMenu**>(0x11DA0C0); };

	static void __fastcall SetShowTipsStats(LoadingMenu* apThis, void*, bool abShow);
	void SetStatText(UInt32 auiStat);
	static void __fastcall UpdateLevelProgressLabels(LoadingMenu* apThis);
};
static_assert(sizeof(LoadingMenu) == 0x5C0);