#pragma once

#include "TESForm.hpp"
#include "BGSPreloadable.hpp"

class BGSDebris : public TESForm, public BGSPreloadable {
	BGSDebris();
	virtual ~BGSDebris();

	BSSimpleList<void*> kUnkList;
};

ASSERT_SIZE(BGSDebris, 0x24);