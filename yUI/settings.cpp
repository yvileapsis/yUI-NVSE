#include <settings.h>
#include <SimpleINILibrary.h>
#include <Utilities.h>

void HandleINIForPath(const std::string& iniPath, const bool isDefault = false)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }
	
	if (isDefault)
	{
		g_logLevel = ini.GetLongValue("Debug", "iLogLevel", 1);
		g_FixIndefiniteSorting = ini.GetOrCreate("General", "bFixIndefiniteSorting", 1, "; fix the issue where items with different conditions would 'jump around' on update");
		g_FixDroppedItems = ini.GetOrCreate("General", "bFixDroppedItems", 1, "; fix the issue where Container Menu would display only a single dropped item at a time");
		g_FixTablineSelected = ini.GetOrCreate("General", "bFixTablineSelected", 1, "; fix the issue where Inventory Menu tabline shows up with buttons already selected");
		g_FixReorderMCM = ini.GetOrCreate("General", "bFixReorderMCM", 1, "; fix the issues where MCM is incompatible with newest xNVSE features like Get/SetUIFloatAlt commands and inline expressions. This is fixed by reordering MCM's .xml in-code to work with these commands and hooking deprecated commands to work through Get/SetUIFloatAlt.");

		g_ySI = ini.GetOrCreate("General", "bSortingIcons", 0, "; enable Sorting and Icons section which controls ySI, don't enable this if you don't have ySI installed unless you know what you are doing");
		g_ySI_Sort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
		g_ySI_Icons = ini.GetOrCreate("Sorting and Icons", "bAddInventoryIcons", 1, "; add ycons to inventory, container and barter menus");
		g_ySI_Hotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");
		g_ySI_Categories = ini.GetOrCreate("Sorting and Icons", "bEnableCategories", 1, "; enable keyring-like clickable categories (this destroys vanilla keyring, so you have to have .json files supplying a new keyring category, i.e. ySI.json)");

		g_yMC = ini.GetOrCreate("General", "bMatchingCursor", 0, "; match cursor color to HUD color");
	}
	else
	{
		if (!g_ySI) g_ySI = ini.GetLongValue("General", "bSortingIcons", 0);
		if (!g_FixReorderMCM) g_FixReorderMCM = ini.GetLongValue("General", "bFixReorderMCM", 0);
	}
	
	if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

}


void handleINIOptions()
{
	auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yUI.ini)";
	HandleINIForPath(iniPath, true);

	iniPath = GetCurPath() + R"(\Data\menus\ySI\ySI.ini)";
	HandleINIForPath(iniPath);

	iniPath = GetCurPath() + R"(\Data\Config\MCMFix.ini)";
	HandleINIForPath(iniPath);
}