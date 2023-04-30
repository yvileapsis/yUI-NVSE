#include "main.h"
#include "Menu.h"
#include "PluginAPI.h"
#include <string>

#include "ConfigurationMenu.h"
#include "GameData.h"

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS);
bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS);

UInt8 g_activeMod		= 0;
UInt8 g_activeSetting	= 0;
bool g_Reset			= false;
bool g_NewValue			= false;
bool g_ShowScale		= false;
bool g_DefaultScale		= false;

class MCMMod
{
public:
	std::string id;
	std::string title;
	UInt8 modID = 0;
	UInt8 columns = 1;
};

class MCMItem
{
public:
	bool enable = false;
	std::string prefix;
	std::string suffix;

	std::string title;

	enum eType
	{
		kNone = 0,
		kChoice = 1,
		kSlider = 2,
		kControl = 3,
		kOnOff = 4,
		kTick = 5,
		kOnOffCustom = 6,
		kString = 7,
		kThreeSliders = 8,
		kRGB = 9,
	};

	Float64 value = 0;
	std::string valueAlt;

	UInt32 type = kNone;

	UInt32 altFont = 0;
	UInt32 brightness = 0;
	UInt32 hightlight = 0;
	UInt32 indent = 0;
	UInt32 rgb = 0;
	UInt32 rbgalt = 0;

	std::string textOFF;
	std::string textON;

};

std::unordered_map<UInt8, MCMMod> mods;
std::unordered_map<UInt8, std::unordered_map<UInt8, MCMItem>> items;

std::string MCMPath(UInt32 child, UInt32 grandchild, std::string src)
{

	std::string path = "StartMenu/MCM/";
	if (child == 0)
	{
	}
	else if (child == 1) {
		path += "MCM_Options/";
		if (grandchild != 0) {
			path += "Option" + std::to_string(grandchild) + "/";
		}
	}
	else if (child == 2) {
		path += "MCM_Scale/";
	}
	else if (child == 3) {
		path += "MCM_List/";
		if (grandchild != 0) {
			path += "Item" + std::to_string(grandchild) + "/";
		}
	}
	else if (child == 4) {
		path += "MCM_Trigger/";
	}
	else if (child == 5) {
		path += "MCM_ModList/";
		if (grandchild == 0) {}
		else if (grandchild <= 23) {
			path += "Mod" + std::to_string(grandchild) + "/";
		}
		else {
			path += "SubMenu" + std::to_string(grandchild - 23) + "/";
		}
	}
	else if (child == 9) {
		path += "MCM_Info/";
	}
	else if (child == 15) {
		path += "MCM_Input/";
	}
	else if (child == 17) {
		path += "MCM_Images/";
	}
	path += std::string(src);
	return path;
}

Float64 GetMCMInternal(UInt32 child, UInt32 grandchild, std::string src)
{
	if (child == 0 && grandchild == 0)
	{
		if (src == "_ActiveMod")	return g_activeMod;
		if (src == "_Reset")		return g_Reset;
	}

	return 0;
}


void SetMCMInternal(UInt32 child, UInt32 grandchild, std::string src, Float64 val)
{
	if (child == 0 && grandchild == 0)
	{
//		if (src == "_ActiveMod")	return g_activeMod;
		if (src == "_Reset")		g_Reset = val;
	}

	if (child == 1 && grandchild == 0)
	{
		if (src == "_columns")			mods[g_activeMod].columns = val;
	}

	if (child == 1 && grandchild >= 1)
	{
		g_activeSetting = grandchild - 1;
		if (src == "_Enable")			items[g_activeMod][g_activeSetting].enable = val;
		if (src == "_Type")				items[g_activeMod][g_activeSetting].type = std::trunc(val);
		if (src == "_Value")			items[g_activeMod][g_activeSetting].value = val;
	}
}

void SetMCMInternalString(UInt32 child, UInt32 grandchild, std::string src, std::string val)
{
	if (child == 1 && grandchild >= 1)
	{
		g_activeSetting = grandchild - 1;
		if (src == "_Title")			items[g_activeMod][g_activeSetting].title = val;
		if (src == "value/*:1/string")	items[g_activeMod][g_activeSetting].valueAlt = val;
		if (src == "_textOn")			items[g_activeMod][g_activeSetting].textON = val;
		if (src == "_textOff")			items[g_activeMod][g_activeSetting].textOFF = val;
	}
}

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;
//	std::string path = MCMPath(child, grandchild, src);
//	if (const auto val = StringToTilePath(path)) *result = val->num;

	*result = GetMCMInternal(child, grandchild, src);

	return true;
}

bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	float value = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src, &value)) return true;

	SetMCMInternal(child, grandchild, src, value);

	/*
	std::string path = MCMPath(child, grandchild, src);
	if (const auto val = StringToTilePath(path)) {
		*result = 1;
		val->parent->Set(val->id, value);
	}*/

	return true;
}

bool Cmd_GetyCMString_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;

	/*
	std::string path = MCMPath(child, grandchild, src);
	if (const auto val = StringToTilePath(path)) AssignString(PASS_COMMAND_ARGS, val->str);*/

	return true;
}

