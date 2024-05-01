#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"
#include "SpellItem.hpp"

// 0x14
class TESSpellList : public BaseFormComponent {
public:
	enum
	{
		kModified_BaseSpellList = 0x00000020,
		// CHANGE_ACTOR_BASE_SPELLLIST
		//	UInt16	numSpells;
		//	UInt32	spells[numSpells];
	};

	TESSpellList();
	~TESSpellList();

	virtual UInt32	GetSaveSize(UInt32 changedFlags);
	virtual void	Save(UInt32 changedFlags);
	virtual void	Load(UInt32 changedFlags);

	BSSimpleList<SpellItem*>	pkSpellList;			// 004
	BSSimpleList<SpellItem*>	pkLeveledSpellList;		// 00C

	UInt32	GetSpellCount() const {
		return pkSpellList.ItemsInList();
	}

	// return the nth spell
	SpellItem* GetNthSpell(SInt32 whichSpell) const {
		auto entry = pkSpellList.GetAt(whichSpell);
		return entry ? entry->GetItem() : nullptr;
	}
	// removes all spells and returns how many spells were removed
	//UInt32 RemoveAllSpells();
};
static_assert(sizeof(TESSpellList) == 0x14);