#include "SI.h"

#include <main.h>
#include <ranges>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace SI
{
	void HandleINIs()
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		auto iniPath = GetCurPath() + yUI_INI;
		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		g_FixIndefiniteSorting = ini.GetOrCreate("General", "bFixIndefiniteSorting", 1, "; fix the issue where items with different conditions would 'jump around' on update");

		g_SI = ini.GetOrCreate("General", "bSortingIcons", 0, "; enable Sorting and Icons section which controls ySI, don't enable this if you don't have ySI installed unless you know what you are doing");
		bSort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
		bIcons = ini.GetOrCreate("Sorting and Icons", "bAddInventoryIcons", 1, "; add ycons to inventory, container and barter menus");
		bHotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");
		bCategories = ini.GetOrCreate("Sorting and Icons", "bEnableCategories", 1, "; enable keyring-like clickable categories (this destroys vanilla keyring, so you have to have .json files supplying a new keyring category, i.e. ySI.json)");

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

		iniPath = GetCurPath() + R"(\Data\menus\ySI\ySI.ini)";
		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

		if (!g_SI) g_SI = ini.GetLongValue("General", "bSortingIcons", 0);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void ProcessSIEntries()
	{
		ra::sort(g_Items, [&](const Item& entry1, const Item& entry2)
		         { return entry1.common.priority > entry2.common.priority; });

		if (!bJustInTime)
		{
			for (const auto form : *GetAllForms()) {
				if (!form || !form->IsInventoryObjectAlt()) continue;
				Files::AssignCategoryToItem(form);
			}
//			SIFiles::g_Items_JSON = std::vector<SIItem>();
		}

		ra::sort(g_Categories, [&](const Category& entry1, const Category& entry2)
		         { return entry1.priority > entry2.priority; });

		for (auto& entry : g_Categories) {
			if (!entry.name.empty()) g_Keyrings.emplace(entry.tag);
			g_StringToCategory.emplace(entry.tag, std::move(entry));
		}
		g_Categories = std::vector<Category>();

		ra::sort(g_Tabs, [&](const Tab& entry1, const Tab& entry2)
			{ return entry1.priority > entry2.priority; });
		for (auto& entry : g_Tabs) g_StringToTabs.emplace(entry.tab, std::move(entry));

		std::vector<Tab> vector;
		for (const auto& snd : g_StringToTabs | std::views::values) vector.push_back(snd);
		ra::sort(vector, [&](const Tab& entry1, const Tab& entry2)
			{ return entry1.tabPriority > entry2.tabPriority; });
		for (const auto& it : vector) if (it.inventory) g_Tabline.push_back(it.tab);

		g_Tabs = std::vector<Tab>();
	}

	void ProcessFiles()
	{
		Log("Loading files");
		const auto dir = GetCurPath() + R"(\Data\menus\ySI)";
		const auto then = std::chrono::system_clock::now();
		if (std::filesystem::exists(dir))
			for (std::filesystem::directory_iterator iter(dir.c_str()), end; iter != end; ++iter)
			{
				const auto& path = iter->path();
				const auto& fileName = path.filename();
				if (iter->is_directory())
					Log(iter->path().string() + " found");
				else if (_stricmp(path.extension().string().c_str(), ".json") == 0)
					Files::HandleSIJson(iter->path());
				else if (_stricmp(path.extension().string().c_str(), ".xml") == 0)
				{
					auto pathstring = iter->path().generic_string();
					auto relativepath = pathstring.substr(pathstring.find_last_of("\\Data\\") - 3);
					g_XMLPaths.emplace_back(std::filesystem::path(relativepath));
				}
			}
		else
			Log(dir + " does not exist.");
		ProcessSIEntries();
		const auto now = std::chrono::system_clock::now();
		const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
		Log(FormatString("Loaded items, categories and tabs in %d ms", diff.count()));
	}

	void Patch()
	{
		Patches::AlterSorting(g_FixIndefiniteSorting || (g_SI && bSort));
		Patches::AddIcons(g_SI && bIcons);
		Patches::ReplaceHotkeyIcons(g_SI && bHotkeys);
		Patches::AddKeyrings(g_SI && bCategories);
		Patches::AddTabs(true);
	}
}

namespace SortingIcons
{
	extern void Init()
	{
		SI::Commands::Register();
		if (g_nvseInterface->isEditor) return;
		SI::HandleINIs();

		pluginLoad.emplace_back(SI::Patch);
		deferredInit.emplace_back(FillCraftingComponents);
		if (SI::bSort || SI::bIcons || SI::bHotkeys || SI::bCategories) deferredInit.emplace_back(SI::ProcessFiles);
		if (SI::bCategories) mainLoop.emplace_back(SI::Sorting::KeyringRefreshPostStewie);
		if (SI::bIcons) mainLoopDoOnce.emplace_back(SI::Icons::InjectTemplates);
	}
}