UInt32 SetyCMStringParams;
bool Cmd_SetyCMString_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	char buffer[0x80];
	if (!ExtractFormatStringArgs(3, buffer, EXTRACT_ARGS_EX, SetyCMStringParams, &child, &grandchild, &src)) return true;

	SetMCMInternalString(child, grandchild, src, buffer);

	/*
	std::string path = MCMPath(child, grandchild, src);
	if (const auto val = StringToTilePath(path)) {
		*result = 1;
		val->parent->Set(val->id, buffer);
	}*/

	return true;
}

bool Cmd_SetUIFloat_Execute(COMMAND_ARGS)
{
	*result = 0;
	char src[0x100] = "\0";
	float value = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &value)) return true;

	/*
	if (const auto val = StringToTilePath(src)) {
		*result = 1;
		val->parent->Set(val->id, value);
	}*/

	return true;
}

CMCategory::CMCategory(const MCMMod& mod)
{
	id = mod.id;
	name = mod.title;
	doublestacked = mod.columns - 1;
	compatibilityMode = true;
}

CMSetting::CMSetting(const MCMMod& mod, const MCMItem& item)
{
	name = item.title;
	mods.emplace(mod.id);
}

Category::Category(const MCMMod& mod)
{
	name = mod.title;
	categoryID = mod.id;
}

Choice::Choice(const MCMMod& mod, const MCMItem& item) : CMSetting(mod, item)
{
	setting = IO();
}

Slider::Slider(const MCMMod& mod, const MCMItem& item) : CMSetting(mod, item)
{
	setting = IO();
}

Control::Control(const MCMMod& mod, const MCMItem& item) : CMSetting(mod, item)
{
	keyboard = IO();
	mouse = IO();
	controller = IO();
}

void ModConfigurationMenu::ReadMCM()
{
	for (const auto& mod : mods | std::views::values)
	{
		const auto tag = CMCategory(mod);
		mapCategories.emplace(tag.id, std::make_unique<CMCategory>(tag));

		setSettings.emplace(std::make_unique<Category>(mod));
	}

	for (const auto& [mod, val] : items)
	for (const auto& item : val | std::views::values)
	{
		if (item.enable == 0) continue;

		std::unique_ptr<CMSetting> setting;

		switch (item.type)
		{
		default:
		case MCMItem::kNone:
		case MCMItem::kString:		setting = std::make_unique<CMSetting>(mods[mod], item); break;

		case MCMItem::kChoice:
		case MCMItem::kOnOff:
		case MCMItem::kOnOffCustom:
		case MCMItem::kTick:		setting = std::make_unique<Choice>(mods[mod], item); break;

		case MCMItem::kSlider:		setting = std::make_unique<Slider>(mods[mod], item); break;

		case MCMItem::kControl:		setting = std::make_unique<Control>(mods[mod], item);break;
//			kThreeSliders = 8,
//			kRGB = 9,
		}

		//if (elem.contains("category"))		setting = std::make_unique<Category>(item);
//		else if (elem.contains("font"))		setting = std::make_unique<Font>(item);

		setSettings.emplace(std::move(setting));
	}

}

BGSListForm* modList;

bool doonce = false;
void MainLoop()
{
	if (MenuMode()) return;
	if (doonce) return;
	doonce = true;

	if (!modList) modList = reinterpret_cast<BGSListForm*>(TESForm::GetByID("The Mod Configuration Menu.esp", 0x000AE6));

	for (const auto iter : modList->list)
	{
		MCMMod mod{};

		mod.id = g_TESDataHandler->GetNthModName(iter->modIndex);
		mod.title = iter->GetTheName();
		mod.modID = iter->modIndex;

		mods.emplace(mod.modID, mod);
	}

	g_activeMod = g_TESDataHandler->LookupModByName("FOVSlider.esp")->modIndex;
	g_Reset = true;
}

void WriteMCMHooks()
{
	/*
		RegisterCommand GetModINISetting (21C0)
		RegisterCommand SetModINISetting (21C1)
		RegisterCommand GetMCMFloat (21C2)
		RegisterCommand SetMCMFloat (21C3)
		RegisterCommand SetMCMString (21C4)
		RegisterCommand SetMCMFloatMass (21C5)
		RegisterCommand SetMCMStringMass (21C6)
		RegisterCommand SetMCMModList (21C7)
		RegisterCommand GetMCMListWidth (21C8)
	 */


	g_commandInterface->GetByOpcode(0x21C2)->execute = Cmd_GetyCMFloat_Execute;
	g_commandInterface->GetByOpcode(0x21C3)->execute = Cmd_SetyCMFloat_Execute;
	g_commandInterface->GetByOpcode(0x21C4)->execute = Cmd_SetyCMString_Execute;
	SetyCMStringParams = g_commandInterface->GetByOpcode(0x21C4)->numParams;

//	g_commandInterface->GetByOpcode(5284)->execute = g_commandInterface->GetByOpcode(5663)->execute;
//	g_commandInterface->GetByOpcode(5285)->execute = g_commandInterface->GetByOpcode(5664)->execute;
//	g_commandInterface->GetByOpcode(5286)->execute = g_commandInterface->GetByOpcode(5665)->execute;
//	g_commandInterface->GetByOpcode(5346)->execute = g_commandInterface->GetByOpcode(5665)->execute;

}

