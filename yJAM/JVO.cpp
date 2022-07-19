#include <JVO.h>

#include <GameTiles.h>
#include <GameUI.h>
#include <GameObjects.h>
#include <GameSettings.h>
#include <GameProcess.h>
#include <cmath>

#include <SimpleINILibrary.h>

#include "functions.h"

namespace JVO
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

//		g_JDC = ini.GetOrCreate("JustMods", "JDC", 1.0, nullptr);
//		g_JDC_Dynamic = ini.GetOrCreate("JDC", "Dynamic", 1.0, nullptr);

		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset(TESObjectREFR* wah, void* par)
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

//		tileMain->SetFloat(Tile::TraitNameToID("_JDCDynamicOffset"), g_JDC_Dynamic & 1);

	}

	void Initialize()
	{
		initialized = true;
		tileMain = g_MenuHUDMain->tile->GetChild("JVO");
		if (!tileMain)
		{
			g_MenuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JVO\JVO.xml)");
			tileMain = g_MenuHUDMain->tile->GetChild("JVO");
		}
		if (!tileMain) return;
		RegisterEvent("JVO:Reset", 0, nullptr, 4);
		SetNativeEventHandler("JVO:Reset", reinterpret_cast<EventHandler>(Reset));
		DispatchEvent("JVO:Reset", nullptr);
	}

	void AddVisualObjective(TESObjectREFR* target)
	{
		
	}

	void MainLoop()
	{
		const auto targets = g_Player->GetCurrentQuestObjectiveTargets();
		const auto target = g_Player->GetPlacedMarkerOrTeleportLink();

		AddVisualObjective(target);
		for (const auto i : *targets)
		{
			AddVisualObjective(i->teleportLinks.size ? i->teleportLinks.data->door : i->target);
		}
	}
}
