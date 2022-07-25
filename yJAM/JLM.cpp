#include <JLM.h>
#include <functions.h>
#include <SimpleINILibrary.h>

#include "dinput8.h"

namespace JLM
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	UInt32 typeCodes[] {116, 46, 49, 25, 115, 108, 41, 40, 103, 24, 26, 47, 31};

	std::vector<ContChangesEntry*> items;

	TESObjectREFR*	container		= nullptr;

	/* those are needed to know if ref should be updated */
	TESObjectREFR*	ref				= nullptr;
	bool			notcannibal		= false;
	bool			dead			= false;
	bool			open			= false;

	TESForm*		formEVE				= nullptr;
	TESForm*		formShovel			= nullptr;
	TESForm*		formShovelUnique	= nullptr;

	SInt32			offset			= 0;
	SInt32			index			= 0;
	SInt32			total			= 0;
	SInt32			tiles			= 0;


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

	void Wah()
	{
		Log("WAAAAAAZ", 2);
		container = nullptr;
		ref = nullptr;
		items.clear();
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
		SetEventHandler("OnAdd", Wah);
		SetEventHandler("OnDrop", Wah);


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

		if (tileMain->GetChild("JLMContainer")) tileMain->GetChild("JLMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JLMContainer");

		for (UInt32 i = 0; i < g_ItemsMax; i++) {
			tileMain->GetChild("JLMContainer")->AddTileFromTemplate("JLMInjected")->SetFloat("_id", g_ItemsMax - i - 1);
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
		Float64 y = 0;
		Float64 x = 0;

		tileMain->SetString("_JLMTitle", container->GetTheName());
		tileMain->SetFloat("_JLMSystemColor", 1 + container->IsCrimeOrEnemy());
		tileMain->SetFloat("_JLMAlphaAC", g_MenuHUDMain->tileActionPointsMeterText1->GetFloat(kTileValue_alpha));

		auto item = items.begin() + offset;

		tiles = 0;

		for (const auto fst : tileMain->GetChild("JLMContainer")->children)
		{
			if (items.empty() || item == items.end())
			{
				fst->SetFloat(kTileValue_visible, 0);
				continue;
			}

			const auto snd = *item;
			++item;

			fst->SetFloat(kTileValue_y, y);

			fst->SetFloat("_JLMActive", tiles == index);

			std::string string = snd->form->GetTheName();

			if (snd->GetWeaponMod()) string += "+";

			if (snd->countDelta > 1) string += " (" + std::to_string(snd->countDelta) + ")";

			fst->SetString(kTileValue_string, string.c_str());
			fst->SetFloat(kTileValue_visible, 1);

			fst->SetFloat("_JLMEquip", snd->GetEquipped());

			// TODO:: fix get health percent
			if (const auto condition = snd->GetHealthPercentAlt(true); condition != -1)
			{
				fst->SetFloat("_JLMMeter", 1);
				fst->SetFloat("_JLMMeterValue", condition);
				fst->SetFloat("_JLMMeterArrow", snd->form->typeID == kFormType_TESObjectWEAP ? 0.75 : 0.5);
			}
			else fst->SetFloat("_JLMMeter", 0);

			const auto stringdimensions = FontManager::GetSingleton()->GetStringDimensions(string.c_str(), tileMain->GetFloat("_JLMFont"), fst->GetChild("JLMButtonText")->GetFloat(kTileValue_wrapwidth));

			const auto height = g_IndentTextY + stringdimensions->y;

			x = max(x, stringdimensions->x + fst->GetFloat("_JLMTextWidth"));

			if (y + height > g_HeightMax - 3 * g_IndentItem) break;
			y += height;

			tiles++;
		}
		tileMain->SetFloat("_JLMItemHeightCur", y);
		tileMain->SetFloat("_JLMItemWidthCur", x);

		tileMain->SetFloat("_JLMArrowUp", offset > 0);
		tileMain->SetFloat("_JLMArrowDown", offset < items.size() - tiles);

		tileMain->SetFloat("_JLMVisible", 1);
	}


	bool NameNotEmpty(TESObjectREFR* ref)
	{
		return !std::string(ref->GetTheName()).empty();
	}

	bool IsDead(TESObjectREFR* ref)
	{
		return ref->IsActor()  ? (reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dead || reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dying) : true;
	}

	// TODO:: sorting
	SInt16 CompareItems(ContChangesEntry* first, ContChangesEntry* second)
	{
		TileContChangesEntryUnk one{ first };
		TileContChangesEntryUnk two{ second };
		const auto wah = CdeclCall<SInt32>(0x75D130, &one, &two);
//		return wah > 0;
		return std::string(first->form->GetTheName()) < std::string(second->form->GetTheName());
	}

	void MinorUpdate()
	{
		Display();
	}


	void MajorUpdate()
	{
		items.clear();
		items = container->GetAllItems();
		ra::sort(items, CompareItems);
		offset = 0;
		index = 0;
		MinorUpdate();
	}

	void UpdateContainer(TESObjectREFR* newref)
	{
		bool update = false;
		if (ref != newref) update = true;
		ref = newref;

		if (!ref || !ref->baseForm->CanContainItems()) { container = nullptr; return; }

		if (dead != IsDead(ref)) { update = true; dead = 1 - dead; }
		if (open != !ref->IsLocked()) { update = true; open = 1 - open; }
		if (notcannibal != true) { update = true; notcannibal = 1 - notcannibal; } // TODO:: cannibal

		if (!update) return;

		if (!dead || !open || !notcannibal || !NameNotEmpty(ref)) { container = nullptr;  return; }
		container = ref->ResolveAshpile();
		MajorUpdate();
	}

	void MainLoop()
	{
		if (!initialized) return;
		if (container)
		{

		}
	}

	void HandleScroll()
	{
		bool update = false;
		if (IsKeyPressed(265)) {
			if (index < tiles - 1) { update = true; index++; }
			else if (offset < items.size() - tiles) { update = true; offset++; }
		}
		if (IsKeyPressed(264)) {
			if (index > 0) { update = true; index--; }
			else if (offset > 0) { update = true; offset--; }
		}
		if (update)
		{
			PlayGameSound("UIPipBoyScroll");
			MinorUpdate();
		}
	}


	void OnRender()
	{
		if (!initialized) return;

		UpdateContainer(InterfaceManager::GetSingleton()->crosshairRef);


		if (container)
		{
			HandleScroll();

		}
		else
		{
			tileMain->SetFloat("_JLMVisible", 0);
		}

	}
}
