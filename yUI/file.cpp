#include "file.h"

#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <utility>


#include "functions.h"
#include "utility.h"
#include "json.h"
#include "Utilities.h"

extern std::vector<JSONEntryItem> g_SI_Items_JSON;
extern std::vector<JSONEntryTag> g_SI_Tags_JSON;
extern std::unordered_map <TESForm*, std::string> g_SI_Items;
extern std::unordered_map <std::string, JSONEntryTag> g_SI_Tags;
extern std::unordered_map <std::string, bool> g_SI_Categories;

extern std::vector<std::filesystem::path> g_XMLPaths;

void LogForm(const TESForm* form)
{
	DebugLog(FormatString("Detected in-game form %X %s", form->refID, form->GetName()));
	//, form->GetFullName() ? form->GetFullName()->name.CStr() : "<no name>")
}

void JSONEntryItemRecursiveEmplace(const std::string&, SInt16, TESForm*, UInt8);

void HandleJson(const std::filesystem::path& path)
{
	Log("\nReading from JSON file " + path.string());
	const auto strToFormID = [](const std::string& formIdStr)
	{
		const auto formId = HexStringToInt(formIdStr);
		if (formId == -1)
		{
			DebugPrint("Form field was incorrectly formatted, got " + formIdStr);
		}
		return formId;
	};
	try
	{
		std::ifstream i(path);
		nlohmann::json j;
		i >> j;
		if (j.contains("tags") && j["tags"].is_array())
		{
			for (auto& elem : j["tags"])
			{
				if (!elem.is_object())
				{
					DebugPrint("JSON error: expected object with mod, form and folder fields");
					continue;
				}

				const auto& tag = elem["tag"].get<std::string>();
				
				SInt16 priority = 0;
				if (elem.contains("priority")) priority = elem["priority"].get<SInt16>();

				UInt32 formType = 0;
				if (elem.contains("formType")) formType = elem["formType"].get<UInt32>();

				UInt8 questItem = 0;
				if (elem.contains("questItem")) questItem = elem["questItem"].get<UInt8>();

				
				if (elem.contains("mod") && elem.contains("form"))
				{
					auto modName = elem.contains("mod") ? elem["mod"].get<std::string>() : "";
					const auto* mod = !modName.empty() ? DataHandler::GetSingleton()->LookupModByName(modName.c_str()) : nullptr;
					if (!mod && !modName.empty())
					{
						DebugPrint("Mod name " + modName + " was not found");
						continue;
					}

					UInt32 formlist = 0;
					if (elem.contains("formlist")) formlist = elem["formlist"].get<UInt32>();
					
					std::vector<int> formIds;
					if (auto * formElem = elem.contains("form") ? &elem["form"] : nullptr; formElem)
					{
						if (formElem->is_array())
							std::ranges::transform(*formElem, std::back_inserter(formIds), [&](auto& i) {return strToFormID(i.template get<std::string>()); });
						else
							formIds.push_back(strToFormID(formElem->get<std::string>()));
						if (std::ranges::find(formIds, -1) != formIds.end())
							continue;
					}

					if (mod && !formIds.empty())
					{
						for (auto formId : formIds)
						{
							formId = (mod->modIndex << 24) + (formId & 0x00FFFFFF);
							auto* form = LookupFormByID(formId);
							if (!form) { DebugPrint(FormatString("Form %X was not found", formId)); continue; }
							if (!formlist) {
								DebugLog(FormatString("Tag: '%10s', individual form: %8X (%s)", tag.c_str(), formId, form->GetName()));
								g_SI_Items_JSON.emplace_back(tag, priority, form, questItem);
							} else if (formlist == 1) {
								JSONEntryItemRecursiveEmplace(tag, priority, form, questItem);
							} else if (formlist == 2) {
								for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter) {
									TESForm* item = mIter.Get();
									if (item->typeID != 40) continue;
									const auto weapon = DYNAMIC_CAST(item, TESForm, TESObjectWEAP);
									if (!weapon) continue;
									if (!weapon->repairItemList.listForm) continue;
									if (IsInListRecursive(weapon->repairItemList.listForm, form)) {
										DebugLog(FormatString("Tag: '%10s', recursive form: %8X (%s), repair list: '%8X'", tag.c_str(), formId, item->GetName(), weapon->repairItemList.listForm->refID));
										g_SI_Items_JSON.emplace_back(tag, priority, item, questItem);
									}
								}
							}
						}
					}
					else {
						DebugLog(FormatString("Tag: '%10s', mod: '%s'", tag.c_str(), modName.c_str()));
						g_SI_Items_JSON.emplace_back(tag, priority, nullptr, questItem);
					}
				}
				else if (formType == 40 || elem.contains("weaponSkill") || elem.contains("weaponType"))
				{
					formType = 40;
					JSONEntryItemWeapon weapon{};

					if (elem.contains("weaponSkill"))			weapon.weaponSkill = elem["weaponSkill"].get<UInt32>();
					if (elem.contains("weaponHandgrip"))		weapon.weaponHandgrip = elem["weaponHandgrip"].get<UInt32>();
					if (elem.contains("weaponAttackAnim"))		weapon.weaponAttackAnim = elem["weaponAttackAnim"].get<UInt32>();
					if (elem.contains("weaponReloadAnim"))		weapon.weaponReloadAnim = elem["weaponReloadAnim"].get<UInt32>();
					if (elem.contains("weaponIsAutomatic"))		weapon.weaponIsAutomatic = elem["weaponIsAutomatic"].get<UInt32>();
					if (elem.contains("weaponHasScope"))		weapon.weaponHasScope = elem["weaponHasScope"].get<UInt32>();
					if (elem.contains("weaponIgnoresDTDR"))		weapon.weaponIgnoresDTDR = elem["weaponIgnoresDTDR"].get<UInt32>();
					if (elem.contains("weaponClipRounds"))		weapon.weaponClipRounds = elem["weaponClipRounds"].get<UInt32>();
					if (elem.contains("weaponNumProjectiles"))	weapon.weaponNumProjectiles = elem["weaponNumProjectiles"].get<UInt32>();
					if (elem.contains("weaponSoundLevel"))		weapon.weaponSoundLevel = elem["weaponSoundLevel"].get<UInt32>();

					if (elem.contains("ammoMod") && elem.contains("ammoForm"))
					{
						auto ammoModName = elem["ammoMod"].get<std::string>();
						const auto* ammoMod = !ammoModName.empty() ? DataHandler::GetSingleton()->LookupModByName(ammoModName.c_str()) : nullptr;
						auto* ammoForm = LookupFormByID((ammoMod->modIndex << 24) + (strToFormID(elem["ammoForm"].get<std::string>()) & 0x00FFFFFF));
						weapon.ammo = ammoForm;
					}
					
					std::vector<int> weaponTypes;
					if (auto * weaponTypeElem = elem.contains("weaponType") ? &elem["weaponType"] : nullptr; weaponTypeElem)
					{
						if (weaponTypeElem->is_array())
							std::ranges::transform(*weaponTypeElem, std::back_inserter(weaponTypes), [&](auto& i) {return i.template get<UInt32>(); });
						else
							weaponTypes.push_back(weaponTypeElem->get<UInt32>());
						if (std::ranges::find(weaponTypes, -1) != weaponTypes.end())
							continue;
					}

					if (!weaponTypes.empty())
					{
						for (auto weaponType : weaponTypes)
						{
							weapon.weaponType = weaponType;
							DebugLog(FormatString("Tag: '%10s', weapon condition, type: %d", tag.c_str(), weaponType));
							g_SI_Items_JSON.emplace_back(tag, priority, formType, questItem, weapon);
						}
					} else {
						DebugLog(FormatString("Tag: '%10s', weapon condition", tag.c_str()));
						g_SI_Items_JSON.emplace_back(tag, priority, formType, questItem, weapon);
					}
				}
				else if (formType == 24)
				{
					formType = 24;
					JSONEntryItemArmor armor{};

					if (elem.contains("armorHead")) 		(elem["armorHead"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1;
					if (elem.contains("armorHair")) 		(elem["armorHair"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 1;
					if (elem.contains("armorUpperBody")) 	(elem["armorUpperBody"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 2;
					if (elem.contains("armorLeftHand")) 	(elem["armorLeftHand"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 3;
					if (elem.contains("armorRightHand")) 	(elem["armorRightHand"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 4;
					if (elem.contains("armorWeapon")) 		(elem["armorWeapon"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 5;
					if (elem.contains("armorPipBoy")) 		(elem["armorPipBoy"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 6;
					if (elem.contains("armorBackpack")) 	(elem["armorBackpack"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 7;
					if (elem.contains("armorNecklace")) 	(elem["armorNecklace"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 8;
					if (elem.contains("armorHeadband")) 	(elem["armorHeadband"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 9;
					if (elem.contains("armorHat")) 			(elem["armorHat"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 10;
					if (elem.contains("armorEyeglasses")) 	(elem["armorEyeglasses"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 11;
					if (elem.contains("armorNosering")) 	(elem["armorNosering"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 12;
					if (elem.contains("armorEarrings")) 	(elem["armorEarrings"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 13;
					if (elem.contains("armorMask")) 		(elem["armorMask"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 14;
					if (elem.contains("armorChoker")) 		(elem["armorChoker"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 15;
					if (elem.contains("armorMouthObject"))	(elem["armorMouthObject"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 16;
					if (elem.contains("armorBodyAddon1"))	(elem["armorBodyAddon1"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 17;
					if (elem.contains("armorBodyAddon2"))	(elem["armorBodyAddon2"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 18;
					if (elem.contains("armorBodyAddon3"))	(elem["armorBodyAddon3"].get<SInt8>() == 1 ? armor.armorSlotsMaskWL : armor.armorSlotsMaskBL) += 1 << 19;


					if (elem.contains("armorClass"))		armor.armorClass = elem["armorClass"].get<UInt16>();
					if (elem.contains("armorPower"))		armor.armorPower = elem["armorPower"].get<SInt8>();
					if (elem.contains("armorHasBackpack"))	armor.armorHasBackpack = elem["armorHasBackpack"].get<SInt8>();

					if (elem.contains("armorDT"))			armor.armorDT = elem["armorDT"].get<float>();
					if (elem.contains("armorDR"))			armor.armorDR = elem["armorDR"].get<UInt16>();
					if (elem.contains("armorChangesAV"))	armor.armorChangesAV = elem["armorChangesAV"].get<UInt16>();

					g_SI_Items_JSON.emplace_back(tag, priority, formType, questItem, armor);
				}
				else if (formType == 31 || elem.contains("miscComponent"))
				{
					formType = 31;
					JSONEntryItemMisc misc{};
					if (elem.contains("miscComponent"))	misc.miscComponent = elem["miscComponent"].get<UInt8>();
					g_SI_Items_JSON.emplace_back(tag, priority, formType, questItem, misc);
				}
				else if (formType == 47 || elem.contains("IsFood") || elem.contains("IsMedicine"))
				{
					formType = 47;
					JSONEntryItemAid aid{};

					if (elem.contains("aidRestoresAV"))			aid.aidRestoresAV = elem["aidRestoresAV"].get<UInt8>();
					if (elem.contains("aidDamagesAV"))			aid.aidDamagesAV = elem["aidDamagesAV"].get<UInt8>();
					if (elem.contains("aidIsAddictive"))		aid.aidIsAddictive = elem["aidIsAddictive"].get<UInt8>();
					if (elem.contains("aidIsFood"))				aid.aidIsFood = elem["aidIsFood"].get<UInt8>();
					if (elem.contains("aidIsWater"))			aid.aidIsWater = elem["aidIsWater"].get<UInt8>();
					if (elem.contains("aidIsMedicine"))			aid.aidIsMedicine = elem["aidIsMedicine"].get<UInt8>();
					if (elem.contains("aidIsPoisonous"))		aid.aidIsPoisonous = elem["aidIsPoisonous"].get<UInt8>();
					g_SI_Items_JSON.emplace_back(tag, priority, formType, questItem, aid);
				}
				else g_SI_Items_JSON.emplace_back(tag, priority, formType, questItem);
			}
		}
		else { DebugPrint(path.string() + " JSON tag array not detected"); }

		if (j.contains("icons") && j["icons"].is_array())
		{
			for (auto& elem : j["icons"])
			{
			if (!elem.is_object())
				{
					DebugPrint("JSON error: expected object with mod, form and folder fields");
					continue;
				}

				const auto& tag = elem["tag"].get<std::string>();

				SInt16 priority = 0;
				if (elem.contains("priority")) priority = elem["priority"].get<SInt16>();
				
				std::string xmltemplate;
				if (elem.contains("template")) xmltemplate = elem["template"].get<std::string>();
				std::string filename;
				if (elem.contains("filename")) filename = elem["filename"].get<std::string>();
				std::string texatlas;
				if (elem.contains("texatlas")) texatlas = elem["texatlas"].get<std::string>();				
				SInt64 systemcolor = 0;
				if (elem.contains("systemcolor")) systemcolor = elem["systemcolor"].get<SInt32>();
				std::string category;
				if (elem.contains("category")) category = elem["category"].get<std::string>();
				std::string name;
				if (elem.contains("name")) name = elem["name"].get<std::string>();
				std::string nameGMST;
				if (elem.contains("nameGMST")) nameGMST = elem["nameGMST"].get<std::string>();

				DebugLog(FormatString("Tag: '%10s', icon: '%s'",  tag.c_str(), filename.c_str()));
				g_SI_Tags_JSON.emplace_back(tag, priority, xmltemplate, filename, texatlas, systemcolor, category, name, nameGMST);
			}
		}
		else { DebugPrint(path.string() + " JSON icon array not detected"); }
	}
	catch (nlohmann::json::exception& e)
	{
		DebugPrint("The JSON is incorrectly formatted! It will not be applied.");
		DebugPrint(FormatString("JSON error: %s\n", e.what()));
	}

}

void JSONEntryItemRecursiveEmplace(const std::string& tag, SInt16 priority, TESForm* list, UInt8 questItem)
{
	if (list->typeID == 85)
	{
		const auto bgslist = DYNAMIC_CAST(list, TESForm, BGSListForm);
		if (!bgslist) return;
		for (auto iter = bgslist->list.Begin(); !iter.End(); ++iter)
			if (iter.Get()) { JSONEntryItemRecursiveEmplace(tag, priority, iter.Get(), questItem); }
	}
	else {
		DebugLog(FormatString("Tag: '%10s', recursive form: %8X (%s)", tag.c_str(), list->refID, list->GetName()));
		g_SI_Items_JSON.emplace_back(tag, priority, list, questItem);
	}
}

extern NiTPointerMap<TESForm>** g_allFormsMap;

void FillSIMapsFromJSON()
{
	ra::sort(g_SI_Items_JSON, [&](const JSONEntryItem& entry1, const JSONEntryItem& entry2)
		{ return entry1.priority > entry2.priority; });

	for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter) {
		TESForm* form = mIter.Get();
		switch (form->typeID)
		{
		default: continue;
		case 24: case 25: case 26: case 29: case 30: case 31: case 40: case 41: case 46: case 47: case 49: case 96:
		case 103: case 108: case 115: case 116: case 117: {}
		}
		for (const auto& entry : g_SI_Items_JSON) {			
			if (entry.form && entry.form->refID != form->refID) continue;
			if (entry.formType && entry.formType != form->typeID) continue;

			if (entry.questItem && entry.questItem != static_cast<UInt8>(form->IsQuestItem2())) continue;

			if (entry.formType == 40) {
				const auto weapon = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
				if (!weapon) continue;
				if (entry.formWeapon.weaponSkill && entry.formWeapon.weaponSkill != weapon->weaponSkill) continue;
				if (entry.formWeapon.weaponType && entry.formWeapon.weaponType != weapon->eWeaponType) continue;
				if (entry.formWeapon.weaponHandgrip && entry.formWeapon.weaponHandgrip != weapon->HandGrip()) continue;
				if (entry.formWeapon.weaponAttackAnim && entry.formWeapon.weaponAttackAnim != weapon->AttackAnimation()) continue;
				if (entry.formWeapon.weaponReloadAnim && entry.formWeapon.weaponReloadAnim != weapon->reloadAnim) continue;
				if (entry.formWeapon.weaponType && entry.formWeapon.weaponType != weapon->eWeaponType) continue;
				if (entry.formWeapon.weaponIsAutomatic && entry.formWeapon.weaponIsAutomatic != static_cast<UInt32>(weapon->IsAutomatic())) continue;
				if (entry.formWeapon.weaponHasScope && entry.formWeapon.weaponHasScope != static_cast<UInt32>(weapon->HasScopeAlt())) continue;
				if (entry.formWeapon.weaponIgnoresDTDR && entry.formWeapon.weaponIgnoresDTDR != static_cast<UInt32>(weapon->IgnoresDTDR())) continue;
				if (entry.formWeapon.weaponClipRounds && entry.formWeapon.weaponClipRounds > static_cast<UInt32>(weapon->GetClipRounds(false))) continue;
				if (entry.formWeapon.weaponNumProjectiles && entry.formWeapon.weaponNumProjectiles > weapon->numProjectiles) continue;
				if (entry.formWeapon.weaponSoundLevel && entry.formWeapon.weaponSoundLevel != weapon->soundLevel) continue;
				if (entry.formWeapon.ammo && !IsInListRecursive(entry.formWeapon.ammo, weapon->ammo.ammo)) continue;
			} else if (entry.formType == 24) {
				const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
				if (!armor) continue;
				if (entry.formArmor.armorSlotsMaskWL && (entry.formArmor.armorSlotsMaskWL & armor->GetArmorValue(6)) != entry.formArmor.armorSlotsMaskWL) continue;
				if (entry.formArmor.armorSlotsMaskWL && (entry.formArmor.armorSlotsMaskBL & armor->GetArmorValue(6)) != 0) continue;
				if (entry.formArmor.armorClass && entry.formArmor.armorClass != armor->GetArmorValue(1)) continue;
				if (entry.formArmor.armorPower && entry.formArmor.armorPower != armor->GetArmorValue(2)) continue;
				if (entry.formArmor.armorHasBackpack && entry.formArmor.armorHasBackpack != armor->GetArmorValue(3)) continue;

				if (entry.formArmor.armorDT && entry.formArmor.armorDT > armor->damageThreshold) continue;
				if (entry.formArmor.armorDR && entry.formArmor.armorDR > armor->armorRating) continue;
//				if (entry.formArmor.armorChangesAV && entry.formArmor.armorChangesAV > armor->armorRating) continue;
			} else if (entry.formType == 31) {
				if (entry.formMisc.miscComponent && !IsCraftingComponent(form)) continue;
			} else if (entry.formType == 47) {
				const auto aid = DYNAMIC_CAST(form, TESForm, AlchemyItem);
				if (!aid) continue;
				if (entry.formAid.aidRestoresAV && !aid->HasBaseEffectRestoresAV(entry.formAid.aidRestoresAV)) continue;
				if (entry.formAid.aidDamagesAV && !aid->HasBaseEffectDamagesAV(entry.formAid.aidDamagesAV)) continue;
				if (entry.formAid.aidIsAddictive && !aid->IsAddictive()) continue;
				if (entry.formAid.aidIsFood && !aid->IsFood()) continue;
				if (entry.formAid.aidIsWater && !aid->IsWaterAlt()) continue;
				if (entry.formAid.aidIsPoisonous && !aid->IsPoison()) continue;
				if (entry.formAid.aidIsMedicine && !aid->IsMedicine()) continue;
			}
			
			g_SI_Items.emplace(form, entry.tag);
		}
	}
	g_SI_Items_JSON = std::vector<JSONEntryItem>();

	ra::sort(g_SI_Tags_JSON, [&](const JSONEntryTag& entry1, const JSONEntryTag& entry2)
		{ return entry1.priority > entry2.priority; });
	for (auto& entry : g_SI_Tags_JSON) {
		if (!entry.category.empty()) g_SI_Categories.emplace(entry.category, false);
		g_SI_Tags.emplace(entry.tag, std::move(entry));
	}
	g_SI_Tags_JSON = std::vector<JSONEntryTag>();

	/*	for (const auto& entry : g_SI_Items_JSON)
 {
		g_jsonContext.script = entry.conditionScript;
		g_jsonContext.pollCondition = entry.pollCondition;
		if (entry.form)
			Log(FormatString("JSON: Loading animations for form %X in path %s", entry.form->refID, entry.folderName.c_str()));
		else
			Log("JSON: Loading animations for global override in path " + entry.folderName);
		const auto path = GetCurPath() + R"(\Data\menus\ySI\)" + entry.folderName;
		if (!entry.form) // global
			LoadPathsForPOV(path, nullptr);
		else if (!LoadForForm(path, entry.form))
			Log(FormatString("Loaded from JSON folder %s to form %X", path.c_str(), entry.form->refID));
		g_jsonContext.Reset();
	}*/
}

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
			{
				Log(iter->path().string() + " found");

				const auto* mod = DataHandler::GetSingleton()->LookupModByName(fileName.string().c_str());

				if (mod) {}
//					LoadModAnimPaths(path, mod);
				else if (_stricmp(fileName.extension().string().c_str(), ".esp") == 0 || _stricmp(fileName.extension().string().c_str(), ".esm") == 0)
					DebugPrint(FormatString("Mod with name %s is not loaded!", fileName.string().c_str()));
				else if (_stricmp(fileName.string().c_str(), "_male") != 0 && _stricmp(fileName.string().c_str(), "_1stperson") != 0)
				{
					Log("Found anim folder " + fileName.string() + " which can be used in JSON");
				}
			}
			else if (_stricmp(path.extension().string().c_str(), ".json") == 0)
			{
				HandleJson(iter->path());
			}
			else if (_stricmp(path.extension().string().c_str(), ".xml") == 0)
			{
				auto pathstring = iter->path().generic_string();
				auto relativepath = pathstring.substr(pathstring.find_last_of("\\Data\\") - 3);
				g_XMLPaths.emplace_back(std::filesystem::path(relativepath));
			}
		}
	}
	else
	{
		Log(dir + " does not exist.");
	}
	FillSIMapsFromJSON();
	const auto now = std::chrono::system_clock::now();
	const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - then);
	DebugPrint(FormatString("Loaded tags and icons in %d ms", diff.count()));
}