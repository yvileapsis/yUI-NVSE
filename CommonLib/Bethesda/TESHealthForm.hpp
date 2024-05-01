#pragma once
#include "BaseFormComponent.hpp"

// 0x8
class TESHealthForm : public BaseFormComponent {
public:
	TESHealthForm();
	virtual ~TESHealthForm();
	virtual UInt32	GetHealth();	// 04

	UInt32	uiHealth;	// 04
};
static_assert(sizeof(TESHealthForm) == 0x8);