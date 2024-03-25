#pragma once
#include "TESForm.hpp"
#include "BGSPreloadable.hpp"

// 24
class BGSDebris :
	public TESForm,
	public BGSPreloadable
{
	BGSDebris();
	virtual ~BGSDebris();

	BSSimpleList<void*> kUnkList;
};
static_assert(sizeof(BGSDebris) == 0x24);