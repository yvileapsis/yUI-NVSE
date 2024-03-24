#pragma once

#include "BaseFormComponent.hpp"
#include "SpellItem.hpp"

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

	BSSimpleList<SpellItem*>	spellList;			// 004
	BSSimpleList<SpellItem*>	leveledSpellList;	// 00C

	UInt32	GetSpellCount() const {
		return spellList.ItemsInList();
	}

	// return the nth spell
	SpellItem* GetNthSpell(SInt32 whichSpell) const {
		auto entry = spellList.GetAt(whichSpell);
		return entry ? entry->GetItem() : nullptr;
	}

	// removes all spells and returns how many spells were removed
	//UInt32 RemoveAllSpells();
};

ASSERT_SIZE(TESSpellList, 0x14);