#include <SortingIcons.h>
#include <GameRTTI.h>
#include <settings.h>
#include <functions.h>
#include <filesystem>
#include <json.h>

#include <GameData.h>
#include <GameAPI.h>
#include <GameRTTI.h>
#include <Utilities.h>
#include <fstream>

void LoadSIMapsFromFiles()
{
	Log("Loading files");
	const auto dir = GetCurPath() + R"(\Data\menus\ySI)";
	const auto then = std::chrono::system_clock::now();
	if (std::filesystem::exists(dir))
	{
		for (std::filesystem::directory_iterator iter(dir.c_str()), end; iter != end; ++iter)
		{
			const auto& path = iter->path();
			const auto& fileName = path.filename();
			if (iter->is_directory())
				Log(iter->path().string() + " found");
			else if (_stricmp(path.extension().string().c_str(), ".json") == 0)
				SIFiles::HandleSIJson(iter->path());
			else if (_stricmp(path.extension().string().c_str(), ".xml") == 0)
			{
				auto pathstring = iter->path().generic_string();
				auto relativepath = pathstring.substr(pathstring.find_last_of("\\Data\\") - 3);
				SI::g_XMLPaths.emplace_back(std::filesystem::path(relativepath));
			}
		}
	}
	else
		Log(dir + " does not exist.");
	SIFiles::FillSIMapsFromJSON();
	const auto now = std::chrono::system_clock::now();
	const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
	Log(FormatString("Loaded items, categories and tabs in %d ms", diff.count()));
}

namespace SI
{
	std::string stringStewie;
	std::string openCategory;
	float compassRoseX = 0, compassRoseY = 0;
}

namespace SIFiles
{
	void JSONEntryItemRecursiveEmplace(const JSONEntryItemCommon&, TESForm*);

	void HandleSIItem(nlohmann::basic_json<> elem)
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
		JSONEntryItemCommon common{};

		common.tag =												elem["tag"].get<std::string>();
		if (elem.contains("priority"))		common.priority =		elem["priority"].get<SInt32>();
		if (elem.contains("formType"))		common.formType =		elem["formType"].get<UInt32>();
		if (elem.contains("questItem"))		common.questItem =		elem["questItem"].get<UInt8>();
		if (elem.contains("miscComponent"))	common.miscComponent =	elem["miscComponent"].get<UInt8>();
		if (elem.contains("miscProduct"))	common.miscProduct =	elem["miscProduct"].get<UInt8>();

