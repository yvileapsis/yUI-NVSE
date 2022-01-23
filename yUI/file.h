#pragma once
#include <unordered_map>
#include <utility>

#include "GameData.h"
#include "GameAPI.h"
#include "GameRTTI.h"

struct JSONEntryItemWeapon
{
	UInt32 weaponSkill;
	UInt32 weaponType;
	UInt32 weaponHandgrip;
	UInt32 weaponAttackAnim;
	UInt32 weaponReloadAnim;
	UInt32 weaponIsAutomatic;
	UInt32 weaponHasScope;
	UInt32 weaponIgnoresDTDR;
	UInt32 weaponClipRounds;
	UInt32 weaponNumProjectiles;
	UInt32 weaponSoundLevel;

	TESForm* ammo;
	
	JSONEntryItemWeapon()
		: weaponSkill(0), weaponType(0), weaponHandgrip(0), weaponAttackAnim(0), weaponReloadAnim(0), weaponIsAutomatic(0),
		weaponHasScope(0), weaponIgnoresDTDR(0), weaponClipRounds(0), weaponNumProjectiles(0), weaponSoundLevel(0), ammo(nullptr)
	{}

};

struct JSONEntryItemArmor
{
	UInt32 armorSlotsMaskWL;
	UInt32 armorSlotsMaskBL;

	UInt16 armorClass;
	SInt8 armorPower;
	SInt8 armorHasBackpack;
	
	float armorDT;
	UInt16 armorDR;
	UInt16 armorChangesAV;
	
	JSONEntryItemArmor() = default;
};

struct JSONEntryItemAid
{
	UInt8 aidRestoresAV;
	UInt8 aidDamagesAV;
	UInt8 aidIsAddictive;
	UInt8 aidIsWater;
	UInt8 aidIsFood;
	UInt8 aidIsMedicine;
	UInt8 aidIsPoisonous;
	JSONEntryItemAid() = default;
};


struct JSONEntryItemMisc
{
	UInt8 miscComponent;
	JSONEntryItemMisc() = default;
};


class JSONEntryItem
{
public:
	std::string tag;
	SInt16 priority;

	TESForm* form;

	UInt32 formType;

	UInt8 questItem;
	
	JSONEntryItemWeapon formWeapon;
	JSONEntryItemArmor formArmor;
	JSONEntryItemAid formAid;
	JSONEntryItemMisc formMisc;
//	Script* conditionScript;
//	bool pollCondition;


//	JSONEntryItem(std::string tag, SInt16 priority, TESForm* form, UInt8 type, BGSListForm* formlist, Script* script, bool pollCondition)
//		: tag(std::move(tag)), priority(priority), form(form), type(type), formlist(formlist), conditionScript(script), pollCondition(pollCondition)
//	{}

	JSONEntryItem(std::string tag, SInt16 priority, TESForm* form, UInt8 questItem) : tag(std::move(tag)), priority(priority), form(form), formType(0), questItem(questItem)
	{}

	JSONEntryItem(std::string tag, SInt16 priority, UInt32 formType, UInt8 questItem, JSONEntryItemWeapon weapon) : tag(std::move(tag)), priority(priority), formType(formType), formWeapon(weapon), form(nullptr), questItem(questItem)
	{}

	JSONEntryItem(std::string tag, SInt16 priority, UInt32 formType, UInt8 questItem) : tag(std::move(tag)), priority(priority), form(nullptr), formType(formType), questItem(questItem)
	{}

	JSONEntryItem(std::string tag, SInt16 priority, UInt32 formType, UInt8 questItem, JSONEntryItemMisc misc) : tag(std::move(tag)), priority(priority), form(nullptr), formType(formType), questItem(questItem), formMisc(misc)
	{}

	JSONEntryItem(std::string tag, SInt16 priority, UInt32 formType, UInt8 questItem, JSONEntryItemArmor armor) : tag(std::move(tag)), priority(priority), form(nullptr), formType(formType), questItem(questItem), formArmor(armor)
	{}

	JSONEntryItem(std::string tag, SInt16 priority, UInt32 formType, UInt8 questItem, JSONEntryItemAid aid) : tag(std::move(tag)), priority(priority), form(nullptr), formType(formType), questItem(questItem), formAid(aid)
	{}
};

class JSONEntryTag
{
public:
	std::string tag;
	SInt16 priority;

	std::string xmltemplate;
	std::string filename;
	std::string texatlas;
	SInt32 systemcolor;
	
	std::string category;
	std::string name;
	std::string icon;
	UInt32 tab;
	UInt32 count;
	
	JSONEntryTag(std::string tag, SInt16 priority, std::string xmltemplate, std::string filename, std::string texatlas,
	             SInt64 systemcolor, std::string category, std::string name, std::string icon, UInt32 tab, UInt32 count)
		: tag(std::move(tag)), priority(priority), xmltemplate(std::move(xmltemplate)), filename(std::move(filename)),
		  texatlas(std::move(texatlas)), systemcolor(systemcolor), category(std::move(category)), name(std::move(name)), icon(std::move(icon)), tab(tab), count(count)
	{
	}
	JSONEntryTag() = default;
};


void LoadSIMapsFromFiles();