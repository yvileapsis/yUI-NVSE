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

const ModInfo * DataHandler::LookupModByName(const char * modName)
{
	return modList.modInfoList.Find(LoadedModFinder(modName));
}

const ModInfo ** DataHandler::GetActiveModList()
{
	static const ModInfo* activeModList[0x100] = { 0 };

	if (!(*activeModList))
	{
		UInt16 index = 0;
		for (index = 0  ; index < DataHandler::GetSingleton()->modList.modInfoList.Count() ; index++)
		{
			ModInfo* entry = DataHandler::GetSingleton()->modList.modInfoList.GetNthItem(index);
			if (entry->IsLoaded())
				activeModList[index] = entry;
		}
	}

	return activeModList;
}

UInt8 DataHandler::GetModIndex(const char* modName)
{
	return modList.modInfoList.GetIndexOf(LoadedModFinder(modName));
}

const char* DataHandler::GetNthModName(UInt32 modIndex)
{
	const ModInfo** activeModList = GetActiveModList();
	if (modIndex < GetActiveModCount() && activeModList[modIndex])
		return activeModList[modIndex]->name;
	else
		return "";
}

void DataHandler::DisableAssignFormIDs(bool shouldAsssign)
{
	ThisStdCall(0x464D30, this, shouldAsssign);
}

struct IsModLoaded
{
	bool Accept(ModInfo* pModInfo) const {
		return pModInfo->IsLoaded();
	}
};

UInt8 DataHandler::GetActiveModCount() const
{
	UInt32 count = modList.modInfoList.CountIf(IsModLoaded());
	return count;
}

ModInfo::ModInfo() {
	//
};

ModInfo::~ModInfo() {
	//
};
