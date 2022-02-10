#include <settings.h>
#include <SimpleINILibrary.h>
#include <Utilities.h>

void HandleINIForPath(const std::string& iniPath, const bool isDefault = false)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	const auto errVal = ini.LoadFile(iniPath.c_str());

	if (errVal == SI_Error::SI_FILE) { return; }
	
	if (isDefault)
	{

	}
	
	ini.SaveFile(iniPath.c_str(), false);

}


void handleINIOptions()
{
	auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yGTM.ini)";
	HandleINIForPath(iniPath, true);
}