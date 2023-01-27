#include <main.h>
#include <functions.h>
#include <SimpleINILibrary.h>
#include <json.h>

#include <dinput8.h>

#include <Menus.h>
#include <GameData.h>
#include <InterfaceManager.h>

#include "GameSettings.h"
#include "SortingIcons/SortingIcons.h"

namespace UserInterface::LootMenu
{

	bool		enable			= false;

	UInt32		logLevel		= 3;

	enum kActions
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

	Float32		speed			= 0.05;

	bool		sounds			= true;
	bool		showEquip		= true;
	bool		showIcon		= true;
	bool		showMeter		= true;
	bool		showName		= true;
	bool		hidePrompt		= true;

	bool		block			= false;
	bool		overScroll		= false;

	Tile*		tileMain		= nullptr;

	std::vector<InventoryChanges*> items;

	/* those are needed to know if ref should be updated */
	TESObjectREFR*		ref					= nullptr;
	TESObjectREFR*		container			= nullptr;

	bool				owned				= false;

	TESForm*			formEVE				= nullptr;
	TESForm*			formVendorScript	= nullptr;

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

	bool				blockActivation		= false;

	TESObjectREFR*		tookItem			= nullptr;
	TESObjectREFR*		openedContainer		= nullptr;

	CommandInfo*		cmd_Activate		= nullptr;
	CommandInfo*		cmd_OnActivate		= nullptr;

	enum kScriptType
	{
		kDisallow = -1,
		kCheck = 0,
		kAllow = 1,
	};

	namespace ScriptChecker
	{
		std::unordered_map<Script*, SInt8> allowList;

		UInt32 StrToFormID(const std::string& formIdStr)
		{
			const auto formId = HexStringToInt(formIdStr);
			if (formId == -1) Log(logLevel) << "Form field was incorrectly formatted, got " + formIdStr;
			return formId;
		}

		std::vector<TESForm*> GetFormsFromElement(const nlohmann::basic_json<>& elem, const std::string& mod, const std::string& form)
		{
			const auto modName = elem.contains(mod) ? elem[mod].get<std::string>() : "";
			std::vector<TESForm*> forms{};
			if (!elem.contains(form)) {}
			else if (!elem[form].is_array()) {
				if (const auto val = TESForm::GetByID(modName.c_str(), elem[form].get<std::string>().c_str())) forms.push_back(val);
			}
			else for (const auto& i : elem[form]) {
				if (const auto val = TESForm::GetByID(modName.c_str(), i.get<std::string>().c_str())) forms.push_back(val);
			}
			return forms;
		}

		void AddFromJSON(nlohmann::basic_json<> elem)
		{
			if (!elem.is_object()) return;

			SInt8 action = 0;

			if (elem.contains("action")) action = elem["action"].get<SInt8>();
			if (elem.contains("mod") && elem.contains("form"))
				for (const auto form : GetFormsFromElement(elem, "mod", "form"))
					allowList[reinterpret_cast<Script*>(form)] = action;
		}

		SInt8 Check(Script* script)
		{
			if (!script) return kAllow;
			if (allowList.contains(script)) return allowList[script];
			if (HasScriptCommand(script, cmd_Activate, cmd_OnActivate)) allowList[script] = kCheck;
			else if (HasScriptCommand(script, cmd_OnActivate, nullptr)) allowList[script] = kDisallow;
			else allowList[script] = kAllow;
			return allowList[script];
		}
	}

	SInt8 CheckContainer(const TESObjectREFR* container)
	{
		return ScriptChecker::Check(container->baseForm->GetScript());
	}

	bool ShouldAllowContainerInteractions()
	{
		return container && !((block || CheckContainer(container) == kCheck) && blockActivation);
	}

	namespace Items
	{
		void Update()
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

				const auto fontManager = FontManager::GetSingleton();
				const auto font = tileMain->GetFloat("_Font");
				const auto wrapWidth = fst->GetChild("ButtonText")->GetFloat(kTileValue_wrapwidth);

				const auto stringDimensions = fontManager->GetStringDimensions(string.c_str(), font, wrapWidth);