		if (elem.contains("mod") && elem.contains("form"))
		{
			auto modName = elem.contains("mod") ? elem["mod"].get<std::string>() : "";
			const auto mod = !modName.empty() ? DataHandler::GetSingleton()->LookupModByName(modName.c_str()) : nullptr;

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
						g_Items_JSON.emplace_back(common);
					}
					else if (formlist == 1) {
						JSONEntryItemRecursiveEmplace(common, common.form);
					}
					else if (formlist == 2) {
						for (const auto item : *GetAllForms()) {
							if (item->typeID == kFormType_TESObjectWEAP) {
								const auto weapon = DYNAMIC_CAST(item, TESForm, TESObjectWEAP);
								if (!weapon || !weapon->repairItemList.listForm) continue;
								if (weapon->refID == common.form->refID || weapon->repairItemList.listForm->refID == common.form->refID || weapon->repairItemList.listForm->ContainsRecursive(common.form)) {
									Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X'", common.tag.c_str(), item->refID, item->GetName(), formId));
									g_Items_JSON.emplace_back(common, item);
								}
							} else if (item->typeID == kFormType_TESObjectARMO) {
								const auto armor = DYNAMIC_CAST(item, TESForm, TESObjectARMO);
								if (!armor || !armor->repairItemList.listForm) continue;
								if (armor->refID == common.form->refID || armor->repairItemList.listForm->refID == common.form->refID || armor->repairItemList.listForm->ContainsRecursive(common.form)) {
									Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive, repair list: '%08X'", common.tag.c_str(), item->refID, item->GetName(), formId));
									g_Items_JSON.emplace_back(common, item);
								}
							}
						}
					}
				}
			}
			else {
				Log(FormatString("Tag: '%10s', mod: '%s'", common.tag.c_str(), modName.c_str()));
				g_Items_JSON.emplace_back(common);
			}
		}
		else if (common.formType == 40 || elem.contains("weaponSkill") || elem.contains("weaponType"))
		{
			common.formType = 40;
			JSONEntryItemWeapon weapon{};

			if (elem.contains("weaponSkill"))			weapon.skill =			elem["weaponSkill"].get<UInt32>();
			if (elem.contains("weaponHandgrip"))		weapon.handgrip =		elem["weaponHandgrip"].get<UInt32>();
			if (elem.contains("weaponAttackAnim"))		weapon.attackAnim =		elem["weaponAttackAnim"].get<UInt32>();
			if (elem.contains("weaponReloadAnim"))		weapon.reloadAnim =		elem["weaponReloadAnim"].get<UInt32>();
			if (elem.contains("weaponIsAutomatic"))		weapon.isAutomatic =	elem["weaponIsAutomatic"].get<UInt32>();
			if (elem.contains("weaponHasScope"))		weapon.hasScope =		elem["weaponHasScope"].get<UInt32>();
			if (elem.contains("weaponIgnoresDTDR"))		weapon.ignoresDTDR =	elem["weaponIgnoresDTDR"].get<UInt32>();
			if (elem.contains("weaponClipRounds"))		weapon.clipRounds =		elem["weaponClipRounds"].get<UInt32>();
			if (elem.contains("weaponNumProjectiles"))	weapon.numProjectiles =	elem["weaponNumProjectiles"].get<UInt32>();
			if (elem.contains("weaponSoundLevel"))		weapon.soundLevel =		elem["weaponSoundLevel"].get<UInt32>();

			if (elem.contains("ammoMod") && elem.contains("ammoForm"))
			{
				auto ammoModName = elem["ammoMod"].get<std::string>();
				const auto ammoMod = !ammoModName.empty() ? DataHandler::GetSingleton()->LookupModByName(ammoModName.c_str()) : nullptr;
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
					g_Items_JSON.emplace_back(common, weapon);
				}
			}
			else {
				Log(FormatString("Tag: '%10s', weapon condition", common.tag.c_str()));
				g_Items_JSON.emplace_back(common, weapon);
			}
		}
		else if (common.formType == 24)
		{
			common.formType = 24;
			JSONEntryItemArmor armor{};

			if (elem.contains("armorHead")) 		(elem["armorHead"].get<SInt8>() == 1 ?			armor.slotsMaskWL : armor.slotsMaskBL) += 1;
			if (elem.contains("armorHair")) 		(elem["armorHair"].get<SInt8>() == 1 ?			armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 1;
			if (elem.contains("armorUpperBody")) 	(elem["armorUpperBody"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 2;
			if (elem.contains("armorLeftHand")) 	(elem["armorLeftHand"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 3;
			if (elem.contains("armorRightHand")) 	(elem["armorRightHand"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 4;
			if (elem.contains("armorWeapon")) 		(elem["armorWeapon"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 5;
			if (elem.contains("armorPipBoy")) 		(elem["armorPipBoy"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 6;
			if (elem.contains("armorBackpack")) 	(elem["armorBackpack"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 7;
			if (elem.contains("armorNecklace")) 	(elem["armorNecklace"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 8;
			if (elem.contains("armorHeadband")) 	(elem["armorHeadband"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 9;
			if (elem.contains("armorHat")) 			(elem["armorHat"].get<SInt8>() == 1 ?			armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 10;
			if (elem.contains("armorEyeglasses")) 	(elem["armorEyeglasses"].get<SInt8>() == 1 ?	armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 11;
			if (elem.contains("armorNosering")) 	(elem["armorNosering"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 12;
			if (elem.contains("armorEarrings")) 	(elem["armorEarrings"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 13;
			if (elem.contains("armorMask")) 		(elem["armorMask"].get<SInt8>() == 1 ?			armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 14;
			if (elem.contains("armorChoker")) 		(elem["armorChoker"].get<SInt8>() == 1 ?		armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 15;
			if (elem.contains("armorMouthObject"))	(elem["armorMouthObject"].get<SInt8>() == 1 ?	armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 16;
			if (elem.contains("armorBodyAddon1"))	(elem["armorBodyAddon1"].get<SInt8>() == 1 ?	armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 17;
			if (elem.contains("armorBodyAddon2"))	(elem["armorBodyAddon2"].get<SInt8>() == 1 ?	armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 18;
			if (elem.contains("armorBodyAddon3"))	(elem["armorBodyAddon3"].get<SInt8>() == 1 ?	armor.slotsMaskWL : armor.slotsMaskBL) += 1 << 19;

			if (elem.contains("armorClass"))		armor.armorClass =	elem["armorClass"].get<UInt16>();
			if (elem.contains("armorPower"))		armor.powerArmor =	elem["armorPower"].get<SInt8>();
			if (elem.contains("armorHasBackpack"))	armor.hasBackpack =	elem["armorHasBackpack"].get<SInt8>();

			if (elem.contains("armorDT"))			armor.dt =			elem["armorDT"].get<float>();
			if (elem.contains("armorDR"))			armor.dr =			elem["armorDR"].get<UInt16>();
			if (elem.contains("armorChangesAV"))	armor.changesAV =	elem["armorChangesAV"].get<UInt16>();

			g_Items_JSON.emplace_back(common, armor);
		}
		else if (common.formType == 31 || elem.contains("miscComponent"))
		{
			common.formType = 31;
			JSONEntryItemMisc misc{};
			g_Items_JSON.emplace_back(common, misc);
		}
		else if (common.formType == 47 || elem.contains("IsFood") || elem.contains("IsMedicine"))
		{
			common.formType = 47;
			JSONEntryItemAid aid{};

			if (elem.contains("aidRestoresAV"))			aid.restoresAV =	elem["aidRestoresAV"].get<UInt8>();
			if (elem.contains("aidDamagesAV"))			aid.damagesAV =		elem["aidDamagesAV"].get<UInt8>();
			if (elem.contains("aidIsAddictive"))		aid.isAddictive =	elem["aidIsAddictive"].get<UInt8>();
			if (elem.contains("aidIsFood"))				aid.isFood =		elem["aidIsFood"].get<UInt8>();
			if (elem.contains("aidIsWater"))			aid.isWater =		elem["aidIsWater"].get<UInt8>();
			if (elem.contains("aidIsMedicine"))			aid.isMedicine =	elem["aidIsMedicine"].get<UInt8>();
			if (elem.contains("aidIsPoisonous"))		aid.isPoisonous =	elem["aidIsPoisonous"].get<UInt8>();
			g_Items_JSON.emplace_back(common, aid);
		}
		else g_Items_JSON.emplace_back(common);
	}

	void HandleSICategory(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log("JSON error: expected object");
			return;
		}

		JSONEntryCategory category{};

		category.tag =													elem["tag"].get<std::string>();
		if (elem.contains("priority"))			category.priority =		elem["priority"].get<SInt16>();
		if (elem.contains("template"))			category.xmltemplate =	elem["template"].get<std::string>();
		if (elem.contains("filename"))			category.filename =		elem["filename"].get<std::string>();
		if (elem.contains("texatlas"))			category.texatlas =		elem["texatlas"].get<std::string>();
		if (elem.contains("systemcolor"))		category.systemcolor =	elem["systemcolor"].get<SInt32>();
		if (elem.contains("category"))			category.category =		elem["category"].get<std::string>();
		if (elem.contains("routeToKeyring"))	category.category =		elem["routeToKeyring"].get<std::string>();
		if (elem.contains("name"))				category.name =			UTF8toANSI(elem["name"].get<std::string>());
		if (elem.contains("icon"))				category.icon =			elem["icon"].get<std::string>();
		if (elem.contains("tab"))				category.tab =			elem["tab"].get<UInt32>();
		if (elem.contains("count"))				category.count =		elem["count"].get<UInt32>();

		Log(FormatString("Tag: '%10s', icon: '%s'", category.tag.c_str(), category.filename.c_str()));
		g_Categories_JSON.emplace_back(category);
	}

	void HandleSITab(nlohmann::basic_json<> elem)
	{
		if (!elem.is_object())
		{
			Log("JSON error: expected object with mod, form and folder fields");
			return;
		}

		JSONEntryTab tab{};
		tab.tab =												elem["tab"].get<std::string>();
		if (elem.contains("name"))			tab.name =			UTF8toANSI(elem["name"].get<std::string>());
		if (elem.contains("priority"))		tab.priority =		elem["priority"].get<SInt32>();
		if (elem.contains("tabNew"))		tab.tabNew =		elem["tabNew"].get<UInt32>();
		if (elem.contains("inventory"))		tab.inventory =		elem["inventory"].get<UInt32>();
		if (elem.contains("container"))		tab.container =		elem["container"].get<UInt32>();
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

		g_Tabs_JSON.push_back(tab);
	}

	void HandleSIJson(const std::filesystem::path& path)
	{
		Log("\nReading JSON file " + path.string());

		try
		{
			std::ifstream i(path);
			auto j = nlohmann::json::parse(i, nullptr, true, true);

			if (j.contains("items") && j["items"].is_array())
				for (const auto& elem : j["items"]) HandleSIItem(elem);
			else if (j.contains("tags") && j["tags"].is_array())				// legacy support
				for (const auto& elem : j["tags"]) HandleSIItem(elem);
			else 
				Log(path.string() + " JSON ySI item array not detected");

			if (j.contains("categories") && j["categories"].is_array())
				for (const auto& elem : j["categories"]) HandleSICategory(elem);
			else if (j.contains("icons") && j["icons"].is_array())				// legacy support
				for (const auto& elem : j["icons"]) HandleSICategory(elem);
			else
				Log(path.string() + " JSON ySI category array not detected");

			if (j.contains("tabs") && j["tabs"].is_array())
				for (const auto& elem : j["tabs"]) HandleSITab(elem);
			else
				Log(path.string() + " JSON ySI tab array not detected");
		}
		catch (nlohmann::json::exception& e)
		{
			Log("The JSON is incorrectly formatted! It will not be applied.");
			Log(FormatString("JSON error: %s\n", e.what()));
		}

	}

	void JSONEntryItemRecursiveEmplace(const JSONEntryItemCommon& common, TESForm* item)
	{
		if (item->typeID == kFormType_BGSListForm)
		{
			const auto bgslist = DYNAMIC_CAST(item, TESForm, BGSListForm);
			if (!bgslist) return;
			for (const auto iter : bgslist->list)
				if (iter) JSONEntryItemRecursiveEmplace(common, iter);
		} else if (!common.formType || item->typeID == common.formType) {
			Log(FormatString("Tag: '%10s', form: %08X (%50s), recursive", common.tag.c_str(), item->refID, item->GetName()));
			g_Items_JSON.emplace_back(common, item);
		}
	}
}

namespace SIFiles
{
	bool AssignCategoryToItem(TESForm* form)
	{
		for (const auto &entry : g_Items_JSON) {
			if (entry.common.form			&& entry.common.form->refID != form->refID) continue;
			if (entry.common.formType		&& entry.common.formType != form->typeID) continue;

			if (entry.common.questItem		&& entry.common.questItem != static_cast<UInt8>(form->IsQuestItem2())) continue;
			if (entry.common.miscComponent	&& !IsCraftingComponent(form)) continue;
			if (entry.common.miscProduct	&& !IsCraftingProduct(form)) continue;

			if (entry.common.formType == 40) {
				const auto weapon = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
				if (!weapon) continue;
				if (entry.weapon.skill				&& entry.weapon.skill != weapon->weaponSkill) continue;
				if (entry.weapon.type				&& entry.weapon.type != weapon->eWeaponType) continue;
				if (entry.weapon.handgrip			&& entry.weapon.handgrip != weapon->HandGrip()) continue;
				if (entry.weapon.attackAnim			&& entry.weapon.attackAnim != weapon->AttackAnimation()) continue;
				if (entry.weapon.reloadAnim			&& entry.weapon.reloadAnim != weapon->reloadAnim) continue;
				if (entry.weapon.type				&& entry.weapon.type != weapon->eWeaponType) continue;
				if (entry.weapon.isAutomatic		&& entry.weapon.isAutomatic != static_cast<UInt32>(weapon->IsAutomatic())) continue;
				if (entry.weapon.hasScope			&& entry.weapon.hasScope != static_cast<UInt32>(weapon->HasScopeAlt())) continue;
				if (entry.weapon.ignoresDTDR		&& entry.weapon.ignoresDTDR != static_cast<UInt32>(weapon->IgnoresDTDR())) continue;
				if (entry.weapon.clipRounds			&& entry.weapon.clipRounds > static_cast<UInt32>(weapon->GetClipRounds(false))) continue;
				if (entry.weapon.numProjectiles		&& entry.weapon.numProjectiles > weapon->numProjectiles) continue;
				if (entry.weapon.soundLevel			&& entry.weapon.soundLevel != weapon->soundLevel) continue;
				if (entry.weapon.ammo				&& !FormContainsRecusive(entry.weapon.ammo, weapon->ammo.ammo)) continue;
			}
			else if (entry.common.formType == 24) {
				const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
				if (!armor) continue;
				if (entry.armor.slotsMaskWL			&& (entry.armor.slotsMaskWL & armor->GetArmorValue(6)) != entry.armor.slotsMaskWL) continue;
				if (entry.armor.slotsMaskBL			&& (entry.armor.slotsMaskBL & armor->GetArmorValue(6)) != 0) continue;
				if (entry.armor.armorClass			&& entry.armor.armorClass != armor->GetArmorValue(1)) continue;
				if (entry.armor.powerArmor			&& entry.armor.powerArmor != armor->GetArmorValue(2)) continue;
				if (entry.armor.hasBackpack			&& entry.armor.hasBackpack != armor->GetArmorValue(3)) continue;

				if (entry.armor.dt					&& entry.armor.dt > armor->damageThreshold) continue;
				if (entry.armor.dr					&& entry.armor.dr > armor->armorRating) continue;
//				if (entry.armor.armorChangesAV && entry.armor.armorChangesAV > armor->armorRating) continue;
			}
			else if (entry.common.formType == 31) {
			}
			else if (entry.common.formType == 47) {
				const auto aid = DYNAMIC_CAST(form, TESForm, AlchemyItem);
				if (!aid) continue;
				if (entry.aid.restoresAV				&& !aid->HasBaseEffectRestoresAV(entry.aid.restoresAV)) continue;
				if (entry.aid.damagesAV				&& !aid->HasBaseEffectDamagesAV(entry.aid.damagesAV)) continue;
				if (entry.aid.isAddictive			&& !aid->IsAddictive()) continue;
				if (entry.aid.isFood					&& !aid->IsFood()) continue;
				if (entry.aid.isWater				&& !aid->IsWaterAlt()) continue;
				if (entry.aid.isPoisonous			&& !aid->IsPoison()) continue;
				if (entry.aid.isMedicine				&& !aid->IsMedicine()) continue;
			}

			SI::g_ItemToCategory.emplace(form, entry.common.tag);
			return true;
		}
		return false;
	}

	bool AssignFiltersToItem(TESForm* form)
	{
		std::unordered_set<std::string> set;
		for (const auto& [fst, snd] : SI::g_Tabs) {
			if (!snd.tabMisc.empty()) continue;
			if (!snd.types.empty() && !snd.types.contains(form->typeID)) continue;
			if (!snd.categories.empty() && !snd.categories.contains(SI::GetCategoryForItem(form))) continue;
			set.emplace(fst);
		}

		for (const auto& [fst, snd] : SI::g_Tabs) {
			if (snd.tabMisc.empty()) continue;
			bool misc = true;
			for (const auto& it : snd.tabMisc) if (set.contains(it)) { misc = false; break; }
			if (misc) set.emplace(fst);
		}

		SI::g_ItemToFilter.emplace(form, std::move(set));
		return true;
	}

	void FillSIMapsFromJSON()
	{
		ra::sort(g_Items_JSON, [&](const JSONEntryItem& entry1, const JSONEntryItem& entry2)
		         { return entry1.common.priority > entry2.common.priority; });

		if (!g_ySI_JustInTime)
		{
			for (const auto form : *GetAllForms()) {
				if (!form || !form->IsInventoryObjectAlt()) continue;
				AssignCategoryToItem(form);
			}
//			SIFiles::g_Items_JSON = std::vector<JSONEntryItem>();
		}

		ra::sort(g_Categories_JSON, [&](const JSONEntryCategory& entry1, const JSONEntryCategory& entry2)
		         { return entry1.priority > entry2.priority; });

		for (auto& entry : g_Categories_JSON) {
			if (!entry.name.empty()) SI::g_Keyrings.emplace(entry.tag);
			SI::g_Categories.emplace(entry.tag, std::move(entry));
		}
		g_Categories_JSON = std::vector<JSONEntryCategory>();

		ra::sort(g_Tabs_JSON, [&](const JSONEntryTab& entry1, const JSONEntryTab& entry2)
				{ return entry1.priority > entry2.priority; });
		for (auto& entry : g_Tabs_JSON) SI::g_Tabs.emplace(entry.tab, std::move(entry));

		std::vector<JSONEntryTab> vector;
		for (const auto& [fst, snd] : SI::g_Tabs) vector.push_back(snd);
		ra::sort(vector, [&](const JSONEntryTab& entry1, const JSONEntryTab& entry2)
				{ return entry1.tabPriority > entry2.tabPriority; });
		for (const auto& it : vector) if (it.inventory) SI::g_Tabline.push_back(it.tab);

		g_Tabs_JSON = std::vector<JSONEntryTab>();
	}
}

namespace SI
{
	void InjectTemplates()
	{
		for (auto& iter : g_XMLPaths)
		{
			HUDMainMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
			RepairMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
			InventoryMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
		}
	}

	void KeyringRefreshPostStewie()
	{
		if (CdeclCall<bool>(0x702360) && InterfaceManager::IsMenuVisible(kMenuType_Inventory) && InventoryMenu::GetSingleton()->IsKeyringOpen()) {
			if (Tile* stew = InventoryMenu::GetSingleton()->tile->GetChild("IM_SearchBar"); stew) {
				if (const auto string = stew->GetValue(kTileValue_string)->str; !stringStewie._Equal(string)) {
					stringStewie = string;
					InventoryMenu::GetSingleton()->itemsList.Filter(KeyringHideNonKeys);
					InventoryMenu::GetSingleton()->itemsList.ForEach(reinterpret_cast<void(*)(Tile*, ContChangesEntry*)>(0x780C00));
				}
			}
			else stringStewie.clear();
		}
	}

	bool IsTagForItem(TESForm* form)
	{
		if (form && g_ItemToCategory.contains(form)) return true;
		return false;
	}

	bool IsTagForItem(ContChangesEntry* entry)
	{
		if (entry && entry->form && g_ItemToCategory.contains(entry->form)) return true;
		return false;
	}
	
	std::string GetCategoryForItem(TESForm* form)
	{
		if (!form) return "";
		if (!IsTagForItem(form)) SIFiles::AssignCategoryToItem(form);
		return g_ItemToCategory[form];
	}

	std::string GetCategoryForItem(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return "";
		if (!IsTagForItem(entry)) SIFiles::AssignCategoryToItem(entry->form);
		return g_ItemToCategory[entry->form];
	}

	bool IsFilterForItem(TESForm* form)
	{
		if (form && g_ItemToFilter.contains(form)) return true;
		return false;
	}

	bool IsFilterForItem(ContChangesEntry* entry)
	{
		if (entry && entry->form && g_ItemToFilter.contains(entry->form)) return true;
		return false;
	}

	bool InjectTemplates(TileMenu* tilemenu, std::string templateName) {
		const auto menu = tilemenu->menu;
		if (!menu->GetTemplateExists(templateName.c_str()))
		{
			if (menu->id != kMenuType_Barter && menu->id != kMenuType_Container && menu->id != kMenuType_RepairServices) return false;
			for (auto& iter : g_XMLPaths) tilemenu->InjectUIXML(iter.generic_string().c_str());
			if (!menu->GetTemplateExists(templateName.c_str())) return false;
		}
		return true;
	}

	void InjectIconTile(const SIFiles::JSONEntryCategory& category, MenuItemEntryList* list, Tile* tile, ContChangesEntry* entry)
	{
		//	if (g_Items.find(entry->type) == g_Items.end()) return;
		if (category.filename.empty()) return;

		TileMenu* menu = tile->GetTileMenu();

		if (!menu || !menu->menu) return;

		const auto text = tile->children.Tail()->data;

		if (!text || !std::string("ListItemText")._Equal(text->name.CStr())) return;

		Tile* icon = tile->GetChild(category.xmltemplate.c_str());

		if (!icon) {
			if (!InjectTemplates(menu, category.xmltemplate)) return;
			const auto last = tile->children.Head();
			icon = tile->AddTileFromTemplate(category.xmltemplate.c_str());
			tile->children.ExchangeNodeData(tile->children.Head(), last);
			if (!icon) return;
		}

		if (!category.filename.empty()) icon->SetString(kTileValue_filename, category.filename.c_str(), false);
		if (!category.texatlas.empty()) icon->SetString(kTileValue_texatlas, category.texatlas.c_str(), false);
		if (!category.systemcolor) {
			icon->SetFloat(kTileValue_systemcolor, menu->GetFloat(kTileValue_systemcolor));
		} else {
			icon->SetFloat(kTileValue_systemcolor, category.systemcolor, false);
		}
		//	icon->SetFloat(kTileValue_alpha, 255, propagate);

		Float32 x = text->GetFloat(kTileValue_x);

		if (icon->GetValue(kTileValue_user0)) x += icon->GetFloat(kTileValue_user0);

		icon->SetFloat(kTileValue_x, x, true);

		x += icon->GetFloat(kTileValue_width);

		if (icon->GetValue(kTileValue_user1)) x += icon->GetFloat(kTileValue_user1);

		text->SetFloat(kTileValue_x, x, true);
		text->SetFloat(kTileValue_wrapwidth, text->GetFloat(kTileValue_wrapwidth) - x, true);
	}

	void __fastcall SetTileStringInjectTile(Tile* tile, ContChangesEntry* entry, MenuItemEntryList* list, const eTileValue tilevalue, const char* tileText, bool propagate)
	{
		tile->SetString(tilevalue, tileText, propagate);
		if (entry && entry->form && TryGetTypeOfForm(entry->form)) InjectIconTile(g_Categories[GetCategoryForItem(entry)], list, tile, entry);
	}

	SInt32 __fastcall CompareItemsWithTags(const TileContChangesEntryUnk* unk1, const TileContChangesEntryUnk* unk2)
	{
		const auto a1 = unk1->entry;
		const auto a2 = unk2->entry;
		const auto tile1 = unk1->tile;
		const auto tile2 = unk2->tile;

		TESForm* form1 = nullptr, * form2 = nullptr;

		if (a1 && a1->form) form1 = a1->form;
		if (a2 && a2->form) form2 = a2->form;

		signed int cmp;

		if (g_ySI_Sort && !g_ItemToCategory.empty())
		{
			std::string tag1, tag2;

			if (form1) tag1 = GetCategoryForItem(a1);
			if (form2) tag2 = GetCategoryForItem(a2);

			if (g_ySI_Categories && !g_Keyrings.empty())
			{
				if (tag1.empty() && tile1 && tile1->GetValue(kTileValue_user16)) tag1 = tile1->GetValue(kTileValue_user16)->str;
				if (tag2.empty() && tile2 && tile2->GetValue(kTileValue_user16)) tag2 = tile2->GetValue(kTileValue_user16)->str;
			}

			if (tag1.empty())
			{
				if (!tag2.empty()) return 1;
			}
			else if (tag2.empty()) return -1;
			else
			{
				cmp = tag1.compare(tag2);
				if (cmp > 0) return 1;
				if (cmp < 0) return -1;
			}
		}
		std::string name1, name2;

		if (form1) name1 = form1->GetTheName();
		if (form2) name2 = form2->GetTheName();

		if (name1.empty() && tile1->GetValue(kTileValue_string)) name1 = tile1->GetValue(kTileValue_string)->str;
		if (name2.empty() && tile2->GetValue(kTileValue_string)) name2 = tile2->GetValue(kTileValue_string)->str;

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

	void __fastcall SetStringValueTagImage(Tile* tile, ContChangesEntry* entry, eTileValue tilevalue, char* src, char propagate)
	{
		if (!tile) return;

		if (Tile* icon = tile->GetChild("HK_Icon"); icon) {
			icon->SetFloat(kTileValue_width, tile->GetFloat(kTileValue_width) - 16, propagate);
			icon->SetFloat(kTileValue_height, tile->GetFloat(kTileValue_height) - 16, propagate);
			icon->SetFloat(kTileValue_x, 8, propagate);
			icon->SetFloat(kTileValue_y, 8, propagate);
		}

		const std::string tag = GetCategoryForItem(entry);

		if (!IsTagForItem(entry)) {
			tile->SetString(tilevalue, src, propagate);
			return;
		}

		tile->SetString(tilevalue, g_Categories[tag].filename.empty() ? src : g_Categories[tag].filename.c_str(), propagate);
	}

	void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, eTileValue tilevalue, char* src, char propagate)
	{
		if (!tile) return;

		if (compassRoseX == 0) compassRoseX = tile->GetFloat(kTileValue_x);
		if (compassRoseY == 0) compassRoseY = tile->GetFloat(kTileValue_y);

		tile->SetFloat(kTileValue_width, 44, propagate);
		tile->SetFloat(kTileValue_height, 44, propagate);
		tile->SetFloat(kTileValue_x, compassRoseX + 3, propagate);
		tile->SetFloat(kTileValue_y, compassRoseY + 3, propagate);

		const std::string tag = GetCategoryForItem(entry);

		if (!IsTagForItem(entry)) {
			tile->SetString(tilevalue, src, propagate);
			return;
		}

		tile->SetString(tilevalue, g_Categories[tag].filename.empty() ? src : g_Categories[tag].filename.c_str(), propagate);
	}

	bool __fastcall HasContainerChangesEntry(ContChangesEntry* entry)
	{
		if (entry && entry->form) return false;
		return true;
	}

	bool __fastcall KeyringShowCategories(Tile* tile)
	{
		std::string tag;
		//&& *((UInt32*)0x78028E) == (UInt32)0x782620 && stringStewie.empty()
		if (tile && tile->GetValue(kTileValue_user16)) tag = tile->GetValue(kTileValue_user16)->str;
		if (g_Categories[tag].tab != InventoryMenu::GetSingleton()->filter) return true;

		const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
		UInt32 keys = 0;
		if (!entryDataList) return true;
		for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			if (iter->data && iter->data->form && g_Categories[GetCategoryForItem(iter->data)].category._Equal(tag))
			{
				if (g_Categories[tag].count == 0) {
					keys = 1;
					break;
				}
				else if (g_Categories[tag].count == 1)
					keys += 1;
				else if (g_Categories[tag].count == 2)
					keys += iter->data->countDelta;
			}
		if (!keys) return true;
		std::string keyringname = g_Categories[tag].name;
		if (keyringname.find("&-") == 0)
			keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
		if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";

		tile->SetString(kTileValue_string, keyringname.c_str(), false);
		return false;
	}

	bool __fastcall KeyringHideKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return false;
		if (!g_Categories[GetCategoryForItem(entry)].category.empty()) return true;
		return false;
	}

	bool __cdecl KeyringHideNonKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return true;
		if (g_Categories[GetCategoryForItem(entry)].category._Equal(openCategory)) {
			if (stringStewie.empty() || stringStewie._Equal("_")) return false;
			return !stristr(entry->form->GetTheName(), stringStewie.c_str());
		}
		return true;
	}

	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile)
	{
		if (tile && tile->GetValue(kTileValue_user16)) openCategory = tile->GetValue(kTileValue_user16)->str; else openCategory = "";
		invmenu->itemsList.Filter(KeyringHideNonKeys);
		invmenu->itemsList.ForEach((void(__cdecl*)(Tile*, ContChangesEntry*))0x780C00);
		invmenu->ResetInventorySelectionAndHideDataTile();
	}
	
	void __fastcall AddSortingCategories()
	{
		const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
		for (auto& entry : g_Keyrings)
		{
			UInt32 keys = 0;
			if (!entryDataList) return;
			for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			{
				if (!iter->data || !iter->data->form) continue;
				if (auto tag = g_Categories[GetCategoryForItem(iter->data)]; !tag.category._Equal(entry)) continue;
				if (g_Categories[entry].count == 0) {
					keys = 1;
					break;
				}
				else if (g_Categories[entry].count == 1)
					keys += 1;
				else if (g_Categories[entry].count == 2)
					keys += iter->data->countDelta;
			}
			if (keys) {
				std::string keyringname = g_Categories[entry].name;
				if (keyringname.find("&-") == 0)
					keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
				if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";
				const auto tile = InventoryMenu::GetSingleton()->itemsList.Insert(nullptr, keyringname.c_str(), nullptr, nullptr);
				tile->SetFloat(kTileValue_id, 30);
				tile->SetString(kTileValue_user16, entry.c_str());
				InjectIconTile(g_Categories[entry], &InventoryMenu::GetSingleton()->itemsList, tile, nullptr);
			}
		}
	}



	std::vector<Tile*> tablineTiles;

	void SetUpTabline(TileRect* tabline, int traitID, const char* strWeapon, const char* strApparel, const char* strAid,
		const char* strMisc, const char* strAmmo, char* zero)
	{
		UInt32 listIndex = 0;
		Float32 width = 0;
		const auto menu = tabline->GetParentMenu();

		tabline->SetFloat(kTileValue_clips, true);
		int i = 0;
		for (const auto& tab : g_Tabline)
		{
			const auto tile = menu->AddTileFromTemplate(tabline, "TabButtonTemplate", 0);

			auto string = g_Tabs[tab].name;
			Log(tab);
			Log(string);
			if (string.find("&-") == 0)
				string = GetStringFromGameSettingFromString(string.substr(2, string.length() - 3));
			Log(string);
			tile->SetString(kTileValue_string, string.c_str(), true);
			tile->SetFloat(kTileValue_listindex, listIndex);
			tile->SetFloat(kTileValue_id, traitID); // + listIndex
			tile->SetFloat(kTileValue_clips, true);
			CdeclCall<void>(0xA04200, 0);
			if (i < 5) width += trunc(tile->GetFloat(kTileValue_width));
			listIndex++;
			tablineTiles.push_back(tile);
			i++;
		}

		const auto tablineWidth = tabline->GetFloat(kTileValue_width);
		const auto leftLineLength = trunc((tablineWidth - width) / (listIndex + 1));
		tabline->SetFloat(Tile::TraitNameToID("_LeftLineLength"), leftLineLength);
		tabline->SetFloat(Tile::TraitNameToID("_ButtonCount"), tablineWidth);

		auto _x = leftLineLength;
		for (const auto tile : tablineTiles)
		{
			tile->SetFloat(Tile::TraitNameToID("_x"), _x);
			_x += leftLineLength + trunc(tile->GetFloat(kTileValue_width));
			i++;
		}

		//	CdeclCall(0x707BE0, tabline, traitID, "<<", strWeapon, strApparel, strAid, ">>", zero);
	}

	UInt32 __fastcall InventoryMenuHandleClickGetFilter(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile)
	{
		return clickedTile->GetFloat(kTileValue_listindex);
	}

	void __fastcall InventoryMenuSetupData(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile)
	{
		const auto listIndex = clickedTile->GetFloat(kTileValue_listindex);

		if (listIndex == 0) {
			menu->tileModButton->SetFloat(kTileValue_visible, true);
		}
		else if (listIndex == 1) {
			ThisCall(0x718630, &InterfaceManager::GetSingleton()->help, InterfaceManager::kHelpApparel, InventoryMenu::GetMenuID(), 512);
			menu->tileModButton->SetFloat(kTileValue_visible, false);
			menu->tileItemIconBadge->SetFloat(kTileValue_visible, false);
			menu->tileStrReq->SetFloat(kTileValue_visible, false);
		}
		else if (listIndex == 4) {
			ThisCall(0x718630, &InterfaceManager::GetSingleton()->help, InterfaceManager::kHelpAmmo, InventoryMenu::GetMenuID(), 500);
			menu->itemsList.ForEach(reinterpret_cast<ListBox<ContChangesEntry>::ForEachFunc>(0x782850), 0, 0x7FFFFFFF);
		}
		else {
			menu->tileModButton->SetFloat(kTileValue_visible, false);
			menu->tileItemIconBadge->SetFloat(kTileValue_visible, false);
			menu->tileStrReq->SetFloat(kTileValue_visible, false);
		}
	}

	Tile* __fastcall InventoryMenuChooseTab(SInt32 key, UInt32 filter)
	{
		SInt32 newFilter = filter;
		if (key == kMenu_RightArrow) newFilter++;
		if (key == kMenu_LeftArrow) newFilter--;
		if (newFilter < 0) newFilter = tablineTiles.size() - 1;
		if (newFilter > tablineTiles.size() - 1) newFilter = 0;
		return tablineTiles[newFilter];
	}

	std::unordered_map<UInt32, InventoryMenu::ScrollPos> scrollPosTab;
	std::unordered_map<std::string, InventoryMenu::ScrollPos> scrollPosKeyring;

	void InventoryMenuSaveScrollPosition()
	{
		const auto menu = InventoryMenu::GetSingleton();
		SInt32 listIndex;
		if (const auto item = menu->itemsList.GetSelectedTile())
			listIndex = trunc(item->GetFloat(kTileValue_listindex));
		else
			listIndex = -1;
		const SInt32 currentValue = menu->itemsList.scrollBar->GetFloat("_current_value");

		if (InventoryMenu::IsKeyringOpen())
		{
			scrollPosKeyring[openCategory].listIndex = listIndex;
			scrollPosKeyring[openCategory].currentValue = currentValue;
		}
		else
		{
			scrollPosTab[menu->filter].listIndex = listIndex;
			scrollPosTab[menu->filter].currentValue = currentValue;
		}

	}

	void InventoryMenuRestoreScrollPosition()
	{
		const auto menu = InventoryMenu::GetSingleton();
		SInt32 listIndex = 0;
		SInt32 currentValue = 0;
		if (InventoryMenu::IsKeyringOpen())
		{
			if (scrollPosKeyring.contains(openCategory))
			{
				listIndex = scrollPosKeyring[openCategory].listIndex;
				currentValue = scrollPosKeyring[openCategory].currentValue;
			}
		}
		else
		{
			if (const auto filter = menu->filter; scrollPosTab.contains(filter))
			{
				listIndex = scrollPosTab[filter].listIndex;
				currentValue = scrollPosTab[filter].currentValue;
			}
		}
		menu->itemsList.RestoreScrollPositionProxy(listIndex, currentValue);
	}

	UInt8 __fastcall InventoryMenuShouldHideItem(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return true;

		const auto filter = InventoryMenu::GetSingleton()->filter;
		if (!IsFilterForItem(entry)) SIFiles::AssignFiltersToItem(entry->form);
		//	if (SI::g_Tabs[SI::g_Tabline[filter]].tabNew) return false;

		if (g_ItemToFilter[entry->form].contains(g_Tabline[filter])) return false;

		return true;
	}

}

namespace SIHooks
{
	__declspec(naked) void SortingBarterContainerMenuHook()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(SI::CompareItemsWithTags);
		_asm
		{
			pop		eax
			mov		ecx, [ebp + 0x8]	// a1
			mov		edx, [ebp + 0xC]	// a2

			call	CompareItems
			mov		esp, ebp
			pop		ebp
			ret
		}
	}
	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, signed int a1)
	{
		tile->SetFloat(tilevalue, InventoryMenu::GetSingleton()->IsKeyringOpen(), true);
	}

	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* string, int propagate)
	{
		std::string stringnew = string;
		if (const auto clickedtile = InventoryMenu::GetSingleton()->itemsList.selected; clickedtile->GetValue(kTileValue_user16)
			&& !SI::g_Categories[clickedtile->GetValue(kTileValue_user16)->str].icon.empty())
			stringnew = SI::g_Categories[clickedtile->GetValue(kTileValue_user16)->str].icon;
		tile->SetString(tilevalue, stringnew.c_str(), true);
	}
}
