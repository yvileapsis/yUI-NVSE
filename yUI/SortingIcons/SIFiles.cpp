#include "SI.h"

#include <chrono>
#include <filesystem>
#include <fstream>

#include <GameData.h>
#include <json.h>

namespace SortingIcons::Files
{
	std::vector<TESForm*> GetFormsFromElement(const nlohmann::basic_json<>& elem, const std::string& mod, const std::string& form)
	{
		const auto modName = elem.contains(mod) ? elem[mod].get<std::string>() : "";
		std::vector<TESForm*> forms{};
		if (!elem.contains(form)) {}
		else if (!elem[form].is_array()) {
			if (const auto val = GetFormByID(modName.c_str(), elem[form].get<std::string>().c_str())) forms.push_back(val);
		}
		else for (const auto& i : elem[form]) {
			if (const auto val = GetFormByID(modName.c_str(), i.get<std::string>().c_str())) forms.push_back(val);
		}
		return forms;
	}

	void ItemRecursiveEmplace(const Item::Common& common, TESForm* form)
	{
		if (form->typeID == kFormType_BGSListForm)
			for (const auto iter : reinterpret_cast<BGSListForm*>(form)->list) ItemRecursiveEmplace(common, iter);
		else if (common.formType == 0 || common.formType == form->typeID) {
			if (common.form == form)
				Log(FormatString("Tag: '%10s', form: %08X (%50s), individual", common.tag.c_str(), form->refID, form->GetName()), logLevel);
			else
				Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, list: '%08X' (%50s)", common.tag.c_str(), form->refID, form->GetName(), common.form->refID, common.form->GetName()), logLevel);
			auto newCommon = common;
			newCommon.form = form;
			g_Items.emplace_back(std::make_shared<Item>(newCommon));
		}
	}

	void ItemRepairListEmplace(const Item::Common& common, TESForm* form)
	{
		for (const auto item : *GetAllForms())
		{
			BGSListForm* list;

			if (item->typeID == kFormType_TESObjectWEAP)
				list = reinterpret_cast<TESObjectWEAP*>(item)->repairItemList.listForm;
			else if (item->typeID == kFormType_TESObjectARMO)
				list = reinterpret_cast<TESObjectARMO*>(item)->repairItemList.listForm;
			else continue;

			if (item->refID == form->refID || list && (list->refID == form->refID && list->ContainsRecursive(form)))
			{
				Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X' (%50s)", common.tag.c_str(), item->refID, item->GetName(), form->refID, common.form->GetName()), logLevel);
				auto newCommon = common;
				newCommon.form = item;
				g_Items.emplace_back(std::make_shared<Item>(newCommon));
			}
		}
	}

	void HandleItem(nlohmann::basic_json<> elem)
	{
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
			UInt32 repairList = 0;
			if (elem.contains("formlist")) repairList = elem["formlist"].get<UInt32>();
			if (elem.contains("repairList")) repairList = elem["repairList"].get<UInt32>();
			for (const auto form : GetFormsFromElement(elem, "mod", "form"))
			{
				common.form = form;
				repairList ? ItemRepairListEmplace(common, form) : ItemRecursiveEmplace(common, form);
			}
			return;
		}

