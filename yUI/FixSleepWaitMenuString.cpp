#include <main.h>
#include <Menus.h>
#include <Tiles.h>
#include <SafeWrite.h>
#include <GameSettings.h>

#include <SimpleINILibrary.h>


namespace Fix::SleepWaitMenuString
{
	bool enable = true;

	void __fastcall SleepWaitSetStringValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* src, char propagate)
	{
		std::string string = GetStringFromGameSettingFromString(!SleepWaitMenu::GetSingleton()->isRest ? "sUActnRestmenu" : "sTargetTypeSleep");
		tile->SetString(tilevalue, string.c_str(), propagate);
	}

	void Patch(const bool enable)
	{
		if (enable)
		{
			WriteRelCall(0x7BFEB1, SleepWaitSetStringValue);
		}
		else
		{
			UndoSafeWrite(0x7BFEB1);
		}
	}

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bFixSleepWaitMenuString", 1, "; fixes Sleep Wait menu 'Sleep'/'Wait' string to use properly capitalized 'sTargetTypeSleep'/'sUActnRestmenu'");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		Patch(enable);
	}
}
