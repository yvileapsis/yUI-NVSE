#include <main.h>
#include <SimpleINILibrary.h>
#include <SafeWrite.h>
#include <InterfaceMenus.h>
#include <InterfaceManager.h>

namespace Patch::MC
{
	inline int bEnable = 0;

	void HandleINIs()
	{
		CSimpleIniA ini;
		ini.SetUnicode();
		const auto iniPath = GetCurPath() + R"(\Data\NVSE\Plugins\yUI.ini)";
		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }
		bEnable = ini.GetOrCreate("General", "bMatchingCursor", 0, "; match cursor color to HUD color");
		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }
	}

	void __fastcall CursorTileSetStringValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* src, char propagate)
	{
		tile->SetFloat(kTileValue_zoom, -1, propagate);
		tile->SetFloat(kTileValue_systemcolor, 1, propagate);
	}

	void __fastcall CursorTileSetIntValue(Tile* tile, void* dummyEDX, eTileValue tilevalue, int value)
	{
		tile->SetFloat(kTileValue_visible, value, true);
		ThisCall(0xA0B350, InterfaceManager::GetSingleton()->cursor, 1, 0);
	}

	void PatchMatchedCursor(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelCall(0x70B33A, CursorTileSetStringValue);
			WriteRelCall(0x70C727, CursorTileSetIntValue);
		}
		else
		{
			UndoSafeWrite(0x70B33A);
			UndoSafeWrite(0x70C727);
		}
	}

	void Patch()
	{
		PatchMatchedCursor(bEnable);
	}
}

namespace Patch::MatchedCursor
{
	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;
		MC::HandleINIs();
		MC::Patch();
	}
}
