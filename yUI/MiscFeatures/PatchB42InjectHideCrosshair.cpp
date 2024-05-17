#include <main.h>
#include <SimpleINILibrary.h>

namespace Patch::B42InjectHideCrosshair
{
	inline bool enable = true;

	void HandleINIs()
	{
		const auto iniPath = GetCurPath() / yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bPatchB42InjectHideCrosshair", 0, "; hide crosshair based on B42 attach and detach events.");

		ini.SaveFile(iniPath.c_str(), false);
	}

	void MainLoopDoOnce()
	{
		RegisterEvent("B42IGO", 0, nullptr, 4);
		SetEventHandler("B42IGO", [] { g_HUDMainMenu->pkTileReticleCenter->Set(Tile::kValue_visible, false); });

		RegisterEvent("B42IDx", 0, nullptr, 4);
		SetEventHandler("B42IDx", [] { g_HUDMainMenu->pkTileReticleCenter->Set(Tile::kValue_visible, true); });
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		if (!enable) return;
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
	}
}
