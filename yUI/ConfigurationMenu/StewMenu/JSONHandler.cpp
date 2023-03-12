#include "JSONHandler.h"
#include "StewMenu.h"
#include "json.h"

using json = nlohmann::json;

void InitTweaksListFromJSON()
{
	// handle settings from INIs in the "Tweaks\\MenuConfig\\" folder
	const auto dir = GetCurPath() + "\\Data\\menus\\";

	for (auto& iter : std::filesystem::directory_iterator(dir))
	{
		if (iter.is_directory()) continue;
		if (iter.path().extension().string() == ".json") ProcessJson(iter.path().string());
	}
}

void ProcessJson(std::string path)
{
	auto menu = StewMenu::GetSingleton();
	try
	{
		std::ifstream i(path);
		json j;
		i >> j;

		if (j.is_array())
		{
			for (auto& elem : j)
			{
				if (!elem.is_object())
				{
					Log(true, Log::kConsole) << ("JSON error: expected tweak json object");
					continue;
				}

				auto tweakName = elem.contains("name") ? elem["name"].get<std::string>() : "";
				auto internalName = elem.contains("internalName") ? elem["internalName"].get<std::string>() : "";
				const char* settingCategory = nullptr;
				std::string internalCategoryStr;
				if (elem.contains("internalCategory"))
				{
					internalCategoryStr = elem["internalCategory"].get<std::string>();
					settingCategory = internalCategoryStr.c_str();
				}

				auto description = elem.contains("description") ? elem["description"].get<std::string>() : "";
				signed int priority = elem.contains("priority") ? elem["priority"].get<signed int>() : 0;

				auto category = elem.contains("category") ? elem["category"].get<std::string>() : "Other";
				menu->allCategories.insert((char*)category.c_str());

				auto stewMenuItem = new StewMenuItem(tweakName.c_str(), description.c_str(), priority, internalName.c_str(), settingCategory, category.c_str());
				menu->tweaksListBox.Insert(stewMenuItem, tweakName.c_str())->SetFloat(kTileValue_id, kStewMenu_TweakListItem);
				
				if (elem.contains("subsettings"))
				{
					auto subsettingsElem = elem["subsettings"];
					if (subsettingsElem.is_array())
					{
						for (auto& e : subsettingsElem)
						{
							if (!e.is_object())
							{
								Log(true, Log::kConsole) << ("JSON error: expected subsetting json object");
								continue;
							}

							SubSettingData::ElementType type = SubSettingData::kSettingType_Boolean;

							if (e.contains("type"))
							{
								auto typeStr = e["type"].get<std::string>();
								type = SubSettingData::StrToType(typeStr[0]);
							}

							auto name = e.contains("name") ? e["name"].get<std::string>() : "";
							internalName = e.contains("internalName") ? e["internalName"].get<std::string>() : "";
							description = e.contains("description") ? e["description"].get<std::string>() : "";
							auto internalCategory = e.contains("internalCategory") ? e["internalCategory"].get<std::string>() : "";

							auto subSetting = new StewMenuSubsettingItem(name.c_str(), description.c_str(), internalName.c_str(), internalCategory.c_str());

							if (e.contains("options"))
							{
								auto optionsElem = e["options"];
								if (optionsElem.is_array())
								{
									for (auto& option : optionsElem)
									{
										auto displayStr = option.contains("name") ? option["name"].get<std::string>() : "";
										auto value = option.contains("value") ? option["value"].get<signed int>() : 0;
										description = option.contains("description") ? option["description"].get<std::string>() : "";
										subSetting->data.AddOption(displayStr.c_str(), value, description.c_str());
									}
								}
								else
								{
									Log(true, Log::kConsole) << ("JSON error: Dropdown options should be an array");
								}

								type = SubSettingData::kSettingType_Options;
							}

							subSetting->data.SetType(type);
							switch (type)
							{
							case SubSettingData::kSettingType_Boolean:
							{
								subSetting->data.minValue = 0;
								subSetting->data.maxValue = 1;
								break;
							}
							case SubSettingData::kSettingType_NumberInput:
							case SubSettingData::kSettingType_Slider:
							{
								auto minValue = e.contains("minValue") ? e["minValue"].get<float>() : -INT_MAX;
								auto maxValue = e.contains("maxValue") ? e["maxValue"].get<float>() : INT_MAX - 1;
								subSetting->data.minValue = minValue;
								subSetting->data.maxValue = maxValue;
								break;
							}
							}

							stewMenuItem->AddSubsetting(subSetting);
						}
					}
					else
					{
						Log(true, Log::kConsole) << ("JSON error: Subsettings should be an array");
					}
				}
			}
		}
	}
	catch (nlohmann::json::exception& e)
	{
		Log(true, Log::kConsole) << ("The JSON is incorrectly formatted! It will not be applied.");
		Log(true, Log::kConsole) << ("JSON error: %s\n", e.what());
	}
}