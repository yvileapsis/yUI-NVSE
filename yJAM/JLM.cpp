#include <JLM.h>
#include <functions.h>
#include <SimpleINILibrary.h>

#include "dinput8.h"
#include "NiObjects.h"

namespace JLM
{
	bool				initialized			= false;
	Tile*				tileMain			= nullptr;

	std::vector<ContChangesEntry*> items;

	TESObjectREFR*		container			= nullptr;

	/* those are needed to know if ref should be updated */
	TESObjectREFR*		ref					= nullptr;
	bool				notcannibal			= false;
	bool				dead				= false;
	bool				open				= false;
	bool				owned				= false;

	TESForm*			formEVE				= nullptr;
	TESForm*			formShovel			= nullptr;
	TESForm*			formShovelUnique	= nullptr;

	UInt32				offset				= 0;
	UInt32				index				= 0;
	UInt32				total				= 0;
	SInt32				tiles				= 0;

	bool				info				= false;

	UInt32				key1				= 0;
	UInt32				key2				= 0;
	UInt32				key3				= 0;
	UInt32				keyAlt				= 0;
	UInt32				button1				= 0;
	UInt32				button2				= 0;
	UInt32				button3				= 0;
	UInt32				buttonAlt			= 0;

	bool				lockTake			= true;

	TESObjectREFR*		tookItem			= nullptr;
	TESObjectREFR*		openedContainer		= nullptr;

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

	std::string GetStringForButton(UInt32 button)
	{
		switch (button)
		{
		case 1 << 0: return "Up)";
		case 1 << 1: return "Down)";
		case 1 << 2: return "Left)";
		case 1 << 3: return "Right)";
		case 1 << 4: return "Start)";
		case 1 << 5: case 1 << 6: case 1 << 7: case 1 << 8: case 1 << 9:
		case 1 << 11: case 1 << 12: case 1 << 13: case 1 << 14:	return "";
		case 1 << 10: return "Guide)";
		default: return std::to_string(button);
		}
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
		SetEventHandler("ShowOff:OnPreActivate", OnPreActivate);

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

		if (!formShovel)		formShovel = GetFormByID("PointLookout.esm", 0x0082B5);
		if (!formShovelUnique)	formShovelUnique = GetFormByID("PointLookout.esm", 0x00D5A8);

		key1 = g_Key1 ? g_Key1 : GetControl(5, kControlType_Keyboard);
		key2 = g_Key2 ? g_Key2 : GetControl(7, kControlType_Keyboard);
		key3 = g_Key3 ? g_Key3 : GetControl(16, kControlType_Keyboard);
		keyAlt = g_KeyAlt ? g_KeyAlt : GetControl(9, kControlType_Keyboard);

		button1 = g_Button1 ? g_Button1 : GetControl(5, kControlType_Joystick);
		button2 = g_Button2 ? g_Button2 : GetControl(7, kControlType_Joystick);
		button3 = g_Button3 ? g_Button3 : GetControl(16, kControlType_Joystick);
		buttonAlt = g_ButtonAlt ? g_Button3 : GetControl(9, kControlType_Joystick);

		if (CdeclCall<bool>(0x4B71D0))
		{
			GetStringForButton(button1).empty() ? tileMain->SetFloat("_JLMButton1", button1) : tileMain->SetString("_JLMKeyString1", button1 ? (GetStringForButton(button1) + ")").c_str() : "");
			GetStringForButton(button2).empty() ? tileMain->SetFloat("_JLMButton2", button2) : tileMain->SetString("_JLMKeyString2", button2 ? (GetStringForButton(button2) + ")").c_str() : "");
			GetStringForButton(button3).empty() ? tileMain->SetFloat("_JLMButton3", button3) : tileMain->SetString("_JLMKeyString3", button3 ? (GetStringForButton(button3) + ")").c_str() : "");
		}
		else
		{
			tileMain->SetString("_JLMKeyString1", key1 ? (GetDXDescription(key1) + ")").c_str() : "");
			tileMain->SetString("_JLMKeyString2", key2 ? (GetDXDescription(key2) + ")").c_str() : "");
			tileMain->SetString("_JLMKeyString3", key3 ? (GetDXDescription(key3) + ")").c_str() : "");
		}
	}

