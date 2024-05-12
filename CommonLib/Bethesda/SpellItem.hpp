#pragma once
#include "MagicItemForm.hpp"

// 0x44
class SpellItem : public MagicItemForm
{
public:
	SpellItem();
	~SpellItem();

	virtual void	ByteSwap();

	enum EnumType : UInt32
	{
		kType_ActorEffect = 0,
		kType_Disease,
		kType_Power,
		kType_LesserPower,
		kType_Ability,
		kType_Poison,
		kType_Addiction = 10,
	};

	EnumType	eType;			// 34
	UInt32		uiCost;			// 38	Cost Unused
	UInt32		uiLevel;		// 3C	Level Unused
	UInt8		ucSpellFlags;	// 40
	UInt8		pad41[3];		// 41
};
static_assert(sizeof(SpellItem) == 0x44);