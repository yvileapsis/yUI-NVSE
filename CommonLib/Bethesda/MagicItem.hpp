#pragma once
#include "TESFullName.hpp"
#include "EffectItem.hpp"

// 0x1C
class MagicItem :
	public TESFullName,
	public EffectItemList
{
public:
	MagicItem();
	~MagicItem();

	// pure virtual args/return values unknown
	virtual void	Unk_04(); // pure virtual
	virtual void	Unk_05(); // pure virtual
	virtual void	Unk_06(); // pure virtual
	virtual bool	Unk_07();
	virtual bool	Unk_08();
	virtual void	Unk_09(); // pure virtual
	virtual void	Unk_0A(); // pure virtual
	virtual void	Unk_0B(); // pure virtual
	virtual void	Unk_0C(); // pure virtual
	virtual void	Unk_0D(); // pure virtual
	virtual void	Unk_0E();
	virtual void	Unk_0F(); // pure virtual

	// perhaps types are no longer correct!
	enum EnumType {
		kType_None = 0,
		kType_Spell = 1,
		kType_Enchantment = 2,
		kType_Alchemy = 3,
		kType_Ingredient = 4,
	};
	EnumType Type() const;
};
static_assert(sizeof(MagicItem) == 0x1C);