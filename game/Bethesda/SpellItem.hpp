#pragma once

#include "MagicItemForm.hpp"

class SpellItem : public MagicItemForm {
public:
	SpellItem();
	~SpellItem();

	virtual void	ByteSwap(void);

	enum
	{
		kType_ActorEffect = 0,
		kType_Disease,
		kType_Power,
		kType_LesserPower,
		kType_Ability,
		kType_Poison,
		kType_Addiction = 10,
	};

	UInt32		type;		// 34
	UInt32	unk38;		// 38	Cost Unused
	UInt32	unk3C;		// 3C	Level Unused
	UInt8		spellFlags;	// 40
	UInt8	pad41[3];	// 41
};

ASSERT_SIZE(SpellItem, 0x44);