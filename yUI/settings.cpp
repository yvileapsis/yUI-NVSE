#include "SimpleINILibrary.h"
#include "settings.h"
#include "functions.h"

void handleINIOptions()
{
	const auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yUI.ini)";
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());
	g_ySI = ini.GetOrCreate("General", "bSortingIcons", 1, "; enable Sorting and Icons section");
	g_yMC = ini.GetOrCreate("General", "bMatchingCursor", 0, "; match cursor color to HUD color");

	g_ySI_SortingFix = ini.GetOrCreate("Sorting and Icons", "bSortingFix", 1, "; fix the issue where items with different conditions would 'jump around' on update");
	g_ySI_Sort = ini.GetOrCreate("Sorting and Icons", "bSortInventory", 1, "; sort inventory according to tag names supplied in .json");
	g_ySI_Icons = ini.GetOrCreate("Sorting and Icons", "bAddInventoryIcons", 1, "; add ycons to inventory, container and barter menus");
	g_ySI_Hotkeys = ini.GetOrCreate("Sorting and Icons", "bReplaceHotkeyIcons", 1, "; replace hotkey icons with ycons");
	ini.SaveFile(iniPath.c_str(), false);
}
