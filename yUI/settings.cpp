#include "SimpleINILibrary.h"
#include "settings.h"
#include "functions.h"

void handleINIOptions()
{
	const auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yUI.ini)";
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());

	//	GetModuleFileNameA(yUIHandle, iniPath, MAX_PATH);

/*	auto errVal = ini.LoadFile(iniPath)
	if (errVal == SI_Error::SI_FILE)
	{
		if (GetModuleHandle(MO2_VFS_DLL_NAME))
		{
			// user is using VFS and will probably not notice the ini if it were to generate in the MO2 overwrites folder, so don't create one
			MessageBoxA(NULL, "Tweaks INI not found, please download it or create an empty file nvse_stewie_tweaks.ini in \'Data\\NVSE\\Plugins\\\' !", "Stewie Tweaks", MB_ICONINFORMATION);
			return;
		}
	}*/

	//	ini.SetSortAlphabetically(ini.GetOrCreate("INI", "bSortAlphabetically", 0, "; sort the ini alphabetically"));
	//	ini.SetPrependNewKeys(ini.GetOrCreate("INI", "bPrependNewSettings", 1, "; add new settings to the top of the ini"));

	g_ySI = ini.GetOrCreate("General", "bSortingIcons", 1, "; enable ycons");
	g_ySI_Sort = ini.GetOrCreate("Sorting Icons", "bSortInventory", 1, "; sort inventory according to tag names");

	ini.SaveFile(iniPath.c_str(), false);
}
