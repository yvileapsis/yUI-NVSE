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

void HandleINIForPathJDC(const std::string& iniPath, const bool isDefault = false)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }
	
	if (isDefault)
	{
		g_JDC					= ini.GetOrCreate("JustMods", "JDCEnabled", 1, nullptr);
		g_JDC_Dynamic			= ini.GetOrCreate("JDC", "JDCDynamic", 1, nullptr);
		g_JDC_ShotgunAlt		= ini.GetOrCreate("JDC", "JDCShotgunAlt", 1, nullptr);
		g_JDC_NoNodeSighting	= ini.GetOrCreate("JDC", "JDCNoNodeSighting", 1, nullptr);
		g_JDC_ModeHolstered		= ini.GetOrCreate("JDC", "JDCModeHolstered", 1, nullptr);
		g_JDC_ModeOut1st		= ini.GetOrCreate("JDC", "JDCModeOut1st", 5, nullptr);
		g_JDC_ModeOut3rd		= ini.GetOrCreate("JDC", "JDCModeOut3rd", 5, nullptr);
		g_JDC_ModeSighting1st	= ini.GetOrCreate("JDC", "JDCModeSighting1st", 3, nullptr);
		g_JDC_ModeSighting3rd	= ini.GetOrCreate("JDC", "JDCModeSighting3rd", 3, nullptr);
		g_JDC_ModeScope			= ini.GetOrCreate("JDC", "JDCModeScope", 0, nullptr);
		g_JDC_Distance			= ini.GetOrCreate("JDC", "JDCDistance", 2.25, nullptr);
		g_JDC_Speed				= ini.GetOrCreate("JDC", "JDCSpeed", 0.3, nullptr);
		g_JDC_LengthMax			= ini.GetOrCreate("JDC", "JDCLengthMax", 72.0, nullptr);
		g_JDC_LengthMin			= ini.GetOrCreate("JDC", "JDCLengthMin", 24.0, nullptr);
		g_JDC_Width				= ini.GetOrCreate("JDC", "JDCWidth", 8.0, nullptr);
		g_JDC_OffsetMax			= ini.GetOrCreate("JDC", "JDCOffsetMax", 256.0, nullptr);
		g_JDC_OffsetMin			= ini.GetOrCreate("JDC", "JDCOffsetMin", 0.0, nullptr);

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

	iniPath = GetCurPath() + R"(\Data\Config\JustMods.ini)";
	HandleINIForPathJDC(iniPath, true);
//	iniPath = GetCurPath() + R"(\Data\menus\ySI\ySI.ini)";
//	HandleINIForPath(iniPath);
}