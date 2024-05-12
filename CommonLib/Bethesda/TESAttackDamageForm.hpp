#pragma once
#include "BaseFormComponent.hpp"

// 0x8
class TESAttackDamageForm : public BaseFormComponent
{
public:
	TESAttackDamageForm();

	virtual ~TESAttackDamageForm();
	virtual UInt16	GetDamage();

	UInt16	usAttackDamage;	// 04
	UInt16	unk0;			// 06 - bitmask? perhaps 2 UInt8s?
};
static_assert(sizeof(TESAttackDamageForm) == 0x8);