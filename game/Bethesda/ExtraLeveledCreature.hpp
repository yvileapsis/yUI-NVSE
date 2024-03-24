#pragma once

#include "BSExtraData.hpp"

class TESForm;

class ExtraLeveledCreature : public BSExtraData {
public:
	ExtraLeveledCreature();
	virtual ~ExtraLeveledCreature();

	EXTRADATATYPE(LEVELEDCREATURE);

	TESForm* pBaseForm;	// 00C
	TESForm* pForm;		// 010
};

ASSERT_SIZE(ExtraLeveledCreature, 0x14);