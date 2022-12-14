#include <main.h>

#include <Form.h>
#include <Menus.h>
#include <SimpleINILibrary.h>

namespace UserInterface::WeaponHweel
{
	bool		enable = false;

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bHitMarker", true, "; enable 'Hit Marker' feature. If required files are not found this will do nothing.");

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			return;
		}
	}

	void MainLoopDoOnce()
	{
//		tileMain = g_HUDMainMenu->tile->GetChild("JHM");
//		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\HitMarker.xml)");
//			tileMain = g_HUDMainMenu->tile->GetChild("JHM");
		}
//		if (!tileMain) return;
		RegisterEvent("JHM:Reset", 0, nullptr, 4);
		SetEventHandler("JHM:Reset", Reset);
		Reset();
	}

	void Init()
	{
		if (g_nvseInterface->isEditor) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}