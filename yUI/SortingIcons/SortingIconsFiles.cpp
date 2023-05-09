#include "SortingIcons.h"

#include <GameData.h>
#include <json.h>

using namespace SortingIcons;
using namespace Files;

class Files::JSON : public nlohmann::basic_json<> {};

__forceinline std::vector<TESForm*> GetFormsFromElement(const nlohmann::basic_json<>& mod, const nlohmann::basic_json<>& form)
{
	const auto modName = mod.get<std::string>();
	std::vector<TESForm*> forms{};
	std::string log;
	if (!form.is_array()) {
		if (const auto val = TESForm::GetByID(modName.c_str(), form.get<std::string>().c_str())) forms.push_back(val);
		else log += FormatString("%6s, ", form.get<std::string>().c_str());
	}
	else for (const auto& i : form) {
		if (const auto val = TESForm::GetByID(modName.c_str(), i.get<std::string>().c_str())) forms.push_back(val);
		else log += FormatString("%6s, ", i.get<std::string>().c_str());
	}
	if (!log.empty()) Log(logLevel >= Log::kWarning) << "JSON warning: Failed to find form, mod: " + modName + ", forms: " + log;
	return forms;
}

template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
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

Object::Object(const JSON& elem)
{
	tag = elem["tag"].get<std::string>();
	priority = elem["priority"].get<SInt32>();
}

Item::Item(const JSON& elem) : Object(elem)
{
	if (elem.contains("formType"))		formType.insert_range(GetSetFromElement<UInt8>(elem["formType"]));

	if (elem.contains("questItem"))		questItem = elem["questItem"].get<UInt8>();
	if (elem.contains("miscComponent"))	miscComponent = elem["miscComponent"].get<UInt8>();
	if (elem.contains("miscProduct"))	miscProduct = elem["miscProduct"].get<UInt8>();

	forms = false;

	if (elem.contains("mod") && elem.contains("form"))
	{
		UInt32 repairList = 0;
		if (elem.contains("formlist")) repairList |= elem["formlist"].get<UInt8>();
		if (elem.contains("repairList")) repairList |= elem["repairList"].get<UInt8>();

		forms = true;

		for (const auto form : GetFormsFromElement(elem["mod"], elem["form"]))
			formIDs.insert_range(repairList ? FlattenListRepair(form) : FlattenListForm(form));
	}
}

Weapon::Weapon(const JSON& elem) : Item(elem)
{
	if (elem.contains("weaponSkill"))			skill.insert_range(GetSetFromElement<UInt32>(elem["weaponSkill"]));
	if (elem.contains("weaponHandgrip"))		handgrip.insert_range(GetSetFromElement<UInt32>(elem["weaponHandgrip"]));
	if (elem.contains("weaponAttackAnim"))		attackAnim.insert_range(GetSetFromElement<UInt32>(elem["weaponAttackAnim"]));
	if (elem.contains("weaponReloadAnim"))		reloadAnim.insert_range(GetSetFromElement<UInt32>(elem["weaponReloadAnim"]));
	if (elem.contains("weaponIsAutomatic"))		isAutomatic = elem["weaponIsAutomatic"].get<UInt8>();
	if (elem.contains("weaponHasScope"))		hasScope = elem["weaponHasScope"].get<UInt8>();
	if (elem.contains("weaponIgnoresDTDR"))		ignoresDTDR = elem["weaponIgnoresDTDR"].get<UInt8>();
	if (elem.contains("weaponClipRounds"))		clipRoundsMin = elem["weaponClipRounds"].get<UInt32>();
	if (elem.contains("weaponNumProjectiles"))	numProjectilesMin = elem["weaponNumProjectiles"].get<UInt32>();
	if (elem.contains("weaponSoundLevel"))		soundLevel.insert_range(GetSetFromElement<UInt32>(elem["weaponSoundLevel"]));

	if (elem.contains("ammoMod") && elem.contains("ammoForm")) for (const auto iter : GetFormsFromElement(elem["ammoMod"], elem["ammoForm"]))
		ammoIDs.emplace(iter->refID);

	if (elem.contains("weaponType"))			type.insert_range(GetSetFromElement<UInt32>(elem["weaponType"]));
}

