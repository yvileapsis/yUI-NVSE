#pragma once
#include "TESForm.hpp"
#include "TESCondition.hpp"
#include "TESRecipeCategory.hpp"

struct ComponentEntry {
	UInt32		uiQuantity;
	TESForm*	pkItem;
};

// 0x5C
class TESRecipe :
	public TESForm,
	public TESFullName
{
public:
	TESRecipe();
	~TESRecipe();

	UInt32				uiReqSkill;			// 24
	UInt32				uiReqSkillLevel;	// 28
	UInt32				uiCategoryID;		// 2C
	UInt32				uiSubCategoryID;	// 30
	TESCondition		kConditions;		// 34
	BSSimpleList<ComponentEntry*> kInputs;	// 3C
	BSSimpleList<ComponentEntry*> kOutputs;	// 44
	UInt32				unk4C;			// 4C
	UInt32				unk50;			// 50
	TESRecipeCategory*	pkCategory;		// 54
	TESRecipeCategory*	pkSubCategory;	// 58
};
static_assert(sizeof(TESRecipe) == 0x5C);