#pragma once
#include "TESForm.hpp"

// 0x30
class TESAmmoEffect : public TESForm 
{
public:
	TESAmmoEffect();
	~TESAmmoEffect();

	enum EnumType : UInt32
	{
		kEffectType_DamageMod = 0,
		kEffectType_DRMod = 1,
		kEffectType_DTMod = 2,
		kEffectType_SpreadMod = 3,
		kEffectType_ConditionMod = 4,
		kEffectType_FatigueMod = 5,
	};

	enum EnumOperation : UInt32
	{
		kOperation_Add = 0,
		kOperation_Multiply = 1,
		kOperation_Subtract = 2,
	};

	TESFullName		kFullName;	// 018
	EnumType		eType;		// 024
	EnumOperation	eOperation;	// 028
	Float32			fValue;		// 02C
};
static_assert(sizeof(TESAmmoEffect) == 0x30);