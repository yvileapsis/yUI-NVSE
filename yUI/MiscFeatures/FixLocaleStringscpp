#include <main.h>
#include <SimpleINILibrary.h>


namespace Fix::LocaleStrings
{
	bool enable = true;

	UInt32* localeCheck = reinterpret_cast<UInt32*>(0x126FDF4);

	void Patch(const bool enable)
	{
		if (enable)
		{
			*localeCheck = 1;
		}
		else
		{
			*localeCheck = 0;
		}
	}

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bFixLocaleStrings", 1, "; fixes the game to use the correct locale-appropriate toupper and tolower functions, fixing myriad of bugs when translating the game, like uppercase-only quest and location names'");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		Patch(enable);
	}
}
