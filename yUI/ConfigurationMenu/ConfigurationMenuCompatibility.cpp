#include "main.h"
#include "Menu.h"
#include "PluginAPI.h"
#include <string>

#include "ConfigurationMenu.h"
#include "GameData.h"

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

	std::optional<Float64> value;
	std::string valueAlt;

	UInt32 type = kNone;

	std::optional<UInt32> altFont;
	std::optional<UInt32> brightness;
	std::optional<UInt32> hightlight;
	std::optional<UInt32> indent;
	std::optional<UInt32> rgb;
	std::optional<UInt32> rbgalt;

	std::string textOFF;
	std::string textON;

	std::unordered_map<UInt8, std::pair<bool, std::string>> choices;

	struct Slider
	{
		std::optional<Float64> min;
		std::optional<Float64> max;
		std::optional<Float64> delta;
		std::optional<UInt32> decimal;
	};

	Slider slider;

	std::optional<Float64> defaultval;

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
			if (!slider.min.has_value() || !slider.max.has_value() || !slider.delta.has_value()) return false;
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
	UInt8 showList		= 0;

	UInt8 mouseover		= 0;


	std::string sliderTitle;

	std::string listTitle;

	std::list<UInt8> listSlider;
	std::list<UInt8> listScaleDefault;
	std::list<UInt8> listChoice;
	std::list<UInt8> listMouseover;


	void UpdateInternal()
	{
		if (listSlider.size())
		{
			if (showScale == 0)
			{
				activeSetting = listSlider.front();
				ShowScale();
			}
			else if (showScale == 2)
			{
				if (items[activeMod][activeSetting].IsValid()) { 
					ShowScale(0); 
					listSlider.pop_front();
				}
			}
		}
		else if (listChoice.size())
		{
			if (showList == 0)
			{
				activeSetting = listChoice.front();
				ShowList();
			}
			else if (showList == 2)
			{
				if (items[activeMod][activeSetting].IsValid()) { 
					ShowScale(0); 
					listChoice.pop_front();
				} 
			}
		}

	}

	Float64 GetInternal(UInt32 child, UInt32 grandchild, const std::string& src)
	{
		UpdateInternal();

		if (child == 0 && grandchild == 0)
		{
			if (src == "_activemod")	return activeMod;
			if (src == "_activeoption")	return activeSetting;
			if (src == "_reset")		return reset;
			if (src == "_newvalue")		return newValue;
			if (src == "_showscale")	return showScale;
			if (src == "_defaultscale")	return defaultScale;
		}

		return 0;
	}


	void SetInternal(UInt32 child, UInt32 grandchild, const std::string& src, Float64 val)
	{
		if (child == 0 && grandchild == 0)
		{
			if (src == "_activemod")		activeMod = val;
			if (src == "_activeoption")		activeSetting = val;
			if (src == "_reset")			Reset(val);
			if (src == "_newvalue")			newValue = val;
			if (src == "_showscale")		ShowScale(val);
			if (src == "_defaultscale")		defaultScale = val;

			if (src == "_value")			items[activeMod][activeSetting].value = val;


			// slider
			if (src == "_valuedecimal")		items[activeMod][activeSetting].slider.decimal = val;
			if (src == "_valueincrement")	items[activeMod][activeSetting].slider.delta = val; 
			if (src == "_valuemax")			items[activeMod][activeSetting].slider.max = val;
			if (src == "_valuemin")			items[activeMod][activeSetting].slider.min = val;

			// choice

		}

		if (child == 1 && grandchild == 0)
		{
			if (src == "_columns")			mods[activeMod].columns = val;
		}

		if (child == 1 && grandchild >= 1)
		{
			activeSetting = grandchild;
			items[activeMod][activeSetting].id = activeSetting;
			if (src == "_enable")			items[activeMod][activeSetting].enable = val;
			if (src == "_type") {
				const UInt32 type = std::trunc(val);
				
				items[activeMod][activeSetting].type = type; 

				if (type == CMMCMItem::kSlider)				listSlider.push_back(activeSetting);
				else if (type == CMMCMItem::kChoice)		listChoice.push_back(activeSetting);
				else if (type == CMMCMItem::kThreeSliders)	listSlider.push_back(activeSetting);
				else if (type == CMMCMItem::kRGB)			listSlider.push_back(activeSetting);
			}
			if (src == "_value")			items[activeMod][activeSetting].value = val;
		}

		if (child == 3 && grandchild >= 1)
		{
			if (src == "_enable")	items[activeMod][activeSetting].choices[grandchild].first = true;
		}

		UpdateInternal();
	}
	
	void SetInternalString(UInt32 child, UInt32 grandchild, const std::string& src, std::string val)
	{
		if (child == 1 && grandchild >= 1)
		{
			activeSetting = grandchild;
			items[activeMod][activeSetting].id = activeSetting;
			if (src == "_title")			items[activeMod][activeSetting].title = val;
			if (src == "value/*:1/string")
			{
				items[activeMod][activeSetting].valueAlt = val;
				if (!items[activeMod][activeSetting].value.has_value())
				{
					try {
						Float64 value = std::stod(val);
						items[activeMod][activeSetting].value = value;
					}
					catch (...) {
						if (items[activeMod][activeSetting].slider.max.has_value())
							items[activeMod][activeSetting].value = items[activeMod][activeSetting].slider.max.value();
					}
				}
			}
			if (src == "_texton")			items[activeMod][activeSetting].textON = val;
			if (src == "_textoff")			items[activeMod][activeSetting].textOFF = val;
		}

		if (child == 2 && grandchild == 0)
		{
			if (src == "_title")	sliderTitle = val;
		}

		if (child == 3 && grandchild == 0)
		{
			if (src == "_title")	listTitle = val;
		}

		if (child == 3 && grandchild >= 1)
		{
			if (src == "text/string")	items[activeMod][activeSetting].choices[grandchild].second = val;
		}

		UpdateInternal();
	}

	void Reset(const UInt32 val = true) { reset = val; }
	void ShowScale(const UInt32 val = true) { showScale = val; }
	void ShowList(const UInt32 val = true) { showList = val; }
};

