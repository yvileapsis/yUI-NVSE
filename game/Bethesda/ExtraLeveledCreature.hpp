#pragma once
#include "BSExtraData.hpp"

class TESForm;
class TESActorBase;

class ExtraLeveledCreature : public BSExtraData {
public:
	ExtraLeveledCreature();
	virtual ~ExtraLeveledCreature();

	TESForm* baseForm;				// 00C
	TESActorBase* actorBase;		// 010
};
static_assert(sizeof(ExtraLeveledCreature) == 0x14);