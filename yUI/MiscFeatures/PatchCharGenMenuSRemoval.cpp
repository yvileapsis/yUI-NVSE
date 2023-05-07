#include <main.h>
#include <SafeWrite.h>

#include <SimpleINILibrary.h>


namespace Patch::CharGenMenuSRemoval
{
	bool enable = false;

	void Patch(const bool enable)
	{
		if (enable)
		{
			WriteRelCall(0x753E32, 0x0753E54);
		}
		else
		{
			UndoSafeWrite(0x753E32);
		}
	}

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bPatchCharGenMenuSRemoval", 0, "; removes hardcoded 's' added if there are multiple points to assign in CharGen menu");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		Patch(enable);
	}
}
