#include "main.h"
#include "Menu.h"
#include "PluginAPI.h"
#include <string>

#include "ConfigurationMenu.h"
#include "GameData.h"

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS);
bool Cmd_SetyCMFloat_Execute(COMMAND_ARGS);


class CMMCMMod
{
public:
	std::string id;
	std::string title;
	UInt8 modID = 0;
	UInt8 columns = 1;
};

class CMMCMItem
{
public:
	UInt8 id = 0;

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
	UInt32 brightness = 255;
	UInt32 hightlight = 0;
	UInt32 indent = 0;
	UInt32 rgb = 0;
	UInt32 rbgalt = 0;

	std::string textOFF;
	std::string textON;

	std::unordered_map<UInt8, std::string> choices;

	Float64 min;
	Float64 max;
	Float64 delta;
	UInt32 decimal;

	Float64 defaultval;

	Float64 currentValue;

	bool IsValid() const
	{
		if (prefix.empty() && suffix.empty() && title.empty()) return false;

		if (type == kNone) {}
		else if (type == kChoice)
		{
			if (choices.empty()) return false;
		}
		else if (type == kSlider)
		{
			if (max == 0 && min == 0 || delta == 0) return false;
		
		}
		else if (type == kControl) {}
		else if (type == kOnOff) {}
		else if (type == kTick) {}
		else if (type == kOnOffCustom) 
		{
			if (textOFF.empty() || textON.empty()) return false;
		}
		else if (type == kString) {}
		else if (type == kThreeSliders) {}
		else if (type == kRGB) {}
		
		return true;
	}
};

std::unordered_map<UInt8, CMMCMMod> mods;
std::unordered_map<UInt8, std::unordered_map<UInt8, CMMCMItem>> items;

class MCMWrapper
{
public:
	static MCMWrapper& GetSingleton()
	{
		static MCMWrapper instance;
		return instance;
	}

	MCMWrapper() {};
	~MCMWrapper() {};

	UInt8 activeMod		= 0;
	UInt8 activeTab		= 0;
	UInt8 activeSetting	= 0;

	UInt8 reset			= 0;
	UInt8 newValue		= 0;
	UInt8 showScale		= 0;
	UInt8 defaultScale	= 0;

	std::string sliderTitle;

	std::list<UInt8> listSlider;

	UInt8 sliderAddress = 0;

	void MainLoop()
	{
		if (listSlider.size())
		{
			if (showScale == 0)
			{
				sliderAddress = listSlider.front();
				activeSetting = sliderAddress;
				ShowScale();
			}
			if (showScale == 2)
			{
				if (items[activeMod][sliderAddress].IsValid()) { 
					ShowScale(0); 
					listSlider.pop_front();
				}
			}

		}
	}

	Float64 GetInternal(UInt32 child, UInt32 grandchild, std::string src)
	{
		if (child == 0 && grandchild == 0)
		{
			if (src == "_ActiveMod")	return activeMod;
			if (src == "_ActiveOption")	return activeSetting;
			if (src == "_Reset")		return reset;
			if (src == "_NewValue")		return newValue;
			if (src == "_ShowScale")	return showScale;
			if (src == "_DefaultScale")	return defaultScale;
		}

		return 0;
	}


	void SetInternal(UInt32 child, UInt32 grandchild, std::string src, Float64 val)
	{
		if (child == 0 && grandchild == 0)
		{
			if (src == "_ActiveMod")	activeMod = val;
			if (src == "_ActiveOption")	activeSetting = val;
			if (src == "_Reset")		Reset(val);
			if (src == "_NewValue")		newValue = val;
			if (src == "_ShowScale")	ShowScale(val);
			if (src == "_DefaultScale")	defaultScale = val;

			MainLoop();

			if (src == "_Value")			
				items[activeMod][sliderAddress].currentValue = val;
			if (src == "_ValueDecimal")		
				items[activeMod][sliderAddress].decimal = val;
			if (src == "_ValueIncrement")	
				items[activeMod][sliderAddress].delta = val;
			if (src == "_ValueMax")			
				items[activeMod][sliderAddress].max = val;
			if (src == "_ValueMin")			
				items[activeMod][sliderAddress].min = val;
		}

		if (child == 1 && grandchild == 0)
		{
			if (src == "_columns")			mods[activeMod].columns = val;
		}

		if (child == 1 && grandchild >= 1)
		{
			activeSetting = grandchild;
			items[activeMod][activeSetting].id = activeSetting;
			if (src == "_Enable")			items[activeMod][activeSetting].enable = val;
			if (src == "_Type") {
				items[activeMod][activeSetting].type = std::trunc(val); 
				if (std::trunc(val) == CMMCMItem::kSlider)
				{
					listSlider.push_back(activeSetting);
				}
			}
			if (src == "_Value")			items[activeMod][activeSetting].value = val;
		}
	}
	
	void SetInternalString(UInt32 child, UInt32 grandchild, std::string src, std::string val)
	{
		if (child == 1 && grandchild >= 1)
		{
			activeSetting = grandchild;
			items[activeMod][activeSetting].id = activeSetting;
			if (src == "_Title")			items[activeMod][activeSetting].title = val;
			if (src == "value/*:1/string")	items[activeMod][activeSetting].valueAlt = val;
			if (src == "_textOn")			items[activeMod][activeSetting].textON = val;
			if (src == "_textOff")			items[activeMod][activeSetting].textOFF = val;
		}

		if (child == 2 && grandchild == 0)
		{
			if (src == "_Title")	sliderTitle = val;
		}
	}

	void Reset(const UInt32 val = true)
	{
		reset = val;
	}

	void ShowScale(const UInt32 val = true)
	{
		if (!val) sliderAddress = 0;
		showScale = val;
	}
};

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

