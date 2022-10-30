#include "SI.h"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <GameData.h>
#include <json.h>

namespace SortingIcons::Files
{
	void ItemRecursiveEmplace(const Item::Common&, TESForm*);

	void HandleItem(nlohmann::basic_json<> elem)
	{
		constexpr auto strToFormID = [](const std::string& formIdStr)
		{
			const auto formId = HexStringToInt(formIdStr);
			if (formId == -1) Log("Form field was incorrectly formatted, got " + formIdStr, logLevel);
			return formId;
		};

		if (!elem.is_object())
		{
			Log("JSON error: expected object", logLevel);
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
				Log("Mod name " + modName + " was not found", logLevel);
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
					if (!common.form) { Log(FormatString("Form %X was not found", formId), logLevel); continue; }
					if (!formlist) {
						Log(FormatString("Tag: '%10s', form: %08X (%50s), individual", common.tag.c_str(), formId, common.form->GetName()), logLevel);
						g_Items.emplace_back(common);
					}
					else if (formlist == 1) {
						common.formType = 0;
						ItemRecursiveEmplace(common, common.form);
					}
					else if (formlist == 2) {
						for (const auto item : *GetAllForms()) {
							if (item->typeID == kFormType_TESObjectWEAP) {
								const auto weapon = reinterpret_cast<TESObjectWEAP*>(item);
								if (!weapon || !weapon->repairItemList.listForm) continue;
								if (weapon->refID == common.form->refID || weapon->repairItemList.listForm->refID == common.form->refID || weapon->repairItemList.listForm->ContainsRecursive(common.form)) {
									Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X'", common.tag.c_str(), item->refID, item->GetName(), formId), logLevel);
									auto newCommon = common;
									newCommon.form = item;
									g_Items.emplace_back(newCommon);
								}
							}
							else if (item->typeID == kFormType_TESObjectARMO) {
								const auto armor = reinterpret_cast<TESObjectARMO*>(item);
								if (!armor || !armor->repairItemList.listForm) continue;
								if (armor->refID == common.form->refID || armor->repairItemList.listForm->refID == common.form->refID || armor->repairItemList.listForm->ContainsRecursive(common.form)) {
									Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X'", common.tag.c_str(), item->refID, item->GetName(), formId), logLevel);
									auto newCommon = common;
									newCommon.form = item;
									g_Items.emplace_back(newCommon);
								}
							}
						}
					}
				}
			}
			else {
				Log(FormatString("Tag: '%10s', mod: '%s'", common.tag.c_str(), modName.c_str()), logLevel);
				g_Items.emplace_back(common);
			}
		}
		else if (common.formType == 40)
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
					Log(FormatString("Tag: '%10s', weapon condition, type: %d", common.tag.c_str(), weaponType), logLevel);
					g_Items.emplace_back(common, weapon);
				}
			}
			else {
				Log(FormatString("Tag: '%10s', weapon condition", common.tag.c_str()), logLevel);
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
		else if (common.formType == 31)
		{
			common.formType = 31;
			Item::Misc misc{};
			g_Items.emplace_back(common, misc);
		}
		else if (common.formType == 47)
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
			Log("JSON error: expected object", logLevel);
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

		Log(FormatString("Tag: '%10s', icon: '%s'", category.tag.c_str(), category.filename.c_str()), logLevel);
		g_Categories.emplace_back(category);
	}

	void HandleTab(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log("JSON error: expected object with mod, form and folder fields", logLevel);
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

	void HandleXML(const std::filesystem::path& path)
	{
		const auto pathstring = path.generic_string();
		const auto relativepath = pathstring.substr(pathstring.find_last_of("\\Data\\") - 3);
		g_XMLPaths.emplace_back(std::filesystem::path(relativepath));
	}

	void HandleJson(const std::filesystem::path& path)
	{
		Log("\nReading JSON file " + path.string(), logLevel);

		try
		{
			std::ifstream i(path);
			auto j = nlohmann::json::parse(i, nullptr, true, true);

			if (j.contains("items") && j["items"].is_array())
				for (const auto& elem : j["items"]) HandleItem(elem);
			else if (j.contains("tags") && j["tags"].is_array())				// legacy support
				for (const auto& elem : j["tags"]) HandleItem(elem);
			else
				Log(path.string() + " JSON ySI item array not detected", logLevel);

			if (j.contains("categories") && j["categories"].is_array())
				for (const auto& elem : j["categories"]) HandleCategory(elem);
			else if (j.contains("icons") && j["icons"].is_array())				// legacy support
				for (const auto& elem : j["icons"]) HandleCategory(elem);
			else
				Log(path.string() + " JSON ySI category array not detected", logLevel);

			if (j.contains("tabs") && j["tabs"].is_array())
				for (const auto& elem : j["tabs"]) HandleTab(elem);
			else
				Log(path.string() + " JSON ySI tab array not detected", logLevel);
		}
		catch (nlohmann::json::exception& e)
		{
			Log("The JSON is incorrectly formatted! It will not be applied.", logLevel);
			Log(FormatString("JSON error: %s\n", e.what()), logLevel);
		}

	}

	void ItemRecursiveEmplace(const Item::Common& common, TESForm* item)
	{
		if (item->typeID == kFormType_BGSListForm) {
			for (const auto iter : reinterpret_cast<BGSListForm*>(item)->list) if (iter) ItemRecursiveEmplace(common, iter);
		} else if (!common.formType || item->typeID == common.formType) {
			Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive", common.tag.c_str(), item->refID, item->GetName()), logLevel);
			auto newCommon = common;
			newCommon.form = item;
			g_Items.emplace_back(newCommon);
		}
	}
}