		if (common.formType == kFormType_TESObjectWEAP)
		{
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

			std::vector<TESForm*> ammos;
			if (elem.contains("ammoMod") && elem.contains("ammoForm"))
				ammos = GetFormsFromElement(elem, "ammoMod", "ammoForm");

			std::vector<UInt32> weaponTypes;
			if (elem.contains("weaponType"))
			{
				if (!elem["weaponType"].is_array()) weaponTypes.push_back(elem["weaponType"].get<UInt32>());
				else std::ranges::transform(elem["weaponType"], std::back_inserter(weaponTypes), [&](auto& i) { return i.template get<UInt32>(); });
			}

			if (!weaponTypes.empty()) for (auto weaponType : weaponTypes)
			{
				weapon.type = weaponType;
				if (!ammos.empty()) for (auto ammo : ammos) {
					weapon.ammo = ammo;
					Log(FormatString("Tag: '%10s', weapon condition, type: %d, ammo form: %08X (%50s)", common.tag.c_str(), weaponType, ammo->refID, ammo->GetName()), logLevel);
					g_Items.emplace_back(std::make_shared<Item>(common, weapon));
				} else {
					Log(FormatString("Tag: '%10s', weapon condition, type: %d", common.tag.c_str(), weaponType), logLevel);
					g_Items.emplace_back(std::make_shared<Item>(common, weapon));
				}
			} else {
				if (!ammos.empty()) for (auto ammo : ammos) {
					weapon.ammo = ammo;
					Log(FormatString("Tag: '%10s', weapon condition, ammo form: %08X (%50s)", common.tag.c_str(), ammo->refID, ammo->GetName()), logLevel);
					g_Items.emplace_back(std::make_shared<Item>(common, weapon));
				} else {
					Log(FormatString("Tag: '%10s', weapon condition", common.tag.c_str()), logLevel);
					g_Items.emplace_back(std::make_shared<Item>(common, weapon));
				}
			}
		}
		else if (common.formType == kFormType_TESObjectARMO)
		{
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

			Log(FormatString("Tag: '%10s', armor condition", common.tag.c_str()), logLevel);
			g_Items.emplace_back(std::make_shared<Item>(common, armor));
		}
		else if (common.formType == 47)
		{
			Item::Aid aid{};

			if (elem.contains("aidRestoresAV"))			aid.restoresAV = elem["aidRestoresAV"].get<UInt8>();
			if (elem.contains("aidDamagesAV"))			aid.damagesAV = elem["aidDamagesAV"].get<UInt8>();
			if (elem.contains("aidIsAddictive"))		aid.isAddictive = elem["aidIsAddictive"].get<UInt8>();
			if (elem.contains("aidIsFood"))				aid.isFood = elem["aidIsFood"].get<UInt8>();
			if (elem.contains("aidIsWater"))			aid.isWater = elem["aidIsWater"].get<UInt8>();
			if (elem.contains("aidIsMedicine"))			aid.isMedicine = elem["aidIsMedicine"].get<UInt8>();
			if (elem.contains("aidIsPoisonous"))		aid.isPoisonous = elem["aidIsPoisonous"].get<UInt8>();

			Log(FormatString("Tag: '%10s', aid condition", common.tag.c_str()), logLevel);
			g_Items.emplace_back(std::make_shared<Item>(common, aid));
		}
		else g_Items.emplace_back(std::make_shared<Item>(common));
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
		/*
		if (elem.contains("category"))			category.category = elem["category"].get<std::string>();
		if (elem.contains("routeToKeyring"))	category.category = elem["routeToKeyring"].get<std::string>();
		if (elem.contains("name"))				category.name = UTF8toANSI(elem["name"].get<std::string>());
		if (elem.contains("icon"))				category.icon = elem["icon"].get<std::string>();
		if (elem.contains("count"))				category.count = elem["count"].get<UInt32>();
		*/
		Log(FormatString("Tag: '%10s', icon: '%s'", category.tag.c_str(), category.filename.c_str()), logLevel);
		g_Categories.emplace_back(std::make_shared<Category>(category));
	}

	void HandleTab(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log("JSON error: expected object with mod, form and folder fields", logLevel);
			return;
		}

		Tab tab{};
		tab.tag = elem["tag"].get<std::string>();
		if (elem.contains("priority"))		tab.priority = elem["priority"].get<SInt32>();
		if (elem.contains("name"))			tab.name = UTF8toANSI(elem["name"].get<std::string>());

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

		if (elem.contains("icon"))			tab.icon = elem["icon"].get<std::string>();

		if (elem.contains("keyring"))		tab.keyring		= elem["keyring"].get<UInt32>();
		if (elem.contains("inventory"))		tab.inventory	= elem["inventory"].get<UInt32>();
		if (elem.contains("container"))		tab.container	= elem["container"].get<UInt32>();

		if (elem.contains("tabPriority"))	tab.tabPriority = elem["tabPriority"].get<UInt32>();

		if (elem.contains("tabNew"))		tab.tabNew		= elem["tabNew"].get<UInt32>();
		if (elem.contains("tabMisc"))		tab.tabMisc		= elem["tabMisc"].get<UInt32>();

		g_Tabs.emplace_back(std::make_shared<Tab>(tab));
	}

	void HandleXML(const std::filesystem::path& path)
	{
		const auto pathstring = path.generic_string();
		const auto relativepath = pathstring.substr(pathstring.find_last_of("\\Data\\") - 3);
		g_XMLPaths.emplace_back(std::filesystem::path(relativepath));
	}

	void HandleJSON(const std::filesystem::path& path)
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
}