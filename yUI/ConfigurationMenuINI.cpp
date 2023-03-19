#pragma once
#include "json.h"
#include "ConfigurationMenu/StewMenu/ConfigurationMenuStew.h"

inline std::unordered_map<std::filesystem::path, SM_Value> ini_map;

inline void ReadINIInternal(const std::filesystem::path& iniPath)
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
			std::filesystem::path fullPath = iniPath;
			fullPath += section.pItem;
			fullPath += key.pItem;

			SM_Value value;

			if (key.pItem[0] == 'b' || key.pItem[0] == 'i') value.emplace<SInt32>(ini.GetLongValue(section.pItem, key.pItem));
			else if (key.pItem[0] == 'f') value.emplace<Float64>(ini.GetDoubleValue(section.pItem, key.pItem));
			else if (key.pItem[0] == 's') value.emplace<std::string>(ini.GetValue(section.pItem, key.pItem));

			ini_map.emplace(fullPath, value);
		}
	}

	ini.SaveFile(iniPath.c_str(), false);
}

SM_Value SM_Setting::ReadINI()
{
	std::filesystem::path iniPath = GetCurPath();
	iniPath += setting.path;

	std::filesystem::path fullPath = iniPath;
	fullPath += setting.category;
	fullPath += setting.value;

	if (ini_map.contains(fullPath)) return ini_map[fullPath];

	ReadINIInternal(iniPath);

	if (ini_map.contains(fullPath)) return ini_map[fullPath];

	return valueDefault;
}

inline void WriteINIInternal(const std::filesystem::path& iniPath, const SM_Setting& setting, SM_Value value)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	if (ini.LoadFile(iniPath.c_str()) == SI_FILE) return;

	if (std::holds_alternative<std::string>(value))
		ini.SetValue(setting.setting.category.c_str(), setting.setting.value.c_str(), std::get<std::string>(value).c_str());
	else if (std::holds_alternative<Float64>(value)) 
		ini.SetDoubleValue(setting.setting.category.c_str(), setting.setting.value.c_str(), std::get<Float64>(value));
	else if (std::holds_alternative<SInt32>(value))
		ini.SetLongValue(setting.setting.category.c_str(), setting.setting.value.c_str(), std::get<SInt32>(value));

	ini.SaveFile(iniPath.c_str(), false);
}

void SM_Setting::WriteINI(SM_Value value)
{
	std::filesystem::path iniPath = GetCurPath();
	iniPath += setting.path;

	std::filesystem::path fullPath = iniPath;
	fullPath += setting.category;
	fullPath += setting.value;

	ini_map[fullPath] = value;

	WriteINIInternal(iniPath, *this, value);
}