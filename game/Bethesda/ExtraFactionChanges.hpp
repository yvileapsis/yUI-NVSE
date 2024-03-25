#pragma once
#include "BSExtraData.hpp"
#include "BSSimpleList.hpp"

class TESFaction;

// 0x10
class ExtraFactionChanges : public BSExtraData {
public:
	ExtraFactionChanges();
	virtual ~ExtraFactionChanges();

	struct Data
	{
		TESFaction* pkFaction;
		UInt8		ucRank;
		UInt8		pad[3];
	};

	BSSimpleList<Data*>* pkData;

	static ExtraFactionChanges* Create();
};
static_assert(sizeof(BSSimpleList<ExtraFactionChanges::Data*>) == 0x8);
static_assert(sizeof(ExtraFactionChanges) == 0x10);