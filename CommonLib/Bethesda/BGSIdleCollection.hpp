#pragma once

#include "BaseFormComponent.hpp"

class TESIdleForm;

class BGSIdleCollection : public BaseFormComponent {
public:
	UInt8			ucIdleFlags;
	UInt8			ucIdleCount;
	TESIdleForm**	ppIdles;
	float			fTimerCheckForIdle;
};

ASSERT_SIZE(BGSIdleCollection, 0x10);