	void Display()
	{
		if (offset + tiles > items.size()) { offset = items.size() > tiles ? items.size() - tiles : 0; }

		auto item = items.begin() + offset;

		Float64 y = 0;
		Float64 x = 0;

		tiles = 0;

		for (const auto fst : tileMain->GetChild("JLMContainer")->children)
		{
			if (items.empty() || item >= items.end())
			{
				fst->SetFloat(kTileValue_visible, 0);
				continue;
			}
			fst->SetFloat(kTileValue_visible, 1);

			const auto snd = *item;
			++item;

			fst->SetFloat(kTileValue_y, y);

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

		if (index > tiles - 1) { index = tiles - 1; }

		UInt32 i = 0;
		for (const auto fst : tileMain->GetChild("JLMContainer")->children) {
			fst->SetFloat("_JLMActive", i == index);
			if (i == index)
			{
				// TODO: send out event
			}
			i++;
		}
	}


	bool NameNotEmpty(TESObjectREFR* ref)
	{
		return !std::string(ref->GetTheName()).empty();
	}

	bool IsDead(TESObjectREFR* ref)
	{
		return ref->IsActor() ? (reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dead || reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dying) : true;
	}

	UInt32 typeCodes[]{ 116, 46, 49, 25, 115, 108, 41, 40, 103, 24, 26, 47, 31 };

	UInt16 CompareItems(ContChangesEntry* first, ContChangesEntry* second)
	{
		TileContChangesEntryUnk first_(first);
		TileContChangesEntryUnk second_(second);
		return CdeclCall<SInt16>(0x7824E0, &first_, &second_) != 1 ? 1 : 0;
	}

	void MinorUpdate()
	{
		if (g_HidePrompt && (!info && container || (info && !container)))
		{
			info = !info;
			SetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, (!info ? -1 : 1) + GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0));
		}

