#pragma once
#include "TESForm.hpp"
#include "TESIcon.hpp"

// 24
class BGSMenuIcon :
	public TESForm,	// 00
	public TESIcon	// 18
{
public:
	BGSMenuIcon();
	virtual ~BGSMenuIcon();
};
static_assert(sizeof(BGSMenuIcon) == 0x24);