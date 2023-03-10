#include "SortingIcons.h"

#include <GameData.h>
#include <json.h>

namespace SortingIcons::Files
{
	__forceinline std::vector<TESForm*> GetFormsFromElement(const nlohmann::basic_json<>& elem, const std::string& mod, const std::string& form)
	{
		const auto modName = elem.contains(mod) ? elem[mod].get<std::string>() : "";
		std::vector<TESForm*> forms{};
		std::string log;
		if (!elem.contains(form)) {}
		else if (!elem[form].is_array()) {
			if (const auto val = TESForm::GetByID(modName.c_str(), elem[form].get<std::string>().c_str())) forms.push_back(val);
			else log += FormatString("%6s, ", elem[form].get<std::string>().c_str());
		} else for (const auto& i : elem[form]) {
			if (const auto val = TESForm::GetByID(modName.c_str(), i.get<std::string>().c_str())) forms.push_back(val);
			else log += FormatString("%6s, ", i.get<std::string>().c_str());
		}
		if (!log.empty()) Log(logLevel >= Log::kWarning) << "JSON warning: Failed to find form, mod: " + modName + ", forms: " + log;
		return forms;
	}

	template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
	{
		std::vector<T> set{};
		if (!elem.is_array()) {
			if (const auto val = elem.get<T>()) set.push_back(val);
		}
		else for (const auto& i : elem) {
			if (const auto val = i.get<T>()) set.push_back(val);
		}
		return set;
	}

	std::vector<UInt32> FlattenListForm(TESForm* form)
	{
		std::vector<UInt32> output;
		if (form->typeID != kFormType_BGSListForm)
			output.push_back(form->refID);
		else for (const auto iter : reinterpret_cast<BGSListForm*>(form)->list)
			for (const auto val : FlattenListForm(iter)) output.push_back(val);
		return output;
	}

	std::vector<UInt32> FlattenListRepair(TESForm* form)
	{
		std::vector<UInt32> output;
		output.push_back(form->refID);
		for (const auto item : *TESForm::GetAll())
		{
			BGSListForm* list;

			if (item->typeID == kFormType_TESObjectWEAP)
				list = reinterpret_cast<TESObjectWEAP*>(item)->repairItemList.listForm;
			else if (item->typeID == kFormType_TESObjectARMO)
				list = reinterpret_cast<TESObjectARMO*>(item)->repairItemList.listForm;
			else continue;

			if (list && list->refID == form->refID) output.push_back(item->refID);
		}
		return output;
	}

	void HandleItem(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log(logLevel >= Log::kError) << "JSON error: Expected object";
			return;
		}

		if (!elem.contains("tag") || !elem.contains("priority"))
		{
			Log(logLevel >= Log::kWarning) << "JSON error: Expected tag and priority";
			return;
		}

		Item item{};

		item.tag = elem["tag"].get<std::string>();
		item.priority = elem["priority"].get<SInt32>();

		if (elem.contains("formType"))		item.common.formType.insert_range(GetSetFromElement<UInt8>(elem["formType"]));

		if (elem.contains("questItem"))		item.common.questItem = elem["questItem"].get<UInt8>();
		if (elem.contains("miscComponent"))	item.common.miscComponent = elem["miscComponent"].get<UInt8>();
		if (elem.contains("miscProduct"))	item.common.miscProduct = elem["miscProduct"].get<UInt8>();

		if (elem.contains("mod") && elem.contains("form"))
		{
			UInt32 repairList = 0;
			if (elem.contains("formlist")) repairList |= elem["formlist"].get<UInt8>();
			if (elem.contains("repairList")) repairList |= elem["repairList"].get<UInt8>();

			for (const auto form : GetFormsFromElement(elem, "mod", "form"))
			{
				item.common.formIDs.insert_range(repairList ? FlattenListRepair(form) : FlattenListForm(form));
			}

			if (item.common.formIDs.empty())
			{
				Log(logLevel >= Log::kWarning) << FormatString("JSON warning: Failed to find any forms for tag: '%6s', priority: '%03d'", item.tag.c_str(), item.priority);

				return;
			}
		}

		if (elem.contains("weaponSkill"))			item.weapon.skill.insert_range(GetSetFromElement<UInt32>(elem["weaponSkill"]));
		if (elem.contains("weaponHandgrip"))		item.weapon.handgrip.insert_range(GetSetFromElement<UInt32>(elem["weaponHandgrip"]));
		if (elem.contains("weaponAttackAnim"))		item.weapon.attackAnim.insert_range(GetSetFromElement<UInt32>(elem["weaponAttackAnim"]));
		if (elem.contains("weaponReloadAnim"))		item.weapon.reloadAnim.insert_range(GetSetFromElement<UInt32>(elem["weaponReloadAnim"]));
		if (elem.contains("weaponIsAutomatic"))		item.weapon.isAutomatic = elem["weaponIsAutomatic"].get<UInt8>();
		if (elem.contains("weaponHasScope"))		item.weapon.hasScope = elem["weaponHasScope"].get<UInt8>();
		if (elem.contains("weaponIgnoresDTDR"))		item.weapon.ignoresDTDR = elem["weaponIgnoresDTDR"].get<UInt8>();
		if (elem.contains("weaponClipRounds"))		item.weapon.clipRoundsMin = elem["weaponClipRounds"].get<UInt32>();
		if (elem.contains("weaponNumProjectiles"))	item.weapon.numProjectilesMin = elem["weaponNumProjectiles"].get<UInt32>();
		if (elem.contains("weaponSoundLevel"))		item.weapon.soundLevel.insert_range(GetSetFromElement<UInt32>(elem["weaponSoundLevel"]));

