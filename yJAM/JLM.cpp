#include <JLM.h>
#include <functions.h>
#include <SimpleINILibrary.h>

namespace JLM
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	UInt32 typeCodes[] {116, 46, 49, 25, 115, 108, 41, 40, 103, 24, 26, 47, 31};

	std::vector<TileContChangesEntryUnk> g_Display;

	TESObjectREFR*	container		= nullptr;

	/* those are needed to know if ref should be updated */
	TESObjectREFR*	ref				= nullptr;
	bool			notcannibal		= false;
	bool			dead			= false;
	bool			open			= false;

	TESForm*		formEVE				= nullptr;
	TESForm*		formShovel			= nullptr;
	TESForm*		formShovelUnique	= nullptr;

	void Initialize()
	{
		initialized = true;
		tileMain = g_MenuHUDMain->tile->GetChild("JLM");
		if (!tileMain)
		{
			g_MenuHUDMain->tile->InjectUIXML(R"(Data\menus\prefabs\JLM\JLM.xml)");
			tileMain = g_MenuHUDMain->tile->GetChild("JLM");
		}
		if (!tileMain) return;
		RegisterEvent("JLM:Reset", 0, nullptr, 4);
		SetEventHandler("JLM:Reset", Reset);
		DispatchEvent("JLM:Reset", nullptr);
	}

	void HandleINI(const std::string& iniPath)
	{
		CSimpleIniA ini;
		ini.SetUnicode();

		if (const auto errVal = ini.LoadFile(iniPath.c_str()); errVal == SI_FILE) { return; }

//		g_JDC				= ini.GetOrCreate("JustMods", "JDC", 1.0, nullptr);
//		g_Dynamic			= ini.GetOrCreate("JDC", "Dynamic", 1.0, nullptr);
		
		if (const auto errVal = ini.SaveFile(iniPath.c_str(), false); errVal == SI_FILE) { return; }

	}

	void Reset()
	{
		HandleINI(GetCurPath() + R"(\Data\Config\JustMods.ini)");

		if (!g_JLM)
		{
			RemoveEventHandler("yJAM:MainLoop", MainLoop);
			RemoveEventHandler("yJAM:JG:OnRender", OnRender);
			return;
		}

		SetEventHandler("yJAM:MainLoop", MainLoop);
		SetEventHandler("yJAM:JG:OnRender", OnRender);

		tileMain->SetFloat("_JLMFontBase", g_Font);
		tileMain->SetFloat("_JLMFontYBase", g_FontY);
		tileMain->SetFloat("_JLMFontHeadBase", g_FontHead);
		tileMain->SetFloat("_JLMFontHeadYBase", g_FontHeadY);

		tileMain->SetFloat("_JLMItemHeightMin", g_HeightMin);
		tileMain->SetFloat("_JLMItemHeightMax", g_HeightMax);
		tileMain->SetFloat("_JLMItemWidthMin", g_WidthMin);
		tileMain->SetFloat("_JLMItemWidthMax", g_WidthMax);

		tileMain->SetFloat("_JLMOffsetX", g_OffsetX);
		tileMain->SetFloat("_JLMOffsetY", g_OffsetY);

		tileMain->SetFloat("_JLMItemIndent", g_IndentItem);
		tileMain->SetFloat("_JLMTextIndentX", g_IndentTextX);
		tileMain->SetFloat("_JLMTextIndentY", g_IndentTextY);

		tileMain->SetFloat("_JLMBracketTitle", 1 - g_HideName);

		tileMain->SetFloat("_JLMShowEquip", g_ShowEquip);
		tileMain->SetFloat("_JLMShowIcon", g_ShowIcon);
		tileMain->SetFloat("_JLMShowMeter", g_ShowMeter);

		tileMain->SetFloat("_JLMJustifyX", g_Justify % 3);
		tileMain->SetFloat("_JLMJustifyY", floor(g_Justify / 3));

		tileMain->SetFloat("_JLMVisible", 1);

		if (tileMain->GetChild("JLMContainer")) tileMain->GetChild("JLMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JLMContainer");

		for (UInt32 i = 0; i < g_ItemsMax; i++) {
			tileMain->GetChild("JLMContainer")->AddTileFromTemplate("JLMInjected")->SetFloat("_id", i);
		}

		if (!formEVE) formEVE = GetFormByID("EVE FNV - ALL DLC.esp", 0x02E612);
		if (!formEVE) formEVE = GetFormByID("EVE FNV - NO DLC.esp", 0x02E612);
		if (!formEVE) formEVE = GetFormByID("EVE FNV - NO GRA.esp", 0x02E612);
		if (!formEVE) formEVE = GetFormByID("RA-Gear.esm", 0x02E612);

		if (!formShovel)		formShovel			= GetFormByID("PointLookout.esm", 0x0082B5);
		if (!formShovelUnique)	formShovelUnique	= GetFormByID("PointLookout.esm", 0x00D5A8);
	}

	void Display()
	{
		Float32 y = 0;
		for (auto iter : g_Display)
		{
			iter.tile->SetString("string", "test");
			iter.tile->SetFloat("_Y", y);

			const auto height = g_IndentTextY + FontManager::GetSingleton()->GetStringDimensions("string", tileMain->GetFloat("_JLMFont"), iter.tile->GetChild("JLMButtonText")->GetFloat(kTileValue_wrapwidth))->y;

			if (y + height > g_HeightMax - 3 * g_IndentItem) break;
			y += height;
		}
		tileMain->SetFloat("_JLMItemHeightCur", y);
	}

	void MainLoop()
	{
		if (!initialized) return;

		tileMain->SetFloat(kTileValue_systemcolor, 1);

		tileMain->SetFloat("_JLMAlphaAC", g_MenuHUDMain->tileActionPointsMeterText1->GetFloat(kTileValue_alpha));
		tileMain->SetFloat("_JLMTotal", 5);
		tileMain->SetFloat("_JLMOffset", 0);
		tileMain->SetFloat("_JLMIndex", 0);
		tileMain->SetFloat("_JLMItems", 5);


		g_Display.clear();
		for (const auto iter : tileMain->GetChild("JLMContainer")->children)
		{
			TileContChangesEntryUnk tilecont;
			tilecont.tile = iter;
			g_Display.push_back(std::move(tilecont));
		}

		Display();
	}

	bool NameNotEmpty(TESObjectREFR* ref)
	{
		return !std::string(ref->GetTheName()).empty();
	}

	void UpdateContainer(TESObjectREFR* newref)
	{
		bool update = false;
		if (ref != newref) update = true;
		ref = newref;
		if (!update && ref && ref->CanContainItems())
		{
			if (dead != ref->IsActor() ? reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dead : true) { update = true; dead = 1 - dead; }
			if (open != !ref->IsLocked()) { update = true; open = 1 - open; }
			if (notcannibal != true) { update = true; notcannibal = 1 - notcannibal; }
		}
		if (!update) return;
		if (!ref) { container = nullptr; return; }
		ref = ref->ResolveAshpile();
		if (!dead || !open || !notcannibal || !NameNotEmpty(ref)) { container = nullptr;  return; }
		container = ref;
	}


	void OnRender()
	{
		UpdateContainer(InterfaceManager::GetSingleton()->crosshairRef);

		if (container)
		{
			tileMain->SetString("_JLMTitle", container->GetTheName());

		}

	}
}
