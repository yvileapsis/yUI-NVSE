#include <SimpleINILibrary.h>
#include <settings.h>
#include <functions.h>

void HandleINIForPath(const std::string& iniPath, const bool isDefault = false)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());

	if (errVal == SI_Error::SI_FILE) { return; }
	
	if (isDefault)
	{
		g_SortingFix = ini.GetOrCreate("General", "bSortingFix", 1, "; fix the issue where items with different conditions would 'jump around' on update");

		g_ySI = ini.GetOrCreate("General", "bSortingIcons", 0, "; enable Sorting and Icons section");
		g_ySI_Sort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
		g_ySI_Icons = ini.GetOrCreate("Sorting and Icons", "bAddInventoryIcons", 1, "; add ycons to inventory, container and barter menus");
		g_ySI_Hotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");
		g_ySI_Categories = ini.GetOrCreate("Sorting and Icons", "bEnableCategories", 1, "; enable keyring-like clickable categories (this destroys vanilla keyring)");

		g_yMC = ini.GetOrCreate("General", "bMatchingCursor", 0, "; match cursor color to HUD color");
	}
	else
	{
		if (!g_ySI) g_ySI = ini.GetLongValue("General", "bSortingIcons", 0);
	}
	
	ini.SaveFile(iniPath.c_str(), false);

}


void handleINIOptions()
{
	auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yUI.ini)";
	HandleINIForPath(iniPath, true);

	iniPath = GetCurPath() + R"(\Data\menus\ySI\ySI.ini)";
	HandleINIForPath(iniPath);
}