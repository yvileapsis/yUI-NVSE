#pragma once

#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class TESFaction;

class TESReactionForm : public BaseFormComponent {
public:
	TESReactionForm();
	~TESReactionForm();

	struct Reaction
	{
		enum
		{
			kNeutral = 0,
			kEnemy,
			kAlly,
			kFriend
		};

		TESFaction* faction;
		SInt32		modifier;
		UInt32		reaction;
	};

	BSSimpleList<Reaction*> m_reactionList;	// 4
	char m_cGroupFormType;		// C
};

ASSERT_SIZE(TESReactionForm, 0x10);