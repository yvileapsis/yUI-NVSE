#include "GameData.h"


class LoadedModFinder
{
	const char * m_stringToFind;

public:
	LoadedModFinder(const char * str) : m_stringToFind(str) { }

	bool Accept(ModInfo* modInfo)
	{
		return !StrCompare(modInfo->name, m_stringToFind);
	}
};

const ModInfo * TESDataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}

std::vector<ModInfo*> activeMods;

std::vector<ModInfo*> TESDataHandler::GetActiveModList()
{
	if (activeMods.empty())
		for (const auto iter: modList.modInfoList)
			if (iter->IsLoaded()) activeMods.push_back(iter);
	return activeMods;
}

UInt8 TESDataHandler::GetActiveModCount() const
{
	if (activeMods.empty())
		for (const auto iter : modList.modInfoList)
			if (iter->IsLoaded()) activeMods.push_back(iter);
	return activeMods.size();
}

UInt8 TESDataHandler::GetModIndex(const char* modName)
{
	return modList.modInfoList.GetIndexOf(LoadedModFinder(modName));
}

const char* TESDataHandler::GetNthModName(UInt32 modIndex)
{
	if (modIndex == 0xFF) return "Runtime";
	if (const auto iter = GetActiveModList()[modIndex]) return iter->name;
	return "";
}

void TESDataHandler::DisableAssignFormIDs(bool shouldAsssign)
{
	ThisStdCall(0x464D30, this, shouldAsssign);
}


ModInfo::ModInfo() {
	//
};

ModInfo::~ModInfo() {
	//
};