		if (g_HidePrompt) g_MenuHUDMain->tileInfo->SetFloat(kTileValue_visible, GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0) == 0);

		if (!container)
		{
			tileMain->SetFloat("_JLMVisible", 0);
			ToggleVanityWheel(true);
			if (key1) DisableKey(key1, false);
			if (key2) DisableKey(key2, false);
			if (key3) DisableKey(key3, false);
			if (keyAlt) DisableKey(keyAlt, false);
			return;
		}
		ToggleVanityWheel(false);
		if (key1) DisableKey(key1, true);
		if (key2) DisableKey(key2, true);
		if (key3) DisableKey(key3, true);
		if (keyAlt) DisableKey(keyAlt, true);

		owned = container->IsCrimeOrEnemy();
		tileMain->SetString("_JLMTitle", container->GetTheName());
		tileMain->SetFloat("_JLMSystemColor", 1 + owned);
		tileMain->SetFloat("_JLMAlphaAC", g_MenuHUDMain->tileActionPointsMeterText1->GetFloat(kTileValue_alpha));

		Display();

		tileMain->SetFloat("_JLMArrowUp", offset > 0);
		tileMain->SetFloat("_JLMArrowDown", offset < items.size() - tiles);

		tileMain->SetFloat("_JLMVisible", 1);
	}


	void MajorUpdate()
	{
		items.clear();
		if (container)
		{
			if (formEVE) container->RemoveItem(formEVE, nullptr, 1, 0, 0, nullptr, 0, 0, 0, 0);
			items = container->GetAllItems();
			ra::sort(items, CompareItems);
		}
		MinorUpdate();
	}

	void HandleContainerChange(TESObjectREFR* container, TESObjectREFR* newcontainer)
	{
		if (tookItem) { g_player->SendStealingAlarm(tookItem, true); tookItem = nullptr; }
		if (openedContainer) { container->OpenCloseContainer(false, g_Sounds & 0x1); openedContainer = nullptr; }

		offset = 0;
		index = 0;
	}

	bool notcannibal2 = true;
	bool owned2 = false;

	void UpdateContainer(TESObjectREFR* newref)
	{
		bool update = false;
		if (ref != newref) update = true;
		ref = newref;

		if (!ref || !ref->baseForm->CanContainItems())
		{
			HandleContainerChange(container, nullptr);
			container = nullptr;
			MajorUpdate();
			return;
		}

		if (owned != owned2) { update = true; owned = !owned; }
		if (dead != IsDead(ref)) { update = true; dead = !dead; }
		if (open != !ref->IsLocked()) { update = true; open = !open; }
		if (notcannibal != notcannibal2) { update = true; notcannibal = !notcannibal; }

		if (!update) return;
		const auto newcontainer = !dead || !open || !notcannibal || !NameNotEmpty(ref) ? nullptr : ref->ResolveAshpile();
		HandleContainerChange(container, newcontainer);
		container = newcontainer;
		MajorUpdate();
	}

	void MainLoop()
	{
		if (!initialized) return;

		if (MenuMode()) UpdateContainer(nullptr);

		notcannibal2 = ref ? GetCannibalPrompt(ref) : true;
		owned2 = ref ? ref->IsCrimeOrEnemy() : false;
	}

	void HandleScroll()
	{
		if (!container) return;

		bool update = false;

		if (!IsKeyPressed(265)) {}
		else if (index < tiles - 1)				{ update = true; index++; }
		else if (offset < items.size() - tiles)	{ update = true; offset++; }
		else if (g_OverScroll)					{ update = true; offset = 0; index = 0; }

		if (!IsKeyPressed(264)) {}
		else if (index > 0)						{ update = true; index--; }
		else if (offset > 0)					{ update = true; offset--; }
		else if (g_OverScroll)					{ update = true; offset = items.size() - tiles; index = tiles - 1; }

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
			if (item < items.end()) weight += CdeclCall<Float64>(0x48EBC0, (*item)->form, g_player->isHardcore);
		}

		tileMain->SetFloat("_JLMWeightColor", weight > weightmax);

	}

	void HandleTakeItem(ContChangesEntry* entry, bool all, bool keepowner, bool equip)
	{
		const auto form = entry->form->TryGetREFRParent();
		if (entry->form->GetIsReference()) {
			g_player->HandlePickupItem(reinterpret_cast<TESObjectREFR*>(entry->form), all ? entry->countDelta : 1, keepowner);
			g_player->SendStealingAlarm((TESObjectREFR*)entry->form, false);
		} else {
			container->RemoveItem(entry->form, entry->extendData ? entry->extendData->GetFirstItem() : nullptr, (all ||  form->typeID == kFormType_TESAmmo) ? entry->countDelta : 1, !container->IsActor(), 0, g_player, 0, 0, 1, 0);
			tookItem = container;
		}
		if (!keepowner && equip) g_player->EquipItem(form);
	}


	void Action(UInt32 action)
	{
		if (!openedContainer) { container->OpenCloseContainer(true, g_Sounds & 0x1); openedContainer = container; }

		const auto keepowner = owned;

		if (action == kActionTakeAll)
		{
			PlayGameSound("UIItemTakeAll");
			if (items.empty()) return;
			for (const auto item : items) HandleTakeItem(item, true, keepowner, false);
			MajorUpdate();
			return;
		}

		if (items.empty())
		{
			PlayGameSound("UIActivateNothing");
			return;
		}

		const auto item = items.begin() + index + offset;
		if (item >= items.end()) return;
		const auto entry = *item;

		bool takeall = false;
		if (CdeclCall<Float64>(0x48EBC0, entry->form, g_player->isHardcore) < 0.02) takeall = true;
		if (entry->countDelta > g_TakeSmartMin) takeall = true;

		g_player->PlayPickupPutdownSounds(entry->form, action != kActionEquip, action == kActionEquip);
		HandleTakeItem(entry, takeall, keepowner, action == kActionEquip);

		MajorUpdate();
	}

	void UpdateKeys()
	{
		const auto alt = keyAlt && IsKeyPressed(keyAlt, DIHookControl::kFlag_RawState) || buttonAlt && IsButtonPressed(buttonAlt);
		owned = container->IsCrimeOrEnemy();

		auto action = (alt ? g_Mode1Alt : g_Mode1) << 1;
		if (action > 0) action -= !owned;
		tileMain->SetFloat("_JLMAction1", action);

		action = (alt ? g_Mode2Alt : g_Mode2) << 1;
		if (action > 0) action -= !owned;
		tileMain->SetFloat("_JLMAction2", action);

		action = (alt ? g_Mode3Alt : g_Mode3) << 1;
		if (action > 0) action -= !owned;
		tileMain->SetFloat("_JLMAction3", action);
	}

	void OnRender()
	{
		if (!initialized) return;

		UpdateContainer(!MenuMode() ? InterfaceManager::GetSingleton()->crosshairRef : nullptr);

		if (container)
		{
			if (key1 && IsKeyPressed(key1, DIHookControl::kFlag_RawState) || button1 && IsButtonPressed(button1) ||
				key2 && IsKeyPressed(key2, DIHookControl::kFlag_RawState) || button2 && IsButtonPressed(button2) ||
				key3 && IsKeyPressed(key3, DIHookControl::kFlag_RawState) || button3 && IsButtonPressed(button3))
			{
				if (lockTake) { lockTake = false; container->Activate(g_player, 0, 0, 1); }
			}
			else lockTake = true;
			Weight();
			HandleScroll();
			UpdateKeys();
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

	UInt32 ActionToTake()
	{
		const auto alt = keyAlt && IsKeyPressed(keyAlt, DIHookControl::kFlag_RawState) || buttonAlt && IsButtonPressed(buttonAlt);
		if (key1 && IsKeyPressed(key1, DIHookControl::kFlag_RawState) || button1 && IsButtonPressed(button1)) return alt ? g_Mode1Alt : g_Mode1;
		if (key2 && IsKeyPressed(key2, DIHookControl::kFlag_RawState) || button2 && IsButtonPressed(button2)) return alt ? g_Mode2Alt : g_Mode2;
		if (key3 && IsKeyPressed(key3, DIHookControl::kFlag_RawState) || button3 && IsButtonPressed(button3)) return alt ? g_Mode3Alt : g_Mode3;
		return kActionNone;
	}

	void OnPreActivate(TESObjectREFR* thisObj, void *parameters)
	{
		const auto ptr = static_cast<ReferenceBool*>(parameters);
		const auto action = ActionToTake();
		if (!container || thisObj != container || ptr->ref != g_player || !ptr->isActivationNotPrevented || action == kActionOpen)
		{
			SetNativeHandlerFunctionBool(true);
			return;
		}
		SetNativeHandlerFunctionBool(false);
		if (action == kActionNone) return;
		Action(action);
	}

}