Armor::Armor(const JSON& elem) : Item(elem)
{
	if (elem.contains("armorHead")) 			(elem["armorHead"].get<SInt8>() == 1 ?			slotsMaskWL : slotsMaskBL) += 1 << 0x0;
	if (elem.contains("armorHair")) 			(elem["armorHair"].get<SInt8>() == 1 ?			slotsMaskWL : slotsMaskBL) += 1 << 0x1;
	if (elem.contains("armorUpperBody")) 		(elem["armorUpperBody"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x2;
	if (elem.contains("armorLeftHand"))			(elem["armorLeftHand"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x3;
	if (elem.contains("armorRightHand")) 		(elem["armorRightHand"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x4;
	if (elem.contains("armorWeapon")) 			(elem["armorWeapon"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x5;
	if (elem.contains("armorPipBoy")) 			(elem["armorPipBoy"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x6;
	if (elem.contains("armorBackpack"))			(elem["armorBackpack"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x7;
	if (elem.contains("armorNecklace"))			(elem["armorNecklace"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x8;
	if (elem.contains("armorHeadband"))			(elem["armorHeadband"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0x9;
	if (elem.contains("armorHat")) 				(elem["armorHat"].get<SInt8>() == 1 ?			slotsMaskWL : slotsMaskBL) += 1 << 0xA;
	if (elem.contains("armorEyeglasses"))		(elem["armorEyeglasses"].get<SInt8>() == 1 ?	slotsMaskWL : slotsMaskBL) += 1 << 0xB;
	if (elem.contains("armorNosering"))			(elem["armorNosering"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0xC;
	if (elem.contains("armorEarrings"))			(elem["armorEarrings"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0xD;
	if (elem.contains("armorMask")) 			(elem["armorMask"].get<SInt8>() == 1 ?			slotsMaskWL : slotsMaskBL) += 1 << 0xE;
	if (elem.contains("armorChoker")) 			(elem["armorChoker"].get<SInt8>() == 1 ?		slotsMaskWL : slotsMaskBL) += 1 << 0xF;
	if (elem.contains("armorMouthObject"))		(elem["armorMouthObject"].get<SInt8>() == 1 ?	slotsMaskWL : slotsMaskBL) += 1 << 0x10;
	if (elem.contains("armorBodyAddon1"))		(elem["armorBodyAddon1"].get<SInt8>() == 1 ?	slotsMaskWL : slotsMaskBL) += 1 << 0x11;
	if (elem.contains("armorBodyAddon2"))		(elem["armorBodyAddon2"].get<SInt8>() == 1 ?	slotsMaskWL : slotsMaskBL) += 1 << 0x12;
	if (elem.contains("armorBodyAddon3"))		(elem["armorBodyAddon3"].get<SInt8>() == 1 ?	slotsMaskWL : slotsMaskBL) += 1 << 0x13;

	if (elem.contains("armorClass"))			armorClass = elem["armorClass"].get<UInt16>();
	if (elem.contains("armorPower"))			powerArmor = elem["armorPower"].get<SInt8>();
	if (elem.contains("armorHasBackpack"))		hasBackpack = elem["armorHasBackpack"].get<SInt8>();

	if (elem.contains("armorDT"))				dt = elem["armorDT"].get<float>();
	if (elem.contains("armorDR"))				dr = elem["armorDR"].get<UInt16>();
	if (elem.contains("armorChangesAV"))		changesAV = elem["armorChangesAV"].get<UInt16>();
}

Aid::Aid(const JSON& elem) : Item(elem)
{
	if (elem.contains("aidRestoresAV"))			restoresAV = elem["aidRestoresAV"].get<UInt8>();
	if (elem.contains("aidDamagesAV"))			damagesAV = elem["aidDamagesAV"].get<UInt8>();
	if (elem.contains("aidIsAddictive"))		isAddictive = elem["aidIsAddictive"].get<UInt8>();
	if (elem.contains("aidIsFood"))				isFood = elem["aidIsFood"].get<UInt8>();
	if (elem.contains("aidIsWater"))			isWater = elem["aidIsWater"].get<UInt8>();
	if (elem.contains("aidIsMedicine"))			isMedicine = elem["aidIsMedicine"].get<UInt8>();
	if (elem.contains("aidIsPoisonous"))		isPoisonous = elem["aidIsPoisonous"].get<UInt8>();
}

Icon::Icon(const JSON& elem) : Object(elem)
{
	if (elem.contains("template"))			xmltemplate = elem["template"].get<std::string>();
	if (elem.contains("filename"))			filename = elem["filename"].get<std::string>();
	if (elem.contains("texatlas"))			texatlas = elem["texatlas"].get<std::string>();
	if (elem.contains("systemcolor"))		systemcolor = elem["systemcolor"].get<SInt32>();
	if (elem.contains("font"))				font = elem["font"].get<SInt32>();
}

Category::Category(const JSON& elem) : Object(elem)
{
	if (elem.contains("name"))			name = UTF8toANSI(elem["name"].get<std::string>());

	if (elem.contains("types"))			types.insert_range(GetSetFromElement<UInt32>(elem["types"]));
	if (elem.contains("categories"))	categories.insert_range(GetSetFromElement<std::string>(elem["categories"]));

	if (elem.contains("tabNew"))		tabNew = elem["tabNew"].get<UInt32>();
	if (elem.contains("tabMisc"))		tabMisc = elem["tabMisc"].get<UInt32>();
}

Tab::Tab(const JSON& elem) : Category(elem)
{
	if (elem.contains("inventory"))		inventory = elem["inventory"].get<UInt32>();
	if (elem.contains("container"))		container = elem["container"].get<UInt32>();

	if (elem.contains("tabPriority"))	tabPriority = elem["tabPriority"].get<SInt32>();
}

Keyring::Keyring(const JSON& elem) : Category(elem)
{
	if (elem.contains("icon"))			icon = elem["icon"].get<std::string>();

	if (elem.contains("keyring"))		keyring = elem["keyring"].get<UInt32>();
}

void Files::HandleXML(const std::filesystem::path& path)
{
	const auto pathstring = path.generic_string();
	g_XMLPaths.emplace_back(pathstring.substr(pathstring.find_last_of("\\Data\\") - 3));
}

void Files::HandleJSON(const std::filesystem::path& path)
{
	Log(logLevel >= Log::kMessage) << "\nJSON message: reading  " + path.string();
	try
	{
		std::ifstream i(path);
		auto j = nlohmann::json::parse(i, nullptr, true, true);

		nlohmann::basic_json items;

		if (j.contains("items")) items = j["items"];
		else if (j.contains("tags")) items = j["tags"]; // legacy

		if (!items.is_array()) Log(logLevel >= Log::kMessage) << "JSON message: ySI item array not detected in " + path.string();
		else for (const auto& elem : items) if (!elem.is_object())
		{
			Log(logLevel >= Log::kError) << "JSON error: Expected object";
			return;
		}
		else if (!elem.contains("tag") || !elem.contains("priority"))
		{
			Log(logLevel >= Log::kWarning) << "JSON error: Expected tag and priority";
			return;
		}
		else
		{
			std::unordered_set<UInt8> formType;

			if (elem.contains("formType")) formType.insert_range(GetSetFromElement<UInt8>(elem["formType"]));

			std::unique_ptr<Item> item;
			
			if (formType.contains(kFormType_TESObjectWEAP))			item = std::make_unique<Weapon>(JSON(elem));
			else if (formType.contains(kFormType_TESObjectARMO))	item = std::make_unique<Armor>(JSON(elem));
			else if (formType.contains(kFormType_AlchemyItem))		item = std::make_unique<Aid>(JSON(elem));
			else													item = std::make_unique<Item>(JSON(elem));

			if (item->IsValid()) g_Items.emplace_back(std::move(item));
		}

		nlohmann::basic_json categories;

		if (j.contains("categories")) categories = j["categories"];
		else if (j.contains("icons")) categories = j["icons"]; // legacy

		if (!categories.is_array()) Log(logLevel >= Log::kMessage) << "JSON message: ySI category array not detected in " + path.string();
		else for (const auto& elem : categories) if (!elem.is_object())
		{
			Log(logLevel >= Log::kError) << "JSON error: Expected object";
			return;
		}
		else if (!elem.contains("tag") || !elem.contains("priority"))
		{
			Log(logLevel >= Log::kWarning) << "JSON error: Expected tag and priority";
			return;
		}
		else
		{
			auto category = std::make_unique<Icon>(JSON(elem));
			if (category->IsValid()) g_Icons.emplace_back(std::move(category));
		}

		nlohmann::basic_json tabs;

		if (j.contains("tabs")) tabs = j["tabs"];

		if (!tabs.is_array()) Log(logLevel >= Log::kMessage) << "JSON message: ySI tabs array not detected in " + path.string();
		else for (const auto& elem : tabs) if (!elem.is_object())
		{
			Log(logLevel >= Log::kError) << "JSON error: Expected object";
			return;
		}
		else if (!elem.contains("tag") || !elem.contains("priority"))
		{
			Log(logLevel >= Log::kWarning) << "JSON error: Expected tag and priority";
			return;
		}
		else
		{
			std::unique_ptr<Category> tab;

			if (elem.contains("keyring"))							tab = std::make_unique<Keyring>(JSON(elem));
			else													tab = std::make_unique<Tab>(JSON(elem));

			if (tab->IsValid()) g_Categories.emplace_back(std::move(tab));
		}
	}
	catch (nlohmann::json::exception& e)
	{
		Log(logLevel >= Log::kError) << "JSON error: JSON file is incorrectly formatted! It will not be applied. " + path.string();
		Log(logLevel >= Log::kError) << FormatString("JSON error: %s\n", e.what());
	}
}
