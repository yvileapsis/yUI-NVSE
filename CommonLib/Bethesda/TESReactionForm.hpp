#pragma once
#include "BaseFormComponent.hpp"
#include "BSSimpleList.hpp"

class TESFaction;

// 0x10
class TESReactionForm : public BaseFormComponent
{
public:
	TESReactionForm();
	~TESReactionForm();

	struct Reaction
	{
		enum EnumReaction : UInt32
		{
			kNeutral = 0,
			kEnemy,
			kAlly,
			kFriend
		};

		TESFaction*		pkFaction;
		SInt32			iModifier;
		EnumReaction	eReaction;
	};

	BSSimpleList<Reaction*>	kReactionList;		// 4
	UInt8					cGroupFormType;		// C
	UInt8					padD[3];			// D
};
static_assert(sizeof(TESReactionForm) == 0x10);