bool Cmd_GetyCMFloat_Execute(COMMAND_ARGS)
{
	*result = -999;
	char src[0x100] = "\0";
	SInt64 child = 0;
	SInt64 grandchild = 0;
	if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;
//	std::string path = MCMPath(child, grandchild, src);
//	if (const auto val = StringToTilePath(path)) *result = val->num;

	*result = MCMWrapper::GetSingleton().GetInternal(child, grandchild, src);

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

	MCMWrapper::GetSingleton().SetInternal(child, grandchild, src, value);

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

	MCMWrapper::GetSingleton().SetInternalString(child, grandchild, src, buffer);

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

CMCategory::CMCategory(const CMMCMMod& mod)
{
	id = mod.id;
	name = mod.title;
	doublestacked = mod.columns - 1;
	compatibilityMode = true;
}

CMSetting::CMSetting(const CMMCMMod& mod, const CMMCMItem& item)
{
	name = item.title;
	mods.emplace(mod.id);
}

CMSettingCategory::CMSettingCategory(const CMMCMMod& mod)
{
	name = mod.title;
	categoryID = mod.id;
}

CMSettingChoice::CMSettingChoice(const CMMCMMod& mod, const CMMCMItem& item) : CMSetting(mod, item)
{
	setting = IO();
	if (item.type == CMMCMItem::kOnOff || item.type == CMMCMItem::kTick)
	{
		choice.emplace((CMValue) (SInt32) 0, "OFF");
		choice.emplace((CMValue) (SInt32) 1, "ON");
	}
	else if (item.type == CMMCMItem::kOnOffCustom)
	{
		choice.emplace((CMValue) (SInt32) 0, "OFF");
		choice.emplace((CMValue) (SInt32) 1, "ON");

		// hated how it looked
//		choice.emplace((CMValue) (SInt32) 0, item.textOFF);
//		choice.emplace((CMValue) (SInt32) 1, item.textON);
	}
	else if (item.type == CMMCMItem::kChoice)
	{

	}

	setting.mcm.modID = mod.modID;
	setting.mcm.option = item.id;
}

CMSettingSlider::CMSettingSlider(const CMMCMMod& mod, const CMMCMItem& item) : CMSetting(mod, item)
{
	setting = IO();

	max = item.max;
	min = item.min;
	delta = item.delta;

	setting.mcm.modID = mod.modID;
	setting.mcm.option = item.id;
}

CMSettingControl::CMSettingControl(const CMMCMMod& mod, const CMMCMItem& item) : CMSetting(mod, item)
{
	keyboard = IO();
	mouse = IO();
	controller = IO();
}

UInt8 doonce2 = 0;
bool start2 = false;

void ModConfigurationMenu::ReadMCM()
{

//	if (doonce2 >= 2) return;

	for (const auto& [mod, val] : items)
		for (const auto& [id, item] : val)
		{
//			if (!item.IsValid()) return;
		}

	if (!start2) return;

	for (const auto& mod : mods | std::views::values)
	{
		const auto tag = CMCategory(mod);
		mapCategories.emplace(tag.id, std::make_unique<CMCategory>(tag));

		setSettings.emplace(std::make_unique<CMSettingCategory>(mod));
	}

	for (const auto& [mod, val] : items)
	for (const auto& [id, item] : val)
	{
		if (!item.IsValid()) continue;

		if (item.enable == 0) continue;

		std::unique_ptr<CMSetting> setting;

		switch (item.type)
		{
		default:
		case CMMCMItem::kNone:
		case CMMCMItem::kString:		setting = std::make_unique<CMSetting>(mods[mod], item); break;

		case CMMCMItem::kChoice:
		case CMMCMItem::kOnOff:
		case CMMCMItem::kOnOffCustom:
		case CMMCMItem::kTick:			setting = std::make_unique<CMSettingChoice>(mods[mod], item); break;

		case CMMCMItem::kSlider:		setting = std::make_unique<CMSettingSlider>(mods[mod], item); break;

		case CMMCMItem::kControl:		setting = std::make_unique<CMSettingControl>(mods[mod], item);break;
//			kThreeSliders = 8,
//			kRGB = 9,
		}

		//if (elem.contains("category"))		setting = std::make_unique<CMSettingCategory>(item);
//		else if (elem.contains("font"))		setting = std::make_unique<CMSettingFont>(item);

		setSettings.emplace(std::move(setting));
	}

	doonce2++;
}

BGSListForm* modList;

bool doonce = false;
void MainLoop()
{
	MCMWrapper::GetSingleton().MainLoop();

	if (MenuMode()) return;
	

	if (doonce) return;
	doonce = true;

	if (!modList) modList = reinterpret_cast<BGSListForm*>(TESForm::GetByID("The Mod Configuration Menu.esp", 0x000AE6));
	

	for (const auto iter : modList->list)
	{
		CMMCMMod mod{};

		mod.id = g_TESDataHandler->GetNthModName(iter->modIndex);
		mod.title = iter->GetTheName();
		mod.modID = iter->modIndex;

		mods.emplace(mod.modID, mod);
	}

	MCMWrapper::GetSingleton().activeMod = g_TESDataHandler->LookupModByName("FOVSlider.esp")->modIndex;
	MCMWrapper::GetSingleton().Reset();

//	ModConfigurationMenu::GetSingleton()->ReadMCM();

	start2 = true;
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

std::optional<CMValue> CMSetting::IO::ReadMCM()
{
	return (SInt32) items[mcm.modID][mcm.option].value;
	return std::optional<CMValue>();
}

void CMSetting::IO::WriteMCM(const CMValue& value) const
{
	items[mcm.modID][mcm.option].value = value;
}