std::string MCMPath(UInt32 child, UInt32 grandchild, std::string src)
{

	std::filesystem::path path = "StartMenu/MCM";
	if (child == 0)
	{
	}
	else if (child == 1) {
		path /= "MCM_Options";
		if (grandchild != 0) {
			path /= "Option" + std::to_string(grandchild);
		}
	}
	else if (child == 2) {
		path /= "MCM_Scale";
	}
	else if (child == 3) {
		path /= "MCM_List";
		if (grandchild != 0) {
			path /= "Item" + std::to_string(grandchild);
		}
	}
	else if (child == 4) {
		path /= "MCM_Trigger";
	}
	else if (child == 5) {
		path /= "MCM_ModList";
		if (grandchild == 0) {}
		else if (grandchild <= 23) {
			path /= "Mod" + std::to_string(grandchild);
		}
		else {
			path /= "SubMenu" + std::to_string(grandchild - 23);
		}
	}
	else if (child == 9) {
		path /= "MCM_Info";
	}
	else if (child == 15) {
		path /= "MCM_Input";
	}
	else if (child == 17) {
		path /= "MCM_Images";
	}
	path /= std::string(src);
	return path.string();
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
		for (const auto& iter : item.choices)
		{
			const auto id = iter.first;
			const auto shouldShow = iter.second.first;
			const auto& str = iter.second.second;
			if (shouldShow) choice.emplace((CMValue)(SInt32)id, str);
		}
	}

	setting.mcm.modID = mod.modID;
	setting.mcm.option = item.id;
}

