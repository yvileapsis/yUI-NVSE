#pragma once
#include "MagicItemForm.hpp"

// 0x44
class EnchantmentItem : public MagicItemForm {
public:
	EnchantmentItem();
	~EnchantmentItem();

	virtual void	ByteSwap();

	enum EnumType : UInt32
	{
		kType_Weapon = 2,
		kType_Apparel,
	};

	EType		eType;				// 34
	UInt32		uiCost;				// 38
	UInt32		uiLevel;			// 3C
	UInt8		ucEnchantmentFlags;	// 40
	UInt8		pad41[3];			// 41
};
static_assert(sizeof(EnchantmentItem) == 0x44);