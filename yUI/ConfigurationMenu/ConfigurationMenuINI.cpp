#pragma once
#include "ConfigurationMenuStew.h"
#include <SimpleINILibrary.h>

inline std::map<std::tuple<std::filesystem::path, std::string, std::string>, SM_Value> ini_map;

inline void ReadINIInternal(const std::filesystem::path& iniPath, const std::filesystem::path& iniPath2)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

	CSimpleIniA::TNamesDepend sections;
	ini.GetAllSections(sections);

	for (const auto& section : sections)
	{
		CSimpleIniA::TNamesDepend keys;
		ini.GetAllKeys(section.pItem, keys);
		for (const auto& key : keys)
		{
			SM_Value value;

			if (key.pItem[0] == 'b' || key.pItem[0] == 'i') value = ini.GetLongValue(section.pItem, key.pItem);
			else if (key.pItem[0] == 'f') value = ini.GetDoubleValue(section.pItem, key.pItem);
			else if (key.pItem[0] == 's') value = static_cast<std::string>(ini.GetValue(section.pItem, key.pItem));

			ini_map.emplace(std::make_tuple(iniPath2, section.pItem, key.pItem), value);
		}
	}

	ini.SaveFile(iniPath.c_str(), false);
}

SM_Value SM_Setting::SettingSource::ReadINI()
{
	std::filesystem::path iniPath = GetCurPath();
	iniPath += std::get<0>(setting);

	const auto settingTuple = setting;

	if (ini_map.contains(settingTuple)) return ini_map[settingTuple];

	ReadINIInternal(iniPath, std::get<0>(setting));

	if (ini_map.contains(settingTuple)) return ini_map[settingTuple];

	return defaultValue;
}

inline void WriteINIInternal(const std::filesystem::path& iniPath, const SM_Setting::SettingSource::INISetting& setting, const SM_Value& value)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

	if (value.IsString())
		ini.SetValue(std::get<1>(setting).c_str(), std::get<2>(setting).c_str(), static_cast<std::string>(value).c_str());
	else if (value.IsFloat()) 
		ini.SetDoubleValue(std::get<1>(setting).c_str(), std::get<2>(setting).c_str(), value);
	else if (value.IsInteger())
		ini.SetLongValue(std::get<1>(setting).c_str(), std::get<2>(setting).c_str(), value);

	ini.SaveFile(iniPath.c_str(), false);
}

void SM_Setting::SettingSource::WriteINI(const SM_Value& value)
{
	std::filesystem::path iniPath = GetCurPath();
	iniPath += std::get<0>(setting);

	ini_map[setting] = value;

	WriteINIInternal(iniPath, setting, value);
}