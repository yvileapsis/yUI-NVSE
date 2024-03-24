#pragma once

#include "BSExtraData.hpp"
#include "BSSimpleList.hpp"

class TESFaction;

class ExtraFactionChanges : public BSExtraData {
public:
	ExtraFactionChanges();
	virtual ~ExtraFactionChanges();

	struct FactionListData
	{
		TESFaction* faction;
		UInt8		rank;
		UInt8		pad[3];
	};

	typedef BSSimpleList<FactionListData> FactionListEntry;
	FactionListEntry* data;

	void		DebugDump();

	static ExtraFactionChanges* Create();
};

ASSERT_SIZE(ExtraFactionChanges, 0x10);