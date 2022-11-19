#include <main.h>
#include <Menus.h>
#include <InterfaceManager.h>

#include <SimpleINILibrary.h>

namespace Fix::TablineSelected
{
	inline int enable = 1;

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
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

		enable = ini.GetOrCreate("General", "bFixTablineSelected", 1, "; fix the issue where Inventory Menu tabline shows up with buttons already selected");

		ini.SaveFile(iniPath.c_str(), false);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		HandleINIs();
		if (enable) mainLoop.emplace_back(MainLoop);
	}
}