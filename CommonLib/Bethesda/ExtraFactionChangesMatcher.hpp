#pragma once

#include "ExtraFactionChanges.hpp"

class TESFaction;

class ExtraFactionChangesMatcher {
	TESFaction* pFaction;
	ExtraFactionChanges* xFactionChanges;
public:
	ExtraFactionChangesMatcher(TESFaction* faction, ExtraFactionChanges* FactionChanges) : pFaction(faction), xFactionChanges(FactionChanges) {}
	bool Accept(ExtraFactionChanges::FactionListData* data) {
		return (data->faction == pFaction) ? true : false;
	}
};

ExtraFactionChanges::FactionListEntry* GetExtraFactionList(BaseExtraList& xDataList);
SInt8 GetExtraFactionRank(BaseExtraList& xDataList, TESFaction* faction);
void SetExtraFactionRank(BaseExtraList& xDataList, TESFaction* faction, SInt8 rank);