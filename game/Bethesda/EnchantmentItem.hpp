#pragma once

#include "MagicItemForm.hpp"

class EnchantmentItem : public MagicItemForm {
public:
	EnchantmentItem();
	~EnchantmentItem();

	virtual void	ByteSwap(void);

	enum
	{
		kType_Weapon = 2,
		kType_Apparel,
	};

	UInt32		type;		// 34
	UInt32		unk38;		// 38
	UInt32		unk3C;		// 3C
	UInt8		enchFlags;	// 40
	UInt8		pad41[3];	// 41
};

ASSERT_OFFSET(EnchantmentItem, type, 0x34);
ASSERT_SIZE(EnchantmentItem, 0x44);