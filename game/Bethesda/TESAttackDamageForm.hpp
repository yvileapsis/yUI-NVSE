#pragma once

#include "BaseFormComponent.hpp"

class TESAttackDamageForm : public BaseFormComponent {
public:
	TESAttackDamageForm();
	~TESAttackDamageForm();

	virtual UInt16	GetDamage(void);

	UInt16	usAttackDamage;	// 04
	UInt16	unk0;	// 06 - bitmask? perhaps 2 UInt8s?
	// 008
};