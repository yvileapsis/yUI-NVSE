#pragma once

#include "TESForm.hpp"
#include "TESIcon.hpp"

class BGSMenuIcon : public TESForm, public TESIcon {
public:
	BGSMenuIcon();
	virtual ~BGSMenuIcon();
};

ASSERT_SIZE(BGSMenuIcon, 0x24);