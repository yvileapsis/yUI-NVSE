#pragma once
#include "TESIcon.hpp"

// 0x10
class BGSMessageIcon : public BaseFormComponent
{
public:
	TESIcon	kIcon;

	BGSMessageIcon();
	~BGSMessageIcon();
};
static_assert(sizeof(BGSMessageIcon) == 0x10);