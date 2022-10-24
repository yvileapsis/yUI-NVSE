#include <main.h>
#include <functions.h>
#include <SimpleINILibrary.h>

#include <dinput8.h>

#include <Menus.h>
#include <InterfaceManager.h>

#include "SortingIcons/SI.h"

namespace UserInterface::LootMenu
{

	bool		enable			= false;

	enum
	{
		kNone		= 0,
		kTake		= 1,
		kEquip		= 2,
		kTakeAll	= 3,
		kOpen		= 4,
	};

	UInt32		key1Base		= 0;
	UInt32		key2Base		= 0;
	UInt32		key3Base		= 0;
	UInt32		keyAltBase		= 0;
	UInt32		keyScrollUp		= 264;
	UInt32		keyScrollDown	= 265;

	UInt32		button1Base		= 0;
	UInt32		button2Base		= 0;
	UInt32		button3Base		= 0;
	UInt32		buttonAltBase	= 0;

	UInt32		mode1			= kTake;
	UInt32		mode1Alt		= kEquip;
	UInt32		mode2			= kOpen;
	UInt32		mode2Alt		= kTakeAll;
	UInt32		mode3			= kNone;
	UInt32		mode3Alt		= kNone;

	UInt32		takeSmartMin	= 5;
	bool		takeWeightless	= true;

	bool		block			= false;
	bool		overScroll		= false;
	bool		hidePrompt		= true;
	bool		hideName		= false;
	UInt32		itemsMax		= 5;

	UInt32		justify			= 3;
	Float32		heightMin		= 32.0;
	Float32		heightMax		= 640.0;
	Float32		widthMin		= 400.0;
	Float32		widthMax		= 640.0;
	Float32		offsetX			= 0.625;
	Float32		offsetY			= 0.5;

	Float32		indentItem		= 8.0;
	Float32		indentTextX		= 10.0;
	Float32		indentTextY		= 10.0;

	UInt32		weightVisible	= 2;
	UInt32		weightAltColor	= 1;
	UInt32		font			= 0;
	UInt32		fontHead		= 0;
	Float32		fontY			= 0;
	Float32		fontHeadY		= 0;

	bool		sounds			= true;
	bool		showEquip		= true;
	bool		showIcon		= true;
	bool		showMeter		= true;


	Tile*		tileMain		= nullptr;

	std::vector<InventoryChanges*> items;

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

			fst->SetFloat("_Equip", snd->GetEquipped());

			const auto category = SortingIcons::Sorting::GetCategoryForItem(snd->form->TryGetREFRParent());
			if (!category.empty())
			{
				fst->SetFloat("_Icon", true);
				fst->SetString("_IconFilename", SortingIcons::g_StringToCategory[category].filename.c_str());
			}
			else fst->SetFloat("_Icon", false);

			if (const auto condition = snd->GetHealthPercentAlt(true); condition != -1)
			{
				fst->SetFloat("_Meter", 1);
				fst->SetFloat("_MeterValue", condition);
				fst->SetFloat("_MeterArrow", snd->form->TryGetREFRParent()->typeID == kFormType_TESObjectWEAP ? 0.75 : 0.5);
			}
			else fst->SetFloat("_Meter", 0);

			const auto stringdimensions = FontManager::GetSingleton()->GetStringDimensions(string.c_str(), tileMain->GetFloat("_Font"), fst->GetChild("ButtonText")->GetFloat(kTileValue_wrapwidth));

			const auto height = indentTextY + stringdimensions->y;

			x = max(x, stringdimensions->x + fst->GetFloat("_TextWidth"));

			if (y + height > heightMax - 3 * indentItem) break;
			y += height;

