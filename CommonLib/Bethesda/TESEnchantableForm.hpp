#pragma once
#include "BaseFormComponent.hpp"

class EnchantmentItem;

// 0x10
class TESEnchantableForm : public BaseFormComponent
{
public:
	TESEnchantableForm();
	~TESEnchantableForm();

	virtual UInt32	Unk_04();	// returns unk2

	EnchantmentItem*	enchantItem;			// 04
	UInt16				enchantment;			// 08
	UInt16				unk1;					// 0A
	UInt32				unk2;					// 0C
};
static_assert(sizeof(TESEnchantableForm) == 0x10);