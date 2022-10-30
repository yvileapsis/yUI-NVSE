#include <main.h>
#include <functions.h>
#include <SimpleINILibrary.h>

#include <dinput8.h>

#include <Menus.h>
#include <InterfaceManager.h>

#include "SafeWrite.h"
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
	UInt32				tiles				= 0;

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

	bool				blockActivation		= true;

	TESObjectREFR*		tookItem			= nullptr;
	TESObjectREFR*		openedContainer		= nullptr;

	CommandInfo*		cmd_Activate		= nullptr;
	CommandInfo*		cmd_OnActivate		= nullptr;
	Cmd_Execute			cmd_Activate_Execute = nullptr;

	enum SInt8
	{
		kDisallow = -1,
		kCheck = 0,
		kAllow = 1,
	};

	std::unordered_map<Script*, SInt8> allowList;

	SInt8 CheckScript(Script* script)
	{
		if (!script) return kAllow;
		if (allowList.contains(script)) return allowList[script];
		if (HasScriptCommand(script, cmd_Activate, cmd_OnActivate)) allowList[script] = kCheck;
		else if (HasScriptCommand(script, cmd_OnActivate, nullptr)) allowList[script] = kDisallow;
		else allowList[script] = kAllow;
		return allowList[script];
	}

	SInt8 CheckContainer(const TESObjectREFR* container)
	{
		return CheckScript(container->baseForm->GetScript());
	}

	bool ShouldAllowContainerInteractions()
	{
		return container && !((block || CheckContainer(container) == kCheck) && blockActivation);
	}

	bool NameNotEmpty(TESObjectREFR* ref)
	{
		return !std::string(ref->GetTheName()).empty();
	}

	bool IsDead(TESObjectREFR* ref)
	{
		return ref->IsActor() ? (reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dead || reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dying) : true;
	}

	void ShowItems()
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

			const auto category = *SortingIcons::Categories::ItemGetCategory(snd);
			if (!category.filename.empty())
			{
				fst->SetFloat("_Icon", true);
				fst->SetString("_IconFilename", category.filename.c_str());
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

	UInt32 typeCodes[]{ 116, 46, 49, 25, 115, 108, 41, 40, 103, 24, 26, 47, 31 };

	UInt16 CompareItems(InventoryChanges* first, InventoryChanges* second)
	{
		TileInventoryChangesUnk first_(first);
		TileInventoryChangesUnk second_(second);
		return CdeclCall<SInt16>(0x7824E0, &first_, &second_) != 1 ? 1 : 0;
	}

	void Hide()
	{
		tileMain->SetFloat("_Visible", 0);
		ToggleVanityWheel(true);
		if (key1) DisableKey(key1, false);
		if (key2) DisableKey(key2, false);
		if (key3) DisableKey(key3, false);
		if (keyAlt) DisableKey(keyAlt, false);
	}

	void Show()
	{
		ToggleVanityWheel(false);
		if (key1) DisableKey(key1, true);
		if (key2) DisableKey(key2, true);
		if (key3) DisableKey(key3, true);
		if (keyAlt) DisableKey(keyAlt, true);

		owned = container->IsCrimeOrEnemy();
		tileMain->SetString("_Title", container->GetTheName());
		tileMain->SetFloat("_SystemColor", 1 + owned);
		tileMain->SetFloat("_AlphaAC", g_HUDMainMenu->tileActionPointsMeterText1->GetFloat(kTileValue_alpha));

		ShowItems();

		tileMain->SetFloat("_ArrowUp", offset > 0);
		tileMain->SetFloat("_ArrowDown", offset < items.size() - tiles);

		tileMain->SetFloat("_Visible", 1);
	}

	void Update()
	{
		items.clear();
		if (container)
		{
			if (formEVE) container->RemoveItem(formEVE, nullptr, 1, 0, 0, nullptr, 0, 0, 0, 0);
			items = container->GetAllItems();
			ra::sort(items, CompareItems);
		}

		const bool var = ShouldAllowContainerInteractions();
		if (hidePrompt && (!info && var || (info && !var)))
		{
			info = !info;
			SetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, (!info ? -1 : 1) + GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0));
		}

		if (hidePrompt) g_HUDMainMenu->tileInfo->SetFloat(kTileValue_visible, GetJIPAuxVarOrDefault("*_JAMInfoDisable", 0, 0) == 0);

		var ? Show() : Hide();
	}

	void HandleContainerChange(TESObjectREFR* newcontainer)
	{
		if (container != newcontainer)
		{
			if (tookItem) { g_player->SendStealingAlarm(tookItem, true); tookItem = nullptr; }
			if (openedContainer) { container->OpenCloseContainer(false, sounds & 0x1); openedContainer = nullptr; }
		}

		offset = 0;
		index = 0;

		blockActivation = true;

		container = newcontainer;
	}

	void UpdateContainer(TESObjectREFR* newref)
	{
		bool update = false;

		if (ref != newref) update = true;
		ref = newref;

		if (ref)
		{
			ref = ref->ResolveAshpile();

			if (owned != ref->IsCrimeOrEnemy())			{ update = true; owned = !owned; }
			if (dead != IsDead(ref))					{ update = true; dead = !dead; }
			if (open != !ref->IsLocked())				{ update = true; open = !open; }
			if (notcannibal != GetCannibalPrompt(ref))	{ update = true; notcannibal = !notcannibal; }

			if (!ref->baseForm->CanContainItems() || CheckContainer(ref) == kDisallow) ref = nullptr;
		}

		if (!update) return;

		ref = ref && dead && open && notcannibal && NameNotEmpty(ref) ? ref : nullptr;

		HandleContainerChange(ref);
		Update();
	}

	void HandleScroll()
	{
		if (!ShouldAllowContainerInteractions()) return;

		bool update = false;

		if (!IsKeyPressed(keyScrollDown)) {}
		else if (index + 1 < tiles)				{ update = true; index++; }
		else if (offset < items.size() - tiles)	{ update = true; offset++; }
		else if (overScroll)					{ update = true; offset = 0; index = 0; }

		if (!IsKeyPressed(keyScrollUp)) {}
		else if (index > 0)						{ update = true; index--; }
		else if (offset > 0)					{ update = true; offset--; }
		else if (overScroll)					{ update = true; offset = items.size() - tiles; index = tiles - 1; }

		if (update)
		{
			PlayGameSound("UIPipBoyScroll");
			Show();
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
			g_player->SendStealingAlarm(static_cast<TESObjectREFR*>(entry->form), false);
		} else {
			container->RemoveItem(entry->form, entry->extendData ? entry->extendData->GetFirstItem() : nullptr, (all ||  form->typeID == kFormType_TESAmmo) ? entry->countDelta : 1, !container->IsActor(), 0, g_player, 0, 0, 1, 0);
			tookItem = container;
		}
		if (!keepowner && equip) g_player->EquipItem(form);
	}

	void FixActivate()
	{
		if (cmd_Activate_Execute) cmd_Activate->execute = cmd_Activate_Execute;
		cmd_Activate_Execute = nullptr;
	}

	bool Cmd_ActivateAlt(COMMAND_ARGS)
	{
		if (!container || thisObj != container) return cmd_Activate_Execute(PASS_COMMAND_ARGS);
		FixActivate();
		blockActivation = false;
		if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }
		Update();
		return true;
	}

	void ReplaceActivate()
	{
		cmd_Activate_Execute = cmd_Activate->execute;
		cmd_Activate->execute = Cmd_ActivateAlt;
	}

	UInt32 ActionToTake()
	{
		if (!ShouldAllowContainerInteractions()) return kNone;
		const auto alt = keyAlt && IsKeyPressed(keyAlt, DIHookControl::kFlag_RawState) || buttonAlt && IsButtonPressed(buttonAlt);
		if (key1 && IsKeyPressed(key1, DIHookControl::kFlag_RawState) || button1 && IsButtonPressed(button1)) return alt ? mode1Alt : mode1;
		if (key2 && IsKeyPressed(key2, DIHookControl::kFlag_RawState) || button2 && IsButtonPressed(button2)) return alt ? mode2Alt : mode2;
		if (key3 && IsKeyPressed(key3, DIHookControl::kFlag_RawState) || button3 && IsButtonPressed(button3)) return alt ? mode3Alt : mode3;
		return kNone;
	}

	bool Action(UInt32 action)
	{
		if (action == kOpen) return true;

		if (action == kNone) {
			if (!blockActivation) return false;
			if (CheckScript(ref->baseForm->GetScript()) == kCheck)
			{
				ReplaceActivate();
				return true;
			}
			blockActivation = false;
			if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }
			Update();
			return false;
		}

		if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }

		const auto keepowner = owned;

		if (action == kTakeAll)
		{
			PlayGameSound("UIItemTakeAll");
			if (items.empty()) return false;
			for (const auto item : items) HandleTakeItem(item, true, keepowner, false);
			Update();
			return false;
		}

		if (items.empty())
		{
			PlayGameSound("UIActivateNothing");
			return false;
		}

		const auto item = items.begin() + index + offset;
		if (item >= items.end()) return false;
		const auto entry = *item;

		bool takeall = false;
		if (CdeclCall<Float64>(0x48EBC0, entry->form, g_player->isHardcore) < 0.02) takeall = true;
		if (entry->countDelta > takeSmartMin) takeall = true;

		g_player->PlayPickupPutdownSounds(entry->form, action != kEquip, action == kEquip);
		HandleTakeItem(entry, takeall, keepowner, action == kEquip);

		Update();
		return false;
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

	void MainLoop()
	{
		if (MenuMode()) UpdateContainer(nullptr);
	}

	void OnRender()
	{
		UpdateContainer(!MenuMode() ? InterfaceManager::GetSingleton()->crosshairRef : nullptr);

		if (ShouldAllowContainerInteractions())
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
		else lockTake = false;
	}

	void OnAddDropUpdate(TESObjectREFR* thisObj, TESObjectREFR* droppedFrom, TESForm* noidea)
	{
		if (container && droppedFrom == container) Update();
	}

	bool OnPreActivate(TESObjectREFR* thisObj, Actor* ref, bool isActivationNotPrevented)
	{
		if (!container) UpdateContainer(thisObj);
		if (!container || thisObj != container || ref != g_player || !isActivationNotPrevented) return true;
		return Action(ActionToTake());
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

	void HandleINI()
	{
//		const auto iniPath = GetCurPath() + yUI_INI";
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();


		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return; 

		enable			= ini.GetOrCreate("General", "bLootMenu", true, "; enable 'Loot Menu' feature. If required files are not found this will do nothing.");

		key1Base		= ini.GetOrCreate("Loot Menu", "iKey1", 0, nullptr);
		key2Base		= ini.GetOrCreate("Loot Menu", "iKey2", 0, nullptr);
		key3Base		= ini.GetOrCreate("Loot Menu", "iKey3", 0, nullptr);
		keyAltBase		= ini.GetOrCreate("Loot Menu", "iKeyAlt", 0, nullptr);
		keyScrollUp		= ini.GetOrCreate("Loot Menu", "iKeyScrollUp", 264, nullptr);
		keyScrollDown	= ini.GetOrCreate("Loot Menu", "iKeyScrollDown", 265, nullptr);

		button1Base		= ini.GetOrCreate("Loot Menu", "iButton1", 0, nullptr);
		button2Base		= ini.GetOrCreate("Loot Menu", "iButton2", 0, nullptr);
		button3Base		= ini.GetOrCreate("Loot Menu", "iButton3", 0, nullptr);
		buttonAltBase	= ini.GetOrCreate("Loot Menu", "iButtonAlt", 0, nullptr);

		mode1			= ini.GetOrCreate("Loot Menu", "iMode1", kTake, nullptr);
		mode1Alt		= ini.GetOrCreate("Loot Menu", "iMode1Alt", kEquip, nullptr);
		mode2			= ini.GetOrCreate("Loot Menu", "iMode", kOpen, nullptr);
		mode2Alt		= ini.GetOrCreate("Loot Menu", "iMode2Alt", kTakeAll, nullptr);
		mode3			= ini.GetOrCreate("Loot Menu", "iMode3", kNone, nullptr);
		mode3Alt		= ini.GetOrCreate("Loot Menu", "iMode3Alt", kNone, nullptr);

		takeSmartMin	= ini.GetOrCreate("Loot Menu", "iTakeSmartMin", 5, nullptr);
		takeWeightless	= ini.GetOrCreate("Loot Menu", "bTakeWeightless", true, nullptr);

		block			= ini.GetOrCreate("Loot Menu", "bBlockBeforeActivate", false, nullptr);
		overScroll		= ini.GetOrCreate("Loot Menu", "bOverScroll", false, nullptr);
		hidePrompt		= ini.GetOrCreate("Loot Menu", "bHidePrompt", true, nullptr);
		hideName		= ini.GetOrCreate("Loot Menu", "bHideName", false, nullptr);
		itemsMax		= ini.GetOrCreate("Loot Menu", "iItemsMax", 5, nullptr);

		justify			= ini.GetOrCreate("Loot Menu", "iJustify", 3, nullptr);
		heightMin		= ini.GetOrCreate("Loot Menu", "fHeightMin", 32.0, nullptr);
		heightMax		= ini.GetOrCreate("Loot Menu", "fHeightMax", 640.0, nullptr);
		widthMin		= ini.GetOrCreate("Loot Menu", "fWidthMin", 400.0, nullptr);
		widthMax		= ini.GetOrCreate("Loot Menu", "fWidthMax", 640.0, nullptr);
		offsetX			= ini.GetOrCreate("Loot Menu", "fOffsetX", 0.625, nullptr);
		offsetY			= ini.GetOrCreate("Loot Menu", "fOffsetY", 0.5, nullptr);

		indentItem		= ini.GetOrCreate("Loot Menu", "fIndentItem", 8.0, nullptr);
		indentTextX		= ini.GetOrCreate("Loot Menu", "fIndentTextX", 10.0, nullptr);
		indentTextY		= ini.GetOrCreate("Loot Menu", "fIndentTextY", 10.0, nullptr);

		weightVisible	= ini.GetOrCreate("Loot Menu", "iWeightVisible", 2, nullptr);
		weightAltColor	= ini.GetOrCreate("Loot Menu", "iWeightAltColor", 1, nullptr);
		font			= ini.GetOrCreate("Loot Menu", "iFont", 0, nullptr);
		fontHead		= ini.GetOrCreate("Loot Menu", "iFontHead", 0, nullptr);
		fontY			= ini.GetOrCreate("Loot Menu", "fFontY", 0.0, nullptr);
		fontHeadY		= ini.GetOrCreate("Loot Menu", "fFontHeadY", 0.0, nullptr);

		sounds			= ini.GetOrCreate("Loot Menu", "bSounds", true, nullptr);
		showEquip		= ini.GetOrCreate("Loot Menu", "bShowEquip", true, nullptr);
		showIcon		= ini.GetOrCreate("Loot Menu", "bShowIcon", true, nullptr);
		showMeter		= ini.GetOrCreate("Loot Menu", "bShowMeter", true, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void Reset()
	{
		HandleINI();

		if (!enable)
		{
			std::erase(onRender, OnRender);
			std::erase(mainLoop, MainLoop);
			std::erase(onAddDrop, OnAddDropUpdate);
			std::erase(onPreActivate, OnPreActivate);
			return;
		}
		onRender.emplace_back(OnRender);
		mainLoop.emplace_back(MainLoop);
		onAddDrop.emplace_back(OnAddDropUpdate);
		onPreActivate.emplace_back(OnPreActivate);

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

	void PluginLoad()
	{
		cmd_Activate = GetByOpcode(0x100D);
		cmd_OnActivate = GetEventCommandInfo(0x2);
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;

		pluginLoad.emplace_back(PluginLoad);
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}
