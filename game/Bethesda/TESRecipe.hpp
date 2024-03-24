#pragma once

#include "TESForm.hpp"
#include "TESCondition.hpp"
#include "TESRecipeCategory.hpp"

struct ComponentEntry {
	UInt32		quantity;
	TESForm* item;
};

class TESRecipe : public TESForm {
public:
	TESRecipe();
	~TESRecipe();

	TESFullName				fullName;		// 18

	UInt32					reqSkill;		// 24
	UInt32					reqSkillLevel;	// 28
	UInt32					categoryID;		// 2C
	UInt32					subCategoryID;	// 30
	TESCondition	conditions;		// 34
	BSSimpleList<ComponentEntry*>	inputs;			// 3C
	BSSimpleList<ComponentEntry*>	outputs;		// 44
	UInt32					unk4C;			// 4C
	UInt32					unk50;			// 50
	TESRecipeCategory* category;		// 54
	TESRecipeCategory* subCategory;	// 58
};

ASSERT_SIZE(TESRecipe, 0x5C);