		if (elem.contains("ammoMod") && elem.contains("ammoForm")) for (const auto iter : GetFormsFromElement(elem, "ammoMod", "ammoForm"))
			item.weapon.ammoIDs.emplace(iter->refID);

		if (elem.contains("weaponType"))			item.weapon.type.insert_range(GetSetFromElement<UInt32>(elem["weaponType"]));

		if (elem.contains("armorHead")) 			(elem["armorHead"].get<SInt8>() == 1 ?			item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1;
		if (elem.contains("armorHair")) 			(elem["armorHair"].get<SInt8>() == 1 ?			item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 1;
		if (elem.contains("armorUpperBody")) 		(elem["armorUpperBody"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 2;
		if (elem.contains("armorLeftHand"))			(elem["armorLeftHand"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 3;
		if (elem.contains("armorRightHand")) 		(elem["armorRightHand"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 4;
		if (elem.contains("armorWeapon")) 			(elem["armorWeapon"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 5;
		if (elem.contains("armorPipBoy")) 			(elem["armorPipBoy"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 6;
		if (elem.contains("armorBackpack"))			(elem["armorBackpack"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 7;
		if (elem.contains("armorNecklace"))			(elem["armorNecklace"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 8;
		if (elem.contains("armorHeadband"))			(elem["armorHeadband"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 9;
		if (elem.contains("armorHat")) 				(elem["armorHat"].get<SInt8>() == 1 ?			item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 10;
		if (elem.contains("armorEyeglasses"))		(elem["armorEyeglasses"].get<SInt8>() == 1 ?	item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 11;
		if (elem.contains("armorNosering"))			(elem["armorNosering"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 12;
		if (elem.contains("armorEarrings"))			(elem["armorEarrings"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 13;
		if (elem.contains("armorMask")) 			(elem["armorMask"].get<SInt8>() == 1 ?			item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 14;
		if (elem.contains("armorChoker")) 			(elem["armorChoker"].get<SInt8>() == 1 ?		item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 15;
		if (elem.contains("armorMouthObject"))		(elem["armorMouthObject"].get<SInt8>() == 1 ?	item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 16;
		if (elem.contains("armorBodyAddon1"))		(elem["armorBodyAddon1"].get<SInt8>() == 1 ?	item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 17;
		if (elem.contains("armorBodyAddon2"))		(elem["armorBodyAddon2"].get<SInt8>() == 1 ?	item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 18;
		if (elem.contains("armorBodyAddon3"))		(elem["armorBodyAddon3"].get<SInt8>() == 1 ?	item.armor.slotsMaskWL : item.armor.slotsMaskBL) += 1 << 19;

		if (elem.contains("armorClass"))			item.armor.armorClass = elem["armorClass"].get<UInt16>();
		if (elem.contains("armorPower"))			item.armor.powerArmor = elem["armorPower"].get<SInt8>();
		if (elem.contains("armorHasBackpack"))		item.armor.hasBackpack = elem["armorHasBackpack"].get<SInt8>();

		if (elem.contains("armorDT"))				item.armor.dt = elem["armorDT"].get<float>();
		if (elem.contains("armorDR"))				item.armor.dr = elem["armorDR"].get<UInt16>();
		if (elem.contains("armorChangesAV"))		item.armor.changesAV = elem["armorChangesAV"].get<UInt16>();

		if (elem.contains("aidRestoresAV"))			item.aid.restoresAV = elem["aidRestoresAV"].get<UInt8>();
		if (elem.contains("aidDamagesAV"))			item.aid.damagesAV = elem["aidDamagesAV"].get<UInt8>();
		if (elem.contains("aidIsAddictive"))		item.aid.isAddictive = elem["aidIsAddictive"].get<UInt8>();
		if (elem.contains("aidIsFood"))				item.aid.isFood = elem["aidIsFood"].get<UInt8>();
		if (elem.contains("aidIsWater"))			item.aid.isWater = elem["aidIsWater"].get<UInt8>();
		if (elem.contains("aidIsMedicine"))			item.aid.isMedicine = elem["aidIsMedicine"].get<UInt8>();
		if (elem.contains("aidIsPoisonous"))		item.aid.isPoisonous = elem["aidIsPoisonous"].get<UInt8>();

		std::string log = FormatString("JSON message: Tag: '%6s', priority: '%03d'", item.tag.c_str(), item.priority);
		if (!item.common.formType.empty()) {
			log += ", types: ";
			UInt32 i = 0;
			for (const auto iter : item.common.formType)
			{
				log += std::to_string(iter);
				if (++i != item.common.formType.size()) log += ", ";
			}
		}
		if (!item.common.formIDs.empty())
		{
			log += ", forms:";
			log += item.common.formIDs.size() == 1 ? " " : "\n";
			UInt32 i = 0;
			for (const auto iter : item.common.formIDs)
			{
				log += FormatString("%08X (%40s)", iter, TESForm::GetByID(iter)->GetName());
				if (++i != item.common.formIDs.size()) log += i % 2 ? ", " : ",\n";
			}
		}
		Log(logLevel >= Log::kMessage) << log;
		g_Items.emplace_back(std::make_shared<Item>(item));
	}

	void HandleCategory(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log(logLevel >= Log::kError) << "JSON error: expected object";
			return;
		}

		Category category{};

		category.tag = elem["tag"].get<std::string>();
		if (elem.contains("priority"))			category.priority = elem["priority"].get<SInt16>();
		if (elem.contains("template"))			category.xmltemplate = elem["template"].get<std::string>();
		if (elem.contains("filename"))			category.filename = elem["filename"].get<std::string>();
		if (elem.contains("texatlas"))			category.texatlas = elem["texatlas"].get<std::string>();
		if (elem.contains("systemcolor"))		category.systemcolor = elem["systemcolor"].get<SInt32>();
		if (elem.contains("font"))				category.font = elem["font"].get<SInt32>();
		/*
		if (elem.contains("category"))			category.category = elem["category"].get<std::string>();
		if (elem.contains("routeToKeyring"))	category.category = elem["routeToKeyring"].get<std::string>();
		if (elem.contains("name"))				category.name = UTF8toANSI(elem["name"].get<std::string>());
		if (elem.contains("icon"))				category.icon = elem["icon"].get<std::string>();
		if (elem.contains("count"))				category.count = elem["count"].get<UInt32>();
		*/
		Log(logLevel >= Log::kMessage) << FormatString("Tag: '%6s', icon: '%s'", category.tag.c_str(), category.filename.c_str());
		g_Categories.emplace_back(std::make_shared<Category>(category));
	}

	void HandleTab(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log(logLevel >= Log::kError) << "JSON error: expected object with mod, form and folder fields";
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

		if (const auto categories = elem.contains("categories") ? elem["categories"] : nullptr; categories)
		{
			if (!categories.is_array()) tab.categories.emplace(categories.get<std::string>());
			else for (const auto& type : elem["categories"]) tab.categories.emplace(type.get<std::string>());
		}

		if (elem.contains("icon"))			tab.icon = elem["icon"].get<std::string>();

		if (elem.contains("keyring"))		tab.keyring		= elem["keyring"].get<UInt32>();
		if (elem.contains("inventory"))		tab.inventory	= elem["inventory"].get<UInt32>();
		if (elem.contains("container"))		tab.container	= elem["container"].get<UInt32>();

		if (elem.contains("tabPriority"))	tab.tabPriority = elem["tabPriority"].get<SInt32>();

		if (elem.contains("tabNew"))		tab.tabNew		= elem["tabNew"].get<UInt32>();
		if (elem.contains("tabMisc"))		tab.tabMisc		= elem["tabMisc"].get<UInt32>();

		g_Tabs.emplace_back(std::make_shared<Tab>(tab));
	}

	void HandleXML(const std::filesystem::path& path)
	{
		const auto pathstring = path.generic_string();
		g_XMLPaths.emplace_back(pathstring.substr(pathstring.find_last_of("\\Data\\") - 3));
	}

	void HandleJSON(const std::filesystem::path& path)
	{
		Log(logLevel >= Log::kMessage) << "\nJSON message: reading  " + path.string();
		try
		{
			std::ifstream i(path);
			auto j = nlohmann::json::parse(i, nullptr, true, true);

			if (j.contains("items") && j["items"].is_array())
				for (const auto& elem : j["items"]) HandleItem(elem);
			else if (j.contains("tags") && j["tags"].is_array())				// legacy support
				for (const auto& elem : j["tags"]) HandleItem(elem);
			else
				Log(logLevel >= Log::kMessage) << "JSON message: ySI item array not detected in " + path.string();

			if (j.contains("categories") && j["categories"].is_array())
				for (const auto& elem : j["categories"]) HandleCategory(elem);
			else if (j.contains("icons") && j["icons"].is_array())				// legacy support
				for (const auto& elem : j["icons"]) HandleCategory(elem);
			else
				Log(logLevel >= Log::kMessage) << "JSON message: ySI category array not detected in " + path.string();

			if (j.contains("tabs") && j["tabs"].is_array())
				for (const auto& elem : j["tabs"]) HandleTab(elem);
			else
				Log(logLevel >= Log::kMessage) << "JSON message: ySI tab array not detected in " + path.string();
		}
		catch (nlohmann::json::exception& e)
		{
			Log(logLevel >= Log::kError) << "JSON error: JSON file is incorrectly formatted! It will not be applied. " + path.string();
			Log(logLevel >= Log::kError) << FormatString("JSON error: %s\n", e.what());
		}
	}
}