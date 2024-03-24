#pragma once

#include "TESForm.hpp"

class TESAmmoEffect : public TESForm {
public:
	TESAmmoEffect();
	~TESAmmoEffect();

	enum
	{
		kEffectType_DamageMod = 0,
		kEffectType_DRMod = 1,
		kEffectType_DTMod = 2,
		kEffectType_SpreadMod = 3,
		kEffectType_ConditionMod = 4,
		kEffectType_FatigueMod = 5,
	};

	enum
	{
		kOperation_Add = 0,
		kOperation_Multiply = 1,
		kOperation_Subtract = 2,
	};

	TESFullName		fullName;	// 018
	UInt32			type;		// 024
	UInt32			operation;	// 028
	float			value;		// 02C
};

ASSERT_SIZE(TESAmmoEffect, 0x30);