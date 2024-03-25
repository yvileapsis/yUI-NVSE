#pragma once
#include "Menu.hpp"


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

	TileImage*	pkTile028;		// 028	RM_Items_LeftFilterArrow
	TileText* itemsTitle;	// 02C	RM_ItemsTitle
	TileImage*	pkTile030;		// 030	RM_Items_RightFilterArrow
	TileImage*	pkTile034;		// 034	RM_Items_InventoryList
	TileText*	pkTile038;		// 038	RM_MadeAtVariable
	TileText*	pkTile03C;		// 03C	RM_SkillRequirement
	TileImage*	pkTile040;		// 040	RM_Items_IngredientList
	TileImage*	pkTileAccept;		// 044	RM_ButtonX
	TileImage*	pkTile048;		// 048	RM_ButtonB
	TileImage*	pkTile04C;		// 04C	RM_ItemIcon
	TileRect*	pkTile050;		// 050	RM_ItemData
	TileText*	pkTile054;		// 054	RM_Items_IngredientList
	TileText*	pkTile058;		// 058	RM_ContainerTitle
	TileText*	pkTile05C;		// 05C	RM_SkillRequirementHeader
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