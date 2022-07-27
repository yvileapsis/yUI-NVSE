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

	bool			info			= false;

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
		SetEventHandler("OnAdd", OnAddDropUpdate);
		SetEventHandler("OnDrop", OnAddDropUpdate);
		SetEventHandler("yJAM:OnPreActivate", OnPreActivate);


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

		tileMain->SetFloat("_JLMWeightVisible", 0);

		if (tileMain->GetChild("JLMContainer")) tileMain->GetChild("JLMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JLMContainer");

		for (UInt32 i = 0; i < g_ItemsMax; i++)	tileMain->GetChild("JLMContainer")->AddTileFromTemplate("JLMInjected");

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
		tiles = 0;

		if (g_HidePrompt) g_MenuHUDMain->tileInfo->SetFloat(kTileValue_visible, GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0) == 0);

		if (!container)
		{
			tileMain->SetFloat("_JLMVisible", 0);
			return;
		}

		tileMain->SetString("_JLMTitle", container->GetTheName());
		tileMain->SetFloat("_JLMSystemColor", 1 + container->IsCrimeOrEnemy());
		tileMain->SetFloat("_JLMAlphaAC", g_MenuHUDMain->tileActionPointsMeterText1->GetFloat(kTileValue_alpha));

		auto item = items.begin() + offset;

		for (const auto fst : tileMain->GetChild("JLMContainer")->children)
		{
			if (items.empty() || item == items.end())
			{
				fst->SetFloat(kTileValue_visible, 0);
				continue;
			}
			fst->SetFloat(kTileValue_visible, 1);

			const auto snd = *item;
			++item;

			fst->SetFloat(kTileValue_y, y);

			fst->SetFloat("_JLMActive", tiles == index);
			if (tiles == index)
			{
				// TODO: send out event
			}

			std::string string = snd->form->GetTheName();

			if (snd->GetWeaponMod()) string += "+";

			if (snd->countDelta > 1) string += " (" + std::to_string(snd->countDelta) + ")";

			fst->SetString(kTileValue_string, string.c_str());

			fst->SetFloat("_JLMEquip", snd->GetEquipped());

			if (const auto condition = snd->GetHealthPercentAlt(true); condition != -1)
			{
				fst->SetFloat("_JLMMeter", 1);
				fst->SetFloat("_JLMMeterValue", condition);
				fst->SetFloat("_JLMMeterArrow", snd->form->TryGetREFRParent()->typeID == kFormType_TESObjectWEAP ? 0.75 : 0.5);
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
		const auto a1 = first;
		const auto a2 = second;

		TESForm* form1 = nullptr, * form2 = nullptr;

		if (a1 && a1->form) form1 = a1->form;
		if (a2 && a2->form) form2 = a2->form;

		signed int cmp;

		std::string name1, name2;

		if (form1) name1 = form1->GetTheName();
		if (form2) name2 = form2->GetTheName();

		cmp = name1.compare(name2);
		if (cmp > 0) return 1;
		if (cmp < 0) return -1;

		if (!form1) return form2 ? -1 : 0;
		if (!form2) return 1;

		const SInt16 mods1 = a1->GetWeaponMod();
		const SInt16 mods2 = a2->GetWeaponMod();
		if (mods1 != mods2) return mods1 > mods2 ? -1 : 1;

		const float condition1 = a1->GetHealthPercent();
		const float condition2 = a2->GetHealthPercent();
		if (condition1 != condition2) return condition1 > condition2 ? -1 : 1;

		const bool equipped1 = a1->GetEquipped();
		const bool equipped2 = a2->GetEquipped();
		if (equipped1 != equipped2) return equipped1 > equipped2 ? -1 : 1;

		const UInt32 refID1 = form1->refID;
		const UInt32 refID2 = form2->refID;
		if (refID1 != refID2) return refID1 > refID2 ? -1 : 1;

		return 0;
	}


	void MinorUpdate()
	{
		Display();
	}


	void MajorUpdate()
	{
		items.clear();
		if (g_HidePrompt && ((!info && container) || (info && !container)))
		{
			info = true - info;
			SetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, (!info ? -1 : 1) + GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0));
		}
		if (container)
		{
			items = container->GetAllItems();
			ra::sort(items, CompareItems);
		}
		MinorUpdate();
	}

	void UpdateContainer(TESObjectREFR* newref)
	{
		bool update = false;
		if (ref != newref) update = true;
		ref = newref;

		if (!ref || !ref->baseForm->CanContainItems()) { container = nullptr; MajorUpdate(); return; }

		if (dead != IsDead(ref)) { update = true; dead = 1 - dead; }
		if (open != !ref->IsLocked()) { update = true; open = 1 - open; }
		if (notcannibal != true) { update = true; notcannibal = 1 - notcannibal; } // TODO:: cannibal

		if (!update) return;

		if (!dead || !open || !notcannibal || !NameNotEmpty(ref)) { container = nullptr; MajorUpdate(); return; }
		container = ref->ResolveAshpile();
		offset = 0;
		index = 0;
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
		if (index > tiles - 1) { update = true; index = tiles - 1; }

		if (update)
		{
			PlayGameSound("UIPipBoyScroll");
			MinorUpdate();
		}
	}

	void Weight()
	{
		if (!g_WeightVisible) return;

		auto weight = g_player->GetActorValue(kAVCode_InventoryWeight);
		const auto weightmax = g_player->GetMaxCarryWeight();

		if (g_WeightVisible == 1 && weight / weightmax < 0.9)
		{
			tileMain->SetFloat("_JLMWeightVisible", 0);
			return;
		}

		tileMain->SetFloat("_JLMWeightVisible", 1);
		tileMain->SetString("_JLMWeight", FormatString("%.0f/%.0f", weight, weightmax).c_str());

		if (!g_WeightAltColor) return;

		if (g_WeightAltColor == 2 && !items.empty())
		{
			const auto item = (items.begin() + index + offset);
			if (item != items.end()) weight += CdeclCall<Float64>(0x48EBC0,(*item)->form, g_player->isHardcore);
		}

		tileMain->SetFloat("_JLMWeightColor", weight > weightmax);

	}

	bool a = true;

	void HandleTakeItem(ContChangesEntry* entry, bool all, bool keepowner, bool equip)
	{
		const auto form = entry->form->TryGetREFRParent();
		if (entry->form->GetIsReference())
			g_player->HandlePickupItem(reinterpret_cast<TESObjectREFR*>(entry->form), all ? entry->countDelta : 1, keepowner);
		else
		{
			container->RemoveItem(entry->form, entry->extendData ? entry->extendData->GetFirstItem() : nullptr, all ? entry->countDelta : 1, keepowner, 0, g_player, 0, 0, 1, 0);
			g_player->PlayPickupPutdownSounds(entry->form, !equip, equip);
		}
		if (!keepowner && equip)
		{
			g_player->EquipItem(form);
		}
	}

	void TakeItem()
	{
		if (items.empty()) return;

		const auto item = items.begin() + index + offset;
		if (item == items.end()) return;
		const auto entry = *item;

		bool keepowner = false;
		bool takeall = false;
		bool equip = false;

		keepowner = container->IsCrimeOrEnemy();

		if (IsKeyPressed(42))
		{
			equip = true;
			Log("AAA", 2);
		}
		{
			if (CdeclCall<Float64>(0x48EBC0, entry->form, g_player->isHardcore) < 0.02) takeall = true;
			if (entry->countDelta > g_TakeSmartMin) takeall = true;
		}
		HandleTakeItem(entry, takeall, keepowner, equip);

		HandleScroll();
		MajorUpdate();
	}

	void OnRender()
	{
		if (!initialized) return;

		UpdateContainer(InterfaceManager::GetSingleton()->crosshairRef);

		if (container)
		{
			if (IsKeyPressed(0x28)) { if (a) { a = false; TakeItem(); } }
			else a = true;
			Weight();
			HandleScroll();
			if (g_HidePrompt) g_MenuHUDMain->tileInfo->SetFloat(kTileValue_visible, GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0) == 0);
		}
	}

	struct ReferenceForm
	{
		TESObjectREFR* ref;
		TESForm* form;
	};

	void OnAddDropUpdate(TESObjectREFR* thisObj, void* parameters)
	{
		if (const auto ptr = static_cast<ReferenceForm*>(parameters); container && ptr && ptr->ref == container) MajorUpdate();
	}

	struct ReferenceBool
	{
		TESObjectREFR* ref;
		bool isActivationNotPrevented;
	};

	void OnPreActivate(TESObjectREFR* thisObj, void *parameters)
	{
		TakeItem();
		const auto ptr = static_cast<ReferenceBool*>(parameters);
	}
}