			tiles++;
		}
		tileMain->SetFloat("_ItemHeightCur", y);
		tileMain->SetFloat("_ItemWidthCur", x);

		if (index > tiles - 1) { index = tiles - 1; }

		UInt32 i = 0;
		for (const auto fst : tileMain->GetChild("JLMContainer")->children) {
			fst->SetFloat("_Active", i == index);
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

	UInt16 CompareItems(InventoryChanges* first, InventoryChanges* second)
	{
		TileInventoryChangesUnk first_(first);
		TileInventoryChangesUnk second_(second);
		return CdeclCall<SInt16>(0x7824E0, &first_, &second_) != 1 ? 1 : 0;
	}

	void MinorUpdate()
	{
		if (hidePrompt && (!info && container || (info && !container)))
		{
			info = !info;
			SetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, (!info ? -1 : 1) + GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0));
		}

		if (hidePrompt) g_HUDMainMenu->tileInfo->SetFloat(kTileValue_visible, GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0) == 0);

		if (!container)
		{
			tileMain->SetFloat("_Visible", 0);
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
		tileMain->SetString("_Title", container->GetTheName());
		tileMain->SetFloat("_SystemColor", 1 + owned);
		tileMain->SetFloat("_AlphaAC", g_HUDMainMenu->tileActionPointsMeterText1->GetFloat(kTileValue_alpha));

		Display();

		tileMain->SetFloat("_ArrowUp", offset > 0);
		tileMain->SetFloat("_ArrowDown", offset < items.size() - tiles);

		tileMain->SetFloat("_Visible", 1);
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
		if (openedContainer) { container->OpenCloseContainer(false, sounds & 0x1); openedContainer = nullptr; }

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
		if (!enable) return;

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
		else if (overScroll)					{ update = true; offset = 0; index = 0; }

		if (!IsKeyPressed(264)) {}
		else if (index > 0)						{ update = true; index--; }
		else if (offset > 0)					{ update = true; offset--; }
		else if (overScroll)					{ update = true; offset = items.size() - tiles; index = tiles - 1; }

		if (update)
		{
			PlayGameSound("UIPipBoyScroll");
			MinorUpdate();
		}
	}

	void Weight()
	{
		if (!weightVisible) return;

		auto weight = g_player->GetActorValue(kAVCode_InventoryWeight);
		const auto weightmax = g_player->GetMaxCarryWeight();

		if (weightVisible == 1 && weight / weightmax < 0.9)
		{
			tileMain->SetFloat("_WeightVisible", 0);
			return;
		}

		tileMain->SetFloat("_WeightVisible", 1);
		tileMain->SetString("_Weight", FormatString("%.0f/%.0f", weight, weightmax).c_str());

		if (!weightAltColor) return;

		if (weightAltColor == 2 && !items.empty())
		{
			const auto item = (items.begin() + index + offset);
			if (item < items.end()) weight += CdeclCall<Float64>(0x48EBC0, (*item)->form, g_player->isHardcore);
		}

		tileMain->SetFloat("_WeightColor", weight > weightmax);

	}

	void HandleTakeItem(InventoryChanges* entry, bool all, bool keepowner, bool equip)
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
		if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }

		const auto keepowner = owned;

		if (action == kTakeAll)
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
		if (entry->countDelta > takeSmartMin) takeall = true;

		g_player->PlayPickupPutdownSounds(entry->form, action != kEquip, action == kEquip);
		HandleTakeItem(entry, takeall, keepowner, action == kEquip);

		MajorUpdate();
	}

	void UpdateKeys()
	{
		const auto alt = keyAlt && IsKeyPressed(keyAlt, DIHookControl::kFlag_RawState) || buttonAlt && IsButtonPressed(buttonAlt);
		owned = container->IsCrimeOrEnemy();

		auto action = (alt ? mode1Alt : mode1) << 1;
		if (action > 0) action -= !owned;
		tileMain->SetFloat("_Action1", action);

		action = (alt ? mode2Alt : mode2) << 1;
		if (action > 0) action -= !owned;
		tileMain->SetFloat("_Action2", action);

		action = (alt ? mode3Alt : mode3) << 1;
		if (action > 0) action -= !owned;
		tileMain->SetFloat("_Action3", action);
	}

	void OnRender()
	{
		if (!enable) return;

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
			if (hidePrompt) g_HUDMainMenu->tileInfo->SetFloat(kTileValue_visible, GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0) == 0);
		}
	}

	void OnAddDropUpdate(TESObjectREFR* thisObj, TESObjectREFR* droppedFrom, TESForm* noidea)
	{
		if (container && droppedFrom && droppedFrom == container) MajorUpdate();
	}

	struct ReferenceBool
	{
		TESObjectREFR* ref;
		bool isActivationNotPrevented;
	};

	UInt32 ActionToTake()
	{
		const auto alt = keyAlt && IsKeyPressed(keyAlt, DIHookControl::kFlag_RawState) || buttonAlt && IsButtonPressed(buttonAlt);
		if (key1 && IsKeyPressed(key1, DIHookControl::kFlag_RawState) || button1 && IsButtonPressed(button1)) return alt ? mode1Alt : mode1;
		if (key2 && IsKeyPressed(key2, DIHookControl::kFlag_RawState) || button2 && IsButtonPressed(button2)) return alt ? mode2Alt : mode2;
		if (key3 && IsKeyPressed(key3, DIHookControl::kFlag_RawState) || button3 && IsButtonPressed(button3)) return alt ? mode3Alt : mode3;
		return kNone;
	}

	void OnPreActivate(TESObjectREFR* thisObj, void *parameters)
	{
		const auto ptr = static_cast<ReferenceBool*>(parameters);
		const auto action = ActionToTake();
		if (!container || thisObj != container || ptr->ref != g_player || !ptr->isActivationNotPrevented || action == kOpen)
		{
			SetNativeHandlerFunctionBool(true);
			return;
		}
		SetNativeHandlerFunctionBool(false);
		if (action == kNone) return;
		Action(action);
	}


	void HandleINI()
	{
		const auto iniPath = GetCurPath() + R"(\Data\Config\JustMods.ini)";
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return; 

		enable = ini.GetOrCreate("JustMods", "bLootMenu", 1, nullptr);

		key1Base		= ini.GetOrCreate("JLM", "iKey1", 0, nullptr);
		key2Base		= ini.GetOrCreate("JLM", "iKey2", 0, nullptr);
		key3Base		= ini.GetOrCreate("JLM", "iKey3", 0, nullptr);
		keyAltBase		= ini.GetOrCreate("JLM", "iKeyAlt", 0, nullptr);
		keyScrollUp		= ini.GetOrCreate("JLM", "iKeyScrollUp", 264, nullptr);
		keyScrollDown	= ini.GetOrCreate("JLM", "iKeyScrollDown", 265, nullptr);

		button1Base		= ini.GetOrCreate("JLM", "iButton1", 0, nullptr);
		button2Base		= ini.GetOrCreate("JLM", "iButton2", 0, nullptr);
		button3Base		= ini.GetOrCreate("JLM", "iButton3", 0, nullptr);
		buttonAltBase	= ini.GetOrCreate("JLM", "iButtonAlt", 0, nullptr);

		mode1			= ini.GetOrCreate("JLM", "iMode1", kTake, nullptr);
		mode1Alt		= ini.GetOrCreate("JLM", "iMode1Alt", kEquip, nullptr);
		mode2			= ini.GetOrCreate("JLM", "iMode", kOpen, nullptr);
		mode2Alt		= ini.GetOrCreate("JLM", "iMode2Alt", kTakeAll, nullptr);
		mode3			= ini.GetOrCreate("JLM", "iMode3", kNone, nullptr);
		mode3Alt		= ini.GetOrCreate("JLM", "iMode3Alt", kNone, nullptr);

		takeSmartMin	= ini.GetOrCreate("JLM", "iTakeSmartMin", 5, nullptr);
		takeWeightless	= ini.GetOrCreate("JLM", "bTakeWeightless", true, nullptr);

		block			= ini.GetOrCreate("JLM", "bBlockBeforeActivate", false, nullptr);
		overScroll		= ini.GetOrCreate("JLM", "bOverScroll", false, nullptr);
		hidePrompt		= ini.GetOrCreate("JLM", "bHidePrompt", true, nullptr);
		hideName		= ini.GetOrCreate("JLM", "bHideName", false, nullptr);
		itemsMax		= ini.GetOrCreate("JLM", "iItemsMax", 5, nullptr);

		justify			= ini.GetOrCreate("JLM", "iJustify", 3, nullptr);
		heightMin		= ini.GetOrCreate("JLM", "fHeightMin", 32.0, nullptr);
		heightMax		= ini.GetOrCreate("JLM", "fHeightMax", 640.0, nullptr);
		widthMin		= ini.GetOrCreate("JLM", "fWidthMin", 400.0, nullptr);
		widthMax		= ini.GetOrCreate("JLM", "fWidthMax", 640.0, nullptr);
		offsetX			= ini.GetOrCreate("JLM", "fOffsetX", 0.625, nullptr);
		offsetY			= ini.GetOrCreate("JLM", "fOffsetY", 0.5, nullptr);

		indentItem		= ini.GetOrCreate("JLM", "fIndentItem", 8.0, nullptr);
		indentTextX		= ini.GetOrCreate("JLM", "fIndentTextX", 10.0, nullptr);
		indentTextY		= ini.GetOrCreate("JLM", "fIndentTextY", 10.0, nullptr);

		weightVisible	= ini.GetOrCreate("JLM", "iWeightVisible", 2, nullptr);
		weightAltColor	= ini.GetOrCreate("JLM", "iWeightAltColor", 1, nullptr);
		font			= ini.GetOrCreate("JLM", "iFont", 0, nullptr);
		fontHead		= ini.GetOrCreate("JLM", "iFontHead", 0, nullptr);
		fontY			= ini.GetOrCreate("JLM", "fFontY", 0.0, nullptr);
		fontHeadY		= ini.GetOrCreate("JLM", "fFontHeadY", 0.0, nullptr);

		sounds			= ini.GetOrCreate("JLM", "bSounds", true, nullptr);
		showEquip		= ini.GetOrCreate("JLM", "bShowEquip", true, nullptr);
		showIcon		= ini.GetOrCreate("JLM", "bShowIcon", true, nullptr);
		showMeter		= ini.GetOrCreate("JLM", "bShowMeter", true, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			std::erase(onRender, OnRender);
			std::erase(onAddDrop, OnAddDropUpdate);
			return;
		}
		onRender.emplace_back(OnRender);
		onAddDrop.emplace_back(OnAddDropUpdate);
		SetEventHandler("ShowOff:OnPreActivate", OnPreActivate);

		tileMain->SetFloat("_FontBase", font);
		tileMain->SetFloat("_FontYBase", fontY);
		tileMain->SetFloat("_FontHeadBase", fontHead);
		tileMain->SetFloat("_FontHeadYBase", fontHeadY);

		tileMain->SetFloat("_ItemHeightMin", heightMin);
		tileMain->SetFloat("_ItemHeightMax", heightMax);
		tileMain->SetFloat("_ItemWidthMin", widthMin);
		tileMain->SetFloat("_ItemWidthMax", widthMax);

		tileMain->SetFloat("_OffsetX", offsetX);
		tileMain->SetFloat("_OffsetY", offsetY);

		tileMain->SetFloat("_ItemIndent", indentItem);
		tileMain->SetFloat("_TextIndentX", indentTextX);
		tileMain->SetFloat("_TextIndentY", indentTextY);

		tileMain->SetFloat("_BracketTitle", 1 - hideName);

		tileMain->SetFloat("_ShowEquip", showEquip);
		tileMain->SetFloat("_ShowIcon", showIcon);
		tileMain->SetFloat("_ShowMeter", showMeter);

		tileMain->SetFloat("_JustifyX", justify % 3);
		tileMain->SetFloat("_JustifyY", floor(justify / 3));

		tileMain->SetFloat("_WeightVisible", 0);

		if (tileMain->GetChild("JLMContainer")) tileMain->GetChild("JLMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JLMContainer");

		for (UInt32 i = 0; i < itemsMax; i++)	tileMain->GetChild("JLMContainer")->AddTileFromTemplate("JLMInjected");

		if (!formEVE) formEVE = GetFormByID("EVE FNV - ALL DLC.esp", 0x02E612);
		if (!formEVE) formEVE = GetFormByID("EVE FNV - NO DLC.esp", 0x02E612);
		if (!formEVE) formEVE = GetFormByID("EVE FNV - NO GRA.esp", 0x02E612);
		if (!formEVE) formEVE = GetFormByID("RA-Gear.esm", 0x02E612);

		if (!formShovel)		formShovel = GetFormByID("PointLookout.esm", 0x0082B5);
		if (!formShovelUnique)	formShovelUnique = GetFormByID("PointLookout.esm", 0x00D5A8);

		key1 = key1Base ? key1Base : GetControl(5, OSInputGlobals::kControlType_Keyboard);
		key2 = key2Base ? key2Base : GetControl(7, OSInputGlobals::kControlType_Keyboard);
		key3 = key3Base ? key3Base : GetControl(16, OSInputGlobals::kControlType_Keyboard);
		keyAlt = keyAltBase ? keyAltBase : GetControl(9, OSInputGlobals::kControlType_Keyboard);

		button1 = button1Base ? button1Base : GetControl(5, OSInputGlobals::kControlType_Joystick);
		button2 = button2Base ? button2Base : GetControl(7, OSInputGlobals::kControlType_Joystick);
		button3 = button3Base ? button3Base : GetControl(16, OSInputGlobals::kControlType_Joystick);
		buttonAlt = buttonAltBase ? buttonAltBase : GetControl(9, OSInputGlobals::kControlType_Joystick);

		if (CdeclCall<bool>(0x4B71D0))
		{
			GetStringForButton(button1).empty() ? tileMain->SetFloat("_Button1", button1) : tileMain->SetString("_KeyString1", button1 ? (GetStringForButton(button1) + ")").c_str() : "");
			GetStringForButton(button2).empty() ? tileMain->SetFloat("_Button2", button2) : tileMain->SetString("_KeyString2", button2 ? (GetStringForButton(button2) + ")").c_str() : "");
			GetStringForButton(button3).empty() ? tileMain->SetFloat("_Button3", button3) : tileMain->SetString("_KeyString3", button3 ? (GetStringForButton(button3) + ")").c_str() : "");
		}
		else
		{
			tileMain->SetString("_KeyString1", key1 ? (GetDXDescription(key1) + ")").c_str() : "");
			tileMain->SetString("_KeyString2", key2 ? (GetDXDescription(key2) + ")").c_str() : "");
			tileMain->SetString("_KeyString3", key3 ? (GetDXDescription(key3) + ")").c_str() : "");
		}
	}


	void MainLoopDoOnce()
	{
		tileMain = g_HUDMainMenu->tile->GetChild("JLM");
		if (!tileMain)
		{
			g_HUDMainMenu->tile->InjectUIXML(R"(Data\menus\yUI\LootMenu.xml)");
			tileMain = g_HUDMainMenu->tile->GetChild("JLM");
		}
		if (!tileMain) return;
		RegisterEvent("JLM:Reset", 0, nullptr, 4);
		SetEventHandler("JLM:Reset", Reset);
		Reset();
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;

		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		mainLoop.emplace_back(MainLoop);
	}
}
