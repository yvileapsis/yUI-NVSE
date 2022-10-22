#include "SI.h"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <GameAPI.h>
#include <GameData.h>
#include <GameRTTI.h>

#include <functions.h>
#include <json.h>

namespace SortingIcons::Files
{
	void ItemRecursiveEmplace(const Item::Common&, TESForm*);

	void HandleItem(nlohmann::basic_json<> elem)
	{
		constexpr auto strToFormID = [](const std::string& formIdStr)
		{
			const auto formId = HexStringToInt(formIdStr);
			if (formId == -1) Log("Form field was incorrectly formatted, got " + formIdStr);
			return formId;
		};

		if (!elem.is_object())
		{
			Log("JSON error: expected object");
			return;
		}
		Item::Common common{};

		common.tag = elem["tag"].get<std::string>();
		if (elem.contains("priority"))		common.priority = elem["priority"].get<SInt32>();
		if (elem.contains("formType"))		common.formType = elem["formType"].get<UInt32>();
		if (elem.contains("questItem"))		common.questItem = elem["questItem"].get<UInt8>();
		if (elem.contains("miscComponent"))	common.miscComponent = elem["miscComponent"].get<UInt8>();
		if (elem.contains("miscProduct"))	common.miscProduct = elem["miscProduct"].get<UInt8>();

		if (elem.contains("mod") && elem.contains("form"))
		{
			auto modName = elem.contains("mod") ? elem["mod"].get<std::string>() : "";
			const auto mod = !modName.empty() ? TESDataHandler::GetSingleton()->LookupModByName(modName.c_str()) : nullptr;

			UInt8 modIndex;
			if (modName == "FF") modIndex = 0xFF;
			else if (!mod && !modName.empty())
			{
				Log("Mod name " + modName + " was not found");
				return;
			}
			modIndex = mod->modIndex;

			UInt32 formlist = 0;
			if (elem.contains("formlist")) formlist = elem["formlist"].get<UInt32>();

			std::vector<int> formIds;
			if (const auto formElem = elem.contains("form") ? &elem["form"] : nullptr; formElem)
			{
				if (formElem->is_array())
					std::ranges::transform(*formElem, std::back_inserter(formIds), [&](auto& i) {return strToFormID(i.template get<std::string>()); });
				else
					formIds.push_back(strToFormID(formElem->get<std::string>()));
				if (std::ranges::find(formIds, -1) != formIds.end())
					return;
			}

			if (mod && !formIds.empty())
			{
				for (auto formId : formIds)
				{
					formId = (modIndex << 24) + (formId & 0x00FFFFFF);
					common.form = GetFormByID(formId);
					if (!common.form) { Log(FormatString("Form %X was not found", formId)); continue; }
					if (!formlist) {
						Log(FormatString("Tag: '%10s', form: %08X (%50s), individual", common.tag.c_str(), formId, common.form->GetName()));
						g_Items.emplace_back(common);
					}
					else if (formlist == 1) {
						ItemRecursiveEmplace(common, common.form);
					}
					else if (formlist == 2) {
						for (const auto item : *GetAllForms()) {
							if (item->typeID == kFormType_TESObjectWEAP) {
								const auto weapon = DYNAMIC_CAST(item, TESForm, TESObjectWEAP);
								if (!weapon || !weapon->repairItemList.listForm) continue;
								if (weapon->refID == common.form->refID || weapon->repairItemList.listForm->refID == common.form->refID || weapon->repairItemList.listForm->ContainsRecursive(common.form)) {
									Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X'", common.tag.c_str(), item->refID, item->GetName(), formId));
									g_Items.emplace_back(common, item);
								}
							}
							else if (item->typeID == kFormType_TESObjectARMO) {
								const auto armor = DYNAMIC_CAST(item, TESForm, TESObjectARMO);
								if (!armor || !armor->repairItemList.listForm) continue;
								if (armor->refID == common.form->refID || armor->repairItemList.listForm->refID == common.form->refID || armor->repairItemList.listForm->ContainsRecursive(common.form)) {
									Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X'", common.tag.c_str(), item->refID, item->GetName(), formId));
									g_Items.emplace_back(common, item);
								}
							}
						}
					}
				}
			}
			else {
				Log(FormatString("Tag: '%10s', mod: '%s'", common.tag.c_str(), modName.c_str()));
				g_Items.emplace_back(common);
			}
		}
		else if (common.formType == 40 || elem.contains("weaponSkill") || elem.contains("weaponType"))
		{
			common.formType = 40;
			Item::Weapon weapon{};

			if (elem.contains("weaponSkill"))			weapon.skill = elem["weaponSkill"].get<UInt32>();
			if (elem.contains("weaponHandgrip"))		weapon.handgrip = elem["weaponHandgrip"].get<UInt32>();
			if (elem.contains("weaponAttackAnim"))		weapon.attackAnim = elem["weaponAttackAnim"].get<UInt32>();
			if (elem.contains("weaponReloadAnim"))		weapon.reloadAnim = elem["weaponReloadAnim"].get<UInt32>();
			if (elem.contains("weaponIsAutomatic"))		weapon.isAutomatic = elem["weaponIsAutomatic"].get<UInt32>();
			if (elem.contains("weaponHasScope"))		weapon.hasScope = elem["weaponHasScope"].get<UInt32>();
			if (elem.contains("weaponIgnoresDTDR"))		weapon.ignoresDTDR = elem["weaponIgnoresDTDR"].get<UInt32>();
			if (elem.contains("weaponClipRounds"))		weapon.clipRounds = elem["weaponClipRounds"].get<UInt32>();
			if (elem.contains("weaponNumProjectiles"))	weapon.numProjectiles = elem["weaponNumProjectiles"].get<UInt32>();
			if (elem.contains("weaponSoundLevel"))		weapon.soundLevel = elem["weaponSoundLevel"].get<UInt32>();

			if (elem.contains("ammoMod") && elem.contains("ammoForm"))
			{
				auto ammoModName = elem["ammoMod"].get<std::string>();
				const auto ammoMod = !ammoModName.empty() ? TESDataHandler::GetSingleton()->LookupModByName(ammoModName.c_str()) : nullptr;
				auto ammoForm = GetFormByID((ammoMod->modIndex << 24) + (strToFormID(elem["ammoForm"].get<std::string>()) & 0x00FFFFFF));
				weapon.ammo = ammoForm;
			}

			std::vector<int> weaponTypes;
			if (auto* weaponTypeElem = elem.contains("weaponType") ? &elem["weaponType"] : nullptr; weaponTypeElem)
			{
				if (weaponTypeElem->is_array())
					std::ranges::transform(*weaponTypeElem, std::back_inserter(weaponTypes), [&](auto& i) {return i.template get<UInt32>(); });
				else
					weaponTypes.push_back(weaponTypeElem->get<UInt32>());
				if (std::ranges::find(weaponTypes, -1) != weaponTypes.end())
					return;
			}

			if (!weaponTypes.empty())
			{
				for (auto weaponType : weaponTypes)
				{
					weapon.type = weaponType;
					Log(FormatString("Tag: '%10s', weapon condition, type: %d", common.tag.c_str(), weaponType));
					g_Items.emplace_back(common, weapon);
				}
			}
			else {
				Log(FormatString("Tag: '%10s', weapon condition", common.tag.c_str()));
				g_Items.emplace_back(common, weapon);
			}
		}
		else if (common.formType == 24)
		{
			common.formType = 24;
			Item::Armor armor{};

			if (elem.contains("armorHead")) 		(elem["armorHead"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1;
			if (elem.contains("armorHair")) 		(elem["armorHair"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 1;
			if (elem.contains("armorUpperBody")) 	(elem["armorUpperBody"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 2;
			if (elem.contains("armorLeftHand")) 	(elem["armorLeftHand"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 3;
			if (elem.contains("armorRightHand")) 	(elem["armorRightHand"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 4;
			if (elem.contains("armorWeapon")) 		(elem["armorWeapon"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 5;
			if (elem.contains("armorPipBoy")) 		(elem["armorPipBoy"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 6;
			if (elem.contains("armorBackpack")) 	(elem["armorBackpack"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 7;
			if (elem.contains("armorNecklace")) 	(elem["armorNecklace"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 8;
			if (elem.contains("armorHeadband")) 	(elem["armorHeadband"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 9;
			if (elem.contains("armorHat")) 			(elem["armorHat"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 10;
			if (elem.contains("armorEyeglasses")) 	(elem["armorEyeglasses"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 11;
			if (elem.contains("armorNosering")) 	(elem["armorNosering"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 12;
			if (elem.contains("armorEarrings")) 	(elem["armorEarrings"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 13;
			if (elem.contains("armorMask")) 		(elem["armorMask"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 14;
			if (elem.contains("armorChoker")) 		(elem["armorChoker"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 15;
			if (elem.contains("armorMouthObject"))	(elem["armorMouthObject"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 16;
			if (elem.contains("armorBodyAddon1"))	(elem["armorBodyAddon1"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 17;
			if (elem.contains("armorBodyAddon2"))	(elem["armorBodyAddon2"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 18;
			if (elem.contains("armorBodyAddon3"))	(elem["armorBodyAddon3"].get<SInt8>() == 1 ? armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 19;

			if (elem.contains("armorClass"))		armor.armorClass = elem["armorClass"].get<UInt16>();
			if (elem.contains("armorPower"))		armor.powerArmor = elem["armorPower"].get<SInt8>();
			if (elem.contains("armorHasBackpack"))	armor.hasBackpack = elem["armorHasBackpack"].get<SInt8>();

			if (elem.contains("armorDT"))			armor.dt = elem["armorDT"].get<float>();
			if (elem.contains("armorDR"))			armor.dr = elem["armorDR"].get<UInt16>();
			if (elem.contains("armorChangesAV"))	armor.changesAV = elem["armorChangesAV"].get<UInt16>();

			g_Items.emplace_back(common, armor);
		}
		else if (common.formType == 31 || elem.contains("miscComponent"))
		{
			common.formType = 31;
			Item::Misc misc{};
			g_Items.emplace_back(common, misc);
		}
		else if (common.formType == 47 || elem.contains("IsFood") || elem.contains("IsMedicine"))
		{
			common.formType = 47;
			Item::Aid aid{};

			if (elem.contains("aidRestoresAV"))			aid.restoresAV = elem["aidRestoresAV"].get<UInt8>();
			if (elem.contains("aidDamagesAV"))			aid.damagesAV = elem["aidDamagesAV"].get<UInt8>();
			if (elem.contains("aidIsAddictive"))		aid.isAddictive = elem["aidIsAddictive"].get<UInt8>();
			if (elem.contains("aidIsFood"))				aid.isFood = elem["aidIsFood"].get<UInt8>();
			if (elem.contains("aidIsWater"))			aid.isWater = elem["aidIsWater"].get<UInt8>();
			if (elem.contains("aidIsMedicine"))			aid.isMedicine = elem["aidIsMedicine"].get<UInt8>();
			if (elem.contains("aidIsPoisonous"))		aid.isPoisonous = elem["aidIsPoisonous"].get<UInt8>();
			g_Items.emplace_back(common, aid);
		}
		else g_Items.emplace_back(common);
	}

	void HandleCategory(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log("JSON error: expected object");
			return;
		}

		Category category{};

		category.tag = elem["tag"].get<std::string>();
		if (elem.contains("priority"))			category.priority = elem["priority"].get<SInt16>();
		if (elem.contains("template"))			category.xmltemplate = elem["template"].get<std::string>();
		if (elem.contains("filename"))			category.filename = elem["filename"].get<std::string>();
		if (elem.contains("texatlas"))			category.texatlas = elem["texatlas"].get<std::string>();
		if (elem.contains("systemcolor"))		category.systemcolor = elem["systemcolor"].get<SInt32>();
		if (elem.contains("category"))			category.category = elem["category"].get<std::string>();
		if (elem.contains("routeToKeyring"))	category.category = elem["routeToKeyring"].get<std::string>();
		if (elem.contains("name"))				category.name = UTF8toANSI(elem["name"].get<std::string>());
		if (elem.contains("icon"))				category.icon = elem["icon"].get<std::string>();
		if (elem.contains("tab"))				category.tab = elem["tab"].get<UInt32>();
		if (elem.contains("count"))				category.count = elem["count"].get<UInt32>();

		Log(FormatString("Tag: '%10s', icon: '%s'", category.tag.c_str(), category.filename.c_str()));
		g_Categories.emplace_back(category);
	}

	void HandleTab(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log("JSON error: expected object with mod, form and folder fields");
			return;
		}

		Tab tab{};
		tab.tab = elem["tab"].get<std::string>();
		if (elem.contains("name"))			tab.name = UTF8toANSI(elem["name"].get<std::string>());
		if (elem.contains("priority"))		tab.priority = elem["priority"].get<SInt32>();
		if (elem.contains("tabNew"))		tab.tabNew = elem["tabNew"].get<UInt32>();
		if (elem.contains("inventory"))		tab.inventory = elem["inventory"].get<UInt32>();
		if (elem.contains("container"))		tab.container = elem["container"].get<UInt32>();
		//		if (elem.contains("tabMisc"))		tab.tabMisc =		elem["tabMisc"].get<UInt32>();

		if (const auto types = elem.contains("types") ? &elem["types"] : nullptr; types)
		{
			if (!types->is_array()) tab.types.emplace(types->get<UInt32>());
			else for (const auto& type : elem["types"]) tab.types.emplace(type.get<UInt32>());
		}

		if (const auto categories = elem.contains("categories") ? &elem["categories"] : nullptr; categories)
		{
			if (!categories->is_array()) tab.categories.emplace(categories->get<std::string>());
			else for (const auto& type : elem["categories"]) tab.categories.emplace(type.get<std::string>());
		}

		if (const auto tabMisc = elem.contains("tabMisc") ? &elem["tabMisc"] : nullptr; tabMisc)
		{
			if (!tabMisc->is_array()) tab.tabMisc.emplace(tabMisc->get<std::string>());
			else for (const auto& type : elem["tabMisc"]) tab.tabMisc.emplace(type.get<std::string>());
		}

		g_Tabs.push_back(tab);
	}

	void HandleSIJson(const std::filesystem::path& path)
	{
		Log("\nReading JSON file " + path.string());

		try
		{
			std::ifstream i(path);
			auto j = nlohmann::json::parse(i, nullptr, true, true);

			if (j.contains("items") && j["items"].is_array())
				for (const auto& elem : j["items"]) HandleItem(elem);
			else if (j.contains("tags") && j["tags"].is_array())				// legacy support
				for (const auto& elem : j["tags"]) HandleItem(elem);
			else
				Log(path.string() + " JSON ySI item array not detected");

			if (j.contains("categories") && j["categories"].is_array())
				for (const auto& elem : j["categories"]) HandleCategory(elem);
			else if (j.contains("icons") && j["icons"].is_array())				// legacy support
				for (const auto& elem : j["icons"]) HandleCategory(elem);
			else
				Log(path.string() + " JSON ySI category array not detected");

			if (j.contains("tabs") && j["tabs"].is_array())
				for (const auto& elem : j["tabs"]) HandleTab(elem);
			else
				Log(path.string() + " JSON ySI tab array not detected");
		}
		catch (nlohmann::json::exception& e)
		{
			Log("The JSON is incorrectly formatted! It will not be applied.");
			Log(FormatString("JSON error: %s\n", e.what()));
		}

	}

	void ItemRecursiveEmplace(const Item::Common& common, TESForm* item)
	{
		if (item->typeID == kFormType_BGSListForm)
		{
			const auto bgslist = DYNAMIC_CAST(item, TESForm, BGSListForm);
			if (!bgslist) return;
			for (const auto iter : bgslist->list)
				if (iter) ItemRecursiveEmplace(common, iter);
		}
		else if (!common.formType || item->typeID == common.formType) {
			Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive", common.tag.c_str(), item->refID, item->GetName()));
			g_Items.emplace_back(common, item);
		}
	}

	bool AssignCategoryToItem(TESForm* form)
	{
		for (const auto& entry : g_Items) {
			if (entry.common.form && entry.common.form->refID != form->refID) continue;
			if (entry.common.formType && entry.common.formType != form->typeID) continue;

			if (entry.common.questItem && entry.common.questItem != static_cast<UInt8>(form->IsQuestItem2())) continue;
			if (entry.common.miscComponent && !CraftingComponents::IsComponent(form)) continue;
			if (entry.common.miscProduct && !CraftingComponents::IsProduct(form)) continue;

			if (entry.common.formType == 40) {
				const auto weapon = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
				if (!weapon) continue;
				if (entry.weapon.skill && entry.weapon.skill != weapon->weaponSkill) continue;
				if (entry.weapon.type && entry.weapon.type != weapon->eWeaponType) continue;
				if (entry.weapon.handgrip && entry.weapon.handgrip != weapon->HandGrip()) continue;
				if (entry.weapon.attackAnim && entry.weapon.attackAnim != weapon->AttackAnimation()) continue;
				if (entry.weapon.reloadAnim && entry.weapon.reloadAnim != weapon->reloadAnim) continue;
				if (entry.weapon.type && entry.weapon.type != weapon->eWeaponType) continue;
				if (entry.weapon.isAutomatic && entry.weapon.isAutomatic != static_cast<UInt32>(weapon->IsAutomatic())) continue;
				if (entry.weapon.hasScope && entry.weapon.hasScope != static_cast<UInt32>(weapon->HasScopeAlt())) continue;
				if (entry.weapon.ignoresDTDR && entry.weapon.ignoresDTDR != static_cast<UInt32>(weapon->IgnoresDTDR())) continue;
				if (entry.weapon.clipRounds && entry.weapon.clipRounds > static_cast<UInt32>(weapon->GetClipRounds(false))) continue;
				if (entry.weapon.numProjectiles && entry.weapon.numProjectiles > weapon->numProjectiles) continue;
				if (entry.weapon.soundLevel && entry.weapon.soundLevel != weapon->soundLevel) continue;
				if (entry.weapon.ammo && !FormContainsRecusive(entry.weapon.ammo, weapon->ammo.ammo)) continue;
			}
			else if (entry.common.formType == 24) {
				const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
				if (!armor) continue;
				if (entry.armor.slotsMaskWL && (entry.armor.slotsMaskWL & armor->GetArmorValue(6)) != entry.armor.slotsMaskWL) continue;
				if (entry.armor.slotsMaskBL && (entry.armor.slotsMaskBL & armor->GetArmorValue(6)) != 0) continue;
				if (entry.armor.armorClass && entry.armor.armorClass != armor->GetArmorValue(1)) continue;
				if (entry.armor.powerArmor && entry.armor.powerArmor != armor->GetArmorValue(2)) continue;
				if (entry.armor.hasBackpack && entry.armor.hasBackpack != armor->GetArmorValue(3)) continue;

				if (entry.armor.dt && entry.armor.dt > armor->damageThreshold) continue;
				if (entry.armor.dr && entry.armor.dr > armor->armorRating) continue;
				//				if (entry.armor.armorChangesAV && entry.armor.armorChangesAV > armor->armorRating) continue;
			}
			else if (entry.common.formType == 31) {
			}
			else if (entry.common.formType == 47) {
				const auto aid = DYNAMIC_CAST(form, TESForm, AlchemyItem);
				if (!aid) continue;
				if (entry.aid.restoresAV && !aid->HasBaseEffectRestoresAV(entry.aid.restoresAV)) continue;
				if (entry.aid.damagesAV && !aid->HasBaseEffectDamagesAV(entry.aid.damagesAV)) continue;
				if (entry.aid.isAddictive && !aid->IsAddictive()) continue;
				if (entry.aid.isFood && !aid->IsFood()) continue;
				if (entry.aid.isWater && !aid->IsWaterAlt()) continue;
				if (entry.aid.isPoisonous && !aid->IsPoison()) continue;
				if (entry.aid.isMedicine && !aid->IsMedicine()) continue;
			}

			g_ItemToCategory.emplace(form, entry.common.tag);
			return true;
		}
		return false;
	}

	bool AssignFiltersToItem(TESForm* form)
	{
		std::unordered_set<std::string> set;
		for (const auto& [fst, snd] : g_StringToTabs) {
			if (!snd.tabMisc.empty()) continue;
			if (!snd.types.empty() && !snd.types.contains(form->typeID)) continue;
			if (!snd.categories.empty() && !snd.categories.contains(Sorting::GetCategoryForItem(form))) continue;
			set.emplace(fst);
		}

		for (const auto& [fst, snd] : g_StringToTabs) {
			if (snd.tabMisc.empty()) continue;
			bool misc = true;
			for (const auto& it : snd.tabMisc) if (set.contains(it)) { misc = false; break; }
			if (misc) set.emplace(fst);
		}

		g_ItemToFilter.emplace(form, std::move(set));
		return true;
	}
}