CMSettingSlider::CMSettingSlider(const CMMCMMod& mod, const CMMCMItem& item) : CMSetting(mod, item)
{
	setting = IO();

	if (item.slider.max.has_value())
		max = item.slider.max.value();
	if (item.slider.min.has_value())
		min = item.slider.min.value();
	if (item.slider.delta.has_value())
		delta = item.slider.delta.value();

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

void ModConfigurationMenu::ReadMCM()
{
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

//BGSListForm modListNew{};

bool doonce = false;
void MainLoop()
{
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

	MCMWrapper::GetSingleton().activeMod = g_TESDataHandler->LookupModByName("MCM Example Menu.esp")->modIndex;
	MCMWrapper::GetSingleton().Reset();

//	ModConfigurationMenu::GetSingleton()->ReadMCM();
}

std::optional<CMValue> CMSetting::IO::ReadMCM()
{
	if (items[mcm.modID][mcm.option].value.has_value())
		return (Float64)items[mcm.modID][mcm.option].value.value();
	else
		return (Float64)0;

	return std::optional<CMValue>();
}

void CMSetting::IO::WriteMCM(const CMValue& value) const
{
	items[mcm.modID][mcm.option].value = value.GetAsFloat();
}

namespace Cmd
{
	enum Opcode
	{
		// nvse
		kGetUIFloat = 0x14A4,
		kSetUIFloat = 0x14A5,
		kSetUIString = 0x14A6,
		kSetUIStringEx = 0x14E2,
		
		// xnvse
		kGetUIFloatAlt = 0x161F,
		kSetUIFloatAlt = 0x1620,
		kSetUIStringAlt = 0x1621,

		// mcm
		kGetMCMFloat = 0x21C2,
		kSetMCMFloat = 0x21C3,
		kSetMCMString = 0x21C4,

		kGetModINISetting = 0x21C0,
		kSetModINISetting = 0x21C1,
		kSetMCMFloatMass = 0x21C5,
		kSetMCMStringMass = 0x21C6,
		kSetMCMModList = 0x21C7,
		kGetMCMListWidth = 0x21C8,

		// jip ln
		kGetString = 0x2639
	};

	bool GetyCMFloat_Execute(COMMAND_ARGS)
	{
		*result = -999;
		char src[0x200] = "\0";
		SInt64 child = 0;
		SInt64 grandchild = 0;
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;
		*result = MCMWrapper::GetSingleton().GetInternal(child, grandchild, src);
		return true;
	}

	bool SetyCMFloat_Execute(COMMAND_ARGS)
	{
		*result = 0;
		char src[0x200] = "\0";
		SInt64 child = 0;
		SInt64 grandchild = 0;
		Float32 value = 0;
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src, &value)) return true;
		MCMWrapper::GetSingleton().SetInternal(child, grandchild, src, value);
		return true;
	}

	bool GetyCMString_Execute(COMMAND_ARGS)
	{
		char src[0x200] = "\0";
		SInt64 child = 0;
		SInt64 grandchild = 0;
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &child, &grandchild, &src)) return true;
//		*result = MCMWrapper::GetSingleton().GetInternalString(child, grandchild, src);
		return true;
	}

	UInt32 SetyCMStringParams;
	bool SetyCMString_Execute(COMMAND_ARGS)
	{
		*result = 0;
		char src[0x200] = "\0";
		SInt64 child = 0;
		SInt64 grandchild = 0;
		char buffer[0x200];
		if (!ExtractFormatStringArgs(3, buffer, EXTRACT_ARGS_EX, SetyCMStringParams, &child, &grandchild, &src)) return true;
		MCMWrapper::GetSingleton().SetInternalString(child, grandchild, src, buffer);
		return true;
	}

	bool (*GetUIFloatAlt_Execute)(COMMAND_ARGS);
	bool (*SetUIFloatAlt_Execute)(COMMAND_ARGS);
	bool (*SetUIStringAlt_Execute)(COMMAND_ARGS);

	bool GetUIFloat_Execute(COMMAND_ARGS)
	{
		*result = -999;
		char src[0x200] = "\0";
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src)) return true;

		std::string string = ToLower(src);

		if (std::strncmp(string.c_str(), "startmenu/mcm/", 14)) return GetUIFloatAlt_Execute(PASS_COMMAND_ARGS);

		string = string.substr(14, string.size());

		UInt32 child = 0;
		UInt32 grandchild = 0;

		auto numPos = string.find_first_of("*:");
		auto slashPos = string.find_first_of('/');

		if (numPos != -1 && slashPos != -1)
		{
			child = std::stoi(string.substr(numPos + 2, slashPos));

			string = string.substr(slashPos + 1, string.size());

			numPos = string.find_first_of("*:");
			slashPos = string.find_first_of('/');

			if (numPos != -1 && slashPos != -1)
			{
				grandchild = std::stoi(string.substr(numPos + 2, slashPos));

				string = string.substr(slashPos + 1, string.size());
			}
		}

		*result = MCMWrapper::GetSingleton().GetInternal(child, grandchild, string);

		return true;
	}


	bool SetUIFloat_Execute(COMMAND_ARGS)
	{
		*result = -999;
		char src[0x200] = "\0";
		float value = 0;
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &value)) return true;
		
		std::string string = ToLower(src);

		if (std::strncmp(string.c_str(), "startmenu/mcm/", 14)) return SetUIFloatAlt_Execute(PASS_COMMAND_ARGS);
		
		string = string.substr(14, string.size());

		UInt32 child = 0;
		UInt32 grandchild = 0;

		auto numPos = string.find_first_of("*:");
		auto slashPos = string.find_first_of('/');

		if (numPos != -1 && slashPos != -1)
		{
			child = std::stoi(string.substr(numPos + 2, slashPos));

			string = string.substr(slashPos + 1, string.size());

			numPos = string.find_first_of("*:");
			slashPos = string.find_first_of('/');

			if (numPos != -1 && slashPos != -1)
			{
				grandchild = std::stoi(string.substr(numPos + 2, slashPos));

				string = string.substr(slashPos + 1, string.size());
			}
		}

		MCMWrapper::GetSingleton().SetInternal(child, grandchild, string, value);

		return true;
	}

	bool SetUIString_Execute(COMMAND_ARGS)
	{
		*result = 0;
		char src[0x200] = "\0";
		char buffer[0x200] = "\0";
		if (!ExtractArgsEx(EXTRACT_ARGS_EX, &src, &buffer)) return true;

		std::string string = ToLower(src);

		if (std::strncmp(string.c_str(), "startmenu/mcm/", 14))  return SetUIStringAlt_Execute(PASS_COMMAND_ARGS);

		string = string.substr(14, string.size());

		UInt32 child = 0;
		UInt32 grandchild = 0;

		auto numPos = string.find_first_of("*:");
		auto slashPos = string.find_first_of('/');

		if (numPos != -1 && slashPos != -1)
		{
			child = std::stoi(string.substr(numPos + 2, slashPos));

			string = string.substr(slashPos + 1, string.size());

			numPos = string.find_first_of("*:");
			slashPos = string.find_first_of('/');

			if (numPos != -1 && slashPos != -1)
			{
				grandchild = std::stoi(string.substr(numPos + 2, slashPos));

				string = string.substr(slashPos + 1, string.size());
			}
		}

		MCMWrapper::GetSingleton().SetInternalString(child, grandchild, string, buffer);

		return true;
	}

	UInt32 SetUIStringParams;
	bool SetUIStringEx_Execute(COMMAND_ARGS)
	{
		*result = 0;
		char src[0x200] = "\0";
		char buffer[0x200];
		if (!ExtractFormatStringArgs(1, buffer, EXTRACT_ARGS_EX, SetUIStringParams, &src)) return true;

		std::string string = ToLower(src);

		if (std::strncmp(string.c_str(), "startmenu/mcm/", 14)) return SetUIStringAlt_Execute(PASS_COMMAND_ARGS);

		string = string.substr(14, string.size());

		UInt32 child = 0;
		UInt32 grandchild = 0;

		auto numPos = string.find_first_of("*:");
		auto slashPos = string.find_first_of('/');

		if (numPos != -1 && slashPos != -1)
		{
			child = std::stoi(string.substr(numPos + 2, slashPos));

			string = string.substr(slashPos + 1, string.size());

			numPos = string.find_first_of("*:");
			slashPos = string.find_first_of('/');

			if (numPos != -1 && slashPos != -1)
			{
				grandchild = std::stoi(string.substr(numPos + 2, slashPos));

				string = string.substr(slashPos + 1, string.size());
			}
		}

		MCMWrapper::GetSingleton().SetInternalString(child, grandchild, string, buffer);

		return true;
	}

	void WriteHooks()
	{
		g_commandInterface->GetByOpcode(kGetMCMFloat)->execute = GetyCMFloat_Execute;
		g_commandInterface->GetByOpcode(kSetMCMFloat)->execute = SetyCMFloat_Execute;
		g_commandInterface->GetByOpcode(kSetMCMString)->execute = SetyCMString_Execute;
		SetyCMStringParams = g_commandInterface->GetByOpcode(kSetMCMString)->numParams;

		GetUIFloatAlt_Execute = g_commandInterface->GetByOpcode(kGetUIFloatAlt)->execute;
		g_commandInterface->GetByOpcode(kGetUIFloat)->execute = GetUIFloat_Execute;

		SetUIFloatAlt_Execute = g_commandInterface->GetByOpcode(kSetUIFloatAlt)->execute;
		g_commandInterface->GetByOpcode(kSetUIFloat)->execute = SetUIFloat_Execute;

		SetUIStringAlt_Execute = g_commandInterface->GetByOpcode(kSetUIStringAlt)->execute;
		g_commandInterface->GetByOpcode(kSetUIString)->execute = SetUIString_Execute;
		g_commandInterface->GetByOpcode(kSetUIStringEx)->execute = SetUIStringEx_Execute;
		SetUIStringParams = g_commandInterface->GetByOpcode(kSetUIStringEx)->numParams;
	}
}


void WriteMCMHooks()
{
	Cmd::WriteHooks();
}