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
		g_logLevel				= ini.GetLongValue("Debug", "iLogLevel", 1);
	}
	else
	{

	}
	
	if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

}

void handleINIOptions()
{
	auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yJAM.ini)";
	HandleINIForPath(iniPath, true);
//	iniPath = GetCurPath() + R"(\Data\menus\ySI\ySI.ini)";
//	HandleINIForPath(iniPath);
}