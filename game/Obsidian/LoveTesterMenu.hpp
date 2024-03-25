#pragma once
#include "Menu.hpp"

// 0x230
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
	enum EnumPages : UInt32
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
	union
	{
		Tile* pkTiles[8];
		struct
		{
			TileImage*	pkTile028;
			TileImage*	pkTile02C;
			TileImage*	pkTile030;
			TileImage*	pkTile034;
			TileImage*	pkTile038;
			TileImage*	pkTile03C;
			TileImage*	pkTile040;
			TileImage*	pkTile044;
		};
	};
	EnumPages			eCurrentPageNumber;
	UInt32				uiFinalPageSelectedSpecialIndex; // used with a controller connected
	Float32				time050;
	UInt8				bIsNotInChangePageAnim; // used to prevent switching pages while the anim plays
	UInt8				pad055;
	UInt8				pad056;
	UInt8				pad057;
	UInt32				uiTotalPointsToAllocate;
	BSFadeNode*			pkNV_VitoMaticVigorTester_Activate;
	BSFadeNode*			pkNV_VitoMaticVigorTester_Cabinet;
	LoveTesterPage		unkPage;
	LoveTesterPage		kVigorTesterPages[9];
	LoveTesterPage		unkPage2;
	NiSourceTexture*	pkBBNumbers[11];
	NiSourceTexture*	pkBBMessages[4];
	NiSourceTexture*	pkBBRTOff;
	NiSourceTexture*	pkBBRTOn;
	NiSourceTexture*	pkBBLTOff;
	NiSourceTexture*	pkBBLTOn;
	NiSourceTexture*	pkBBXOff;
	NiSourceTexture*	pkBBXOn;
	NiSourceTexture*	pkBBDecreaseOff;
	NiSourceTexture*	pkBBDecreaseOn;
	NiSourceTexture*	pkBBIncreaseOff;
	NiSourceTexture*	pkBBIncreaseOn;
	NiSourceTexture*	pkFilamentGlowOFF;
	NiSourceTexture*	pkFilamentGlowON;

	static LoveTesterMenu* GetSingleton() { return *reinterpret_cast<LoveTesterMenu**>(0x11DA2C8); };
};
static_assert(sizeof(LoveTesterMenu) == 0x230);