				const auto height = indentTextY + stringDimensions->y;

				x = max(x, stringDimensions->x + fst->GetFloat("_TextWidth"));

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

			tileMain->SetFloat("_ArrowUp", offset > 0);
			tileMain->SetFloat("_ArrowDown", offset < items.size() - tiles);
		}
	}

	namespace Box
	{
		bool isShown = false;

		void Show()
		{
			owned = container->IsCrimeOrEnemy();
			tileMain->SetString("_Title", container->GetTheName());
			tileMain->SetFloat("_SystemColor", 1 + owned);

			Items::Update();

			if (isShown) return;

			ToggleVanityWheel(false);
			if (key1) DisableKey(key1, true);
			if (key2) DisableKey(key2, true);
			if (key3) DisableKey(key3, true);
			if (keyAlt) DisableKey(keyAlt, true);

			tileMain->SetFloat("_AlphaAC", HUDMainMenu::GetOpacity());
			tileMain->GradualSetFloat("_AlphaMult", 0, 1, speed, 0);

			isShown = true;
		}

		void Hide()
		{
			if (!isShown) return;

			ToggleVanityWheel(true);
			if (key1) DisableKey(key1, false);
			if (key2) DisableKey(key2, false);
			if (key3) DisableKey(key3, false);
			if (keyAlt) DisableKey(keyAlt, false);

			tileMain->GradualSetFloat("_AlphaMult", 1, 0, 2 * speed, 0);

			isShown = false;
		}

		UInt32 typeCodes[]{ 116, 46, 49, 25, 115, 108, 41, 40, 103, 24, 26, 47, 31 };

		UInt16 CompareItems(InventoryChanges* first, InventoryChanges* second)
		{
			ListBoxItem first_(first);
			ListBoxItem second_(second);
			return CdeclCall<SInt16>(0x7824E0, &first_, &second_) != 1 ? 1 : 0;
		}

		void Update()
		{
			const bool var = ShouldAllowContainerInteractions();

			items.clear();
			if (var)
			{
				if (formEVE) container->RemoveItem(formEVE, nullptr, 1, 0, 0, nullptr, 0, 0, 0, 0);
				bool shouldCheckRecursively = false;
				if (const auto script = container->baseForm->GetScript())
					shouldCheckRecursively = script->refID == formVendorScript->refID;
				items = container->GetAllItems(shouldCheckRecursively);
				ra::sort(items, CompareItems);
			}

			if (hidePrompt)
			{
				if (!info && var || info && !var)
				{
					info = !info;
					info ? HideInfoPrompt::Hide() : HideInfoPrompt::Show();
				}
				HideInfoPrompt::Update();
			}

			var ? Show() : Hide();
		}
	}

	namespace Ref
	{
		void ChangeContainer(TESObjectREFR* newcontainer)
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

		bool				notcannibal			= false;
		bool				dead				= false;
		bool				open				= false;

		bool NameNotEmpty(TESObjectREFR* ref)
		{
			return !std::string(ref->GetTheName()).empty();
		}

		bool IsDead(TESObjectREFR* ref)
		{
			return ref->IsActor() ? (reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dead || reinterpret_cast<Actor*>(ref)->lifeState == kLifeState_Dying) : true;
		}

		void Update(TESObjectREFR* newref)
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

			ChangeContainer(ref);
			Box::Update();
		}
	}

	namespace Scroll
	{
		void Update()
		{
			bool update = false;

			if (!IsKeyPressed(keyScrollDown)) {}
			else if (index + 1 < tiles) { update = true; index++; }
			else if (offset < items.size() - tiles) { update = true; offset++; }
			else if (overScroll) { update = true; offset = 0; index = 0; }

			if (!IsKeyPressed(keyScrollUp)) {}
			else if (index > 0) { update = true; index--; }
			else if (offset > 0) { update = true; offset--; }
			else if (overScroll) { update = true; offset = items.size() - tiles; index = tiles - 1; }

			if (update)
			{
				PlayGameSound("UIPipBoyScroll");
				Items::Update();
			}
		}
	}

	namespace Weight
	{
		void Update()
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
	}

	namespace Keys
	{
		bool lockTake = true;

		void Update()
		{
			if (!ShouldAllowContainerInteractions())
			{
				lockTake = false;
				return;
			}

			if ((!key1 || !IsKeyPressed(key1, DIHookControl::kFlag_RawState)) && (!button1 || !IsButtonPressed(button1)) &&
				(!key2 || !IsKeyPressed(key2, DIHookControl::kFlag_RawState)) && (!button2 || !IsButtonPressed(button2)) &&
				(!key3 || !IsKeyPressed(key3, DIHookControl::kFlag_RawState)) && (!button3 || !IsButtonPressed(button3)))
				lockTake = true;
			else if (lockTake)
			{
				lockTake = false;
				container->Activate(g_player, 0, 0, 1);
			}

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
	}

	void Update()
	{
		Ref::Update(MenuMode() ? nullptr : InterfaceManager::GetSingleton()->crosshairRef);
		Keys::Update();
		if (ShouldAllowContainerInteractions())
		{
			Weight::Update();
			Scroll::Update();
			if (hidePrompt) HideInfoPrompt::Update();
		}
	}

	namespace Activate
	{
		Cmd_Execute cmd_Activate_Execute = nullptr;

		void Fix()
		{
			if (cmd_Activate_Execute) cmd_Activate->execute = cmd_Activate_Execute;
			cmd_Activate_Execute = nullptr;
		}

		bool Cmd_ActivateAlt(COMMAND_ARGS)
		{
			if (!container || thisObj != container) return cmd_Activate_Execute(PASS_COMMAND_ARGS);
			Fix();
			blockActivation = false;
			if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }
			Box::Update();
			return true;
		}

		void Replace()
		{
			cmd_Activate_Execute = cmd_Activate->execute;
			cmd_Activate->execute = Cmd_ActivateAlt;
		}
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

	void HandleTakeItem(InventoryChanges* entry, bool all, bool keepowner, bool equip)
	{
		const auto form = entry->form->TryGetREFRParent();
		if (entry->form->GetIsReference()) {
			g_player->HandlePickupItem(reinterpret_cast<TESObjectREFR*>(entry->form), all ? entry->countDelta : 1, keepowner);
			g_player->SendStealingAlarm(reinterpret_cast<TESObjectREFR*>(entry->form), false);
		}
		else {
			container->RemoveItem(entry->form, entry->extendData ? entry->extendData->GetFirstItem() : nullptr, (all || form->typeID == kFormType_TESAmmo) ? entry->countDelta : 1, !container->IsActor(), 0, g_player, 0, 0, 1, 0);
			tookItem = container;
		}
		if (!keepowner && equip) g_player->EquipItem(form);
	}

	bool Action(UInt32 action)
	{
		if (action == kOpen) return true;

		if (action == kNone) {
			if (!blockActivation) return false;
			if (CheckContainer(container) == kCheck)
			{
				Activate::Replace();
				return true;
			}
			blockActivation = false;
			if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }
			Box::Update();
			return false;
		}

		if (!openedContainer) { container->OpenCloseContainer(true, sounds & 0x1); openedContainer = container; }

		const auto keepowner = owned;

		if (action == kTakeAll)
		{
			PlayGameSound("UIItemTakeAll");
			if (items.empty()) return false;
			for (const auto item : items) HandleTakeItem(item, true, keepowner, false);
			Box::Update();
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

		Box::Update();
		return false;
	}

	bool update = true;
	void MainLoop()	{ if (update) { update = false; Update(); } update = true; }
	void OnRender() { if (update) { update = false; Update(); } }
	void OnAddDropUpdate(TESObjectREFR* thisObj, TESObjectREFR* droppedFrom, TESForm* noidea) { if (container && droppedFrom == container) Box::Update(); }

	bool OnPreActivate(TESObjectREFR* thisObj, Actor* ref, bool isActivationNotPrevented)
	{
		if (!container) Ref::Update(thisObj);
		if (!container || thisObj != container || ref != g_player || !isActivationNotPrevented) return true;
		return Action(ActionToTake());
	}

	void HandleJSON()
	{
		const auto dir = GetCurPath() + R"(\Data\menus\yUI)";
		if (!std::filesystem::exists(dir)) Log(Log::kLog | logLevel) << (dir + " does not exist.");
		else for (const auto& iter : std::filesystem::directory_iterator(dir))
			if (iter.is_directory()) Log(logLevel) << iter.path().string() + " found";
			else if (iter.path().extension().string() == ".json")
			{
				try
				{
					std::ifstream i(iter.path());
					auto j = nlohmann::json::parse(i, nullptr, true, true);
					if (!j.contains("lootMenu") || !j["lootMenu"].is_array()) 
						Log(logLevel) << (iter.path().string() + " JSON ySI item array not detected");
					else for (const auto& elem : j["lootMenu"]) ScriptChecker::AddFromJSON(elem);
				}
				catch (nlohmann::json::exception& e)
				{
					Log(logLevel) << ("The JSON is incorrectly formatted! It will not be applied.");
					Log(logLevel) << (FormatString("JSON error: %s\n", e.what()));
				}
			}
	}

	void HandleINI()
	{
		const auto iniPath = GetCurPath() + yUI_INI;
		CSimpleIniA ini;
		ini.SetUnicode();

		if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return; 

		enable			= ini.GetOrCreate("General", "bLootMenu", true, "; enable 'Loot Menu' feature. If required files are not found this will do nothing.");

		block			= ini.GetOrCreate("Loot Menu", "bOnlyOpenLootMenuAfterFirstInteraction", false, "; I recommend considering enabling this option, as it's a little less performance intensive and as bug-free as regular container menu");

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
		showName		= ini.GetOrCreate("Loot Menu", "bShowName", true, nullptr);
		hidePrompt		= ini.GetOrCreate("Loot Menu", "bHidePrompt", true, nullptr);

		overScroll		= ini.GetOrCreate("Loot Menu", "bOverScroll", false, nullptr);

		ini.SaveFile(iniPath.c_str(), false);
	}

	void ProcessGlobals()
	{
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

		tileMain->SetFloat("_BracketTitle", showName);

		tileMain->SetFloat("_ShowEquip", showEquip);
		tileMain->SetFloat("_ShowIcon", showIcon);
		tileMain->SetFloat("_ShowMeter", showMeter);

		tileMain->SetFloat("_JustifyX", justify % 3);
		tileMain->SetFloat("_JustifyY", floor(justify / 3));

		tileMain->SetFloat("_WeightVisible", 0);

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

		ProcessGlobals();

		if (tileMain->GetChild("JLMContainer")) tileMain->GetChild("JLMContainer")->Destroy(true);
		tileMain->AddTileFromTemplate("JLMContainer");
		for (UInt32 i = 0; i < itemsMax; i++) tileMain->GetChild("JLMContainer")->AddTileFromTemplate("JLMInjected");
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

	void DeferredInit()
	{
		if (!formVendorScript) formVendorScript = TESForm::GetByID("FalloutNV.esm", 0x07D4E3);

		if (!formEVE) formEVE = TESForm::GetByID("EVE FNV - ALL DLC.esp", 0x02E612);
		if (!formEVE) formEVE = TESForm::GetByID("EVE FNV - NO DLC.esp", 0x02E612);
		if (!formEVE) formEVE = TESForm::GetByID("EVE FNV - NO GRA.esp", 0x02E612);
		if (!formEVE) formEVE = TESForm::GetByID("RA-Gear.esm", 0x02E612);

		cmd_Activate = GetByOpcode(0x100D);
		cmd_OnActivate = GetEventCommandInfo(0x2);

		HandleJSON();
	}

	extern void Init()
	{
		if (g_nvseInterface->isEditor) return;

		deferredInit.emplace_back(DeferredInit);
		mainLoopDoOnce.emplace_back(MainLoopDoOnce);
		HandleINI();
	}
}