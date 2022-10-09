#include <InterfaceMenus.h>
#include <InterfaceManager.h>
#include <main.h>
#include <SimpleINILibrary.h>


namespace Fix::TS
{
	inline int bEnable = 1;

	bool needToFix = false;
	Tile* tabline = nullptr;

	void MainLoop()
	{
		if (CdeclCall<bool>(0x702360) && InterfaceManager::IsMenuVisible(kMenuType_Inventory)) {
			if (needToFix) {
				needToFix = false;
				if (!tabline)
				{
					tabline = InventoryMenu::GetSingleton()->tile->GetChild("GLOW_BRANCH")->GetChild("IM_Tabline");
					return;
				}
				for (const auto iter : tabline->children) iter->SetFloat(kTileValue_mouseover, 0, false);
			}
		}
		else needToFix = true;
	}

	void HandleINIs()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		const auto iniPath = GetCurPath() + yUI_INI;
		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }
		bEnable = ini.GetOrCreate("General", "bFixTablineSelected", 1, "; fix the issue where Inventory Menu tabline shows up with buttons already selected");
		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }
	}
}

namespace Fix::TablineSelected
{
	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		TS::HandleINIs();
		mainLoop.emplace_back(TS::MainLoop);
	}
}