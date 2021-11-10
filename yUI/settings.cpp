#include "SimpleINILibrary.h"
#include "settings.h"
#include "functions.h"

void handleINIOptions()
{
	const auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yUI.ini)";
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());

	g_SortingFix = ini.GetOrCreate("General", "bSortingFix", 1, "; fix the issue where items with different conditions would 'jump around' on update");
	g_ySI = ini.GetOrCreate("General", "bSortingIcons", 0, "; enable ycons");
	g_yMC = ini.GetOrCreate("General", "bMatchingCursor", 0, "; match cursor color to HUD color");
	g_ySI_Sort = ini.GetOrCreate("Sorting Icons", "bSortInventory", 1, "; sort inventory according to tag names");

	ini.SaveFile(iniPath.c_str(), false);
}
