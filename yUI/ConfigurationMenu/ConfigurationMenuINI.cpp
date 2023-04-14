#pragma once
#include "ConfigurationMenu.h"
#include <SimpleINILibrary.h>

#include "GameData.h"
#include "TESForm.h"
#include "InterfaceManager.h"
#include "Setting.h"

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

inline void WriteINIInternal(const std::filesystem::path& iniPath, const CMSetting::IO::INI& setting, const SM_Value& value)
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


std::optional<SM_Value> CMSetting::IO::ReadSaved()
{
	return {};
}

void CMSetting::IO::WriteSaved(const SM_Value& value) const
{
}

std::optional<SM_Value> CMSetting::IO::ReadINI()
{
	std::filesystem::path iniPath = GetCurPath();
	iniPath += std::get<0>(ini);

	const auto settingTuple = ini;

	if (ini_map.contains(settingTuple)) return ini_map[settingTuple];

	ReadINIInternal(iniPath, std::get<0>(ini));

	if (ini_map.contains(settingTuple)) return ini_map[settingTuple];

	return {};
}

void CMSetting::IO::WriteINI(const SM_Value& value) const
{
	std::filesystem::path iniPath = GetCurPath();
	iniPath += std::get<0>(ini);

	ini_map[ini] = value;

	WriteINIInternal(iniPath, ini, value);
}

std::optional<SM_Value> CMSetting::IO::ReadXML()
{
	if (xml.empty()) return {};

	const auto val = HUDMainMenu::GetSingleton()->tile->Get(xml.c_str());
	const auto val2 = InterfaceManager::GetSingleton()->globalsTile->Get(xml.c_str());
	if (defaultValue.IsString()) {
		if (val) return std::string(val);
		if (val2) return std::string(val2);
		return {};
	}
	if (val) return static_cast<Float32>(val);
	if (val2) return static_cast<Float32>(val2);
	return {};
}

void CMSetting::IO::WriteXML(const SM_Value& value) const
{
	if (xml.empty()) return;

	if (value.IsString())
	{
		HUDMainMenu::GetSingleton()->tile->Set(xml.c_str(), value.GetAsString());
		InterfaceManager::GetSingleton()->globalsTile->Set(xml.c_str(), value.GetAsString());
		return;
	}
	HUDMainMenu::GetSingleton()->tile->Set(xml.c_str(), value.GetAsFloat());
	InterfaceManager::GetSingleton()->globalsTile->Set(xml.c_str(), value.GetAsFloat());
}

std::optional<SM_Value> CMSetting::IO::ReadGameSetting()
{
	if (const auto setting = GetINISetting(gamesetting); setting && !setting->GetAsString().empty()) return setting->GetAsString();
	return {};
}

void CMSetting::IO::WriteGameSetting(const SM_Value& value) const
{
	if (const auto setting = GetINISetting(gamesetting); setting)
	{
		if (value.IsString()) setting->Set(value.GetAsString().c_str());
		else setting->Set(value.GetAsFloat());
	}
}

std::optional<SM_Value> CMSetting::IO::ReadGameINI()
{
	if (const auto setting = GetGameSetting(gameini); setting && !setting->GetAsString().empty()) return setting->GetAsString();
	return {};
}

void CMSetting::IO::WriteGameINI(const SM_Value& value) const
{
	if (const auto setting = GetGameSetting(gameini); setting)
	{
		if (value.IsString()) setting->Set(value.GetAsString().c_str());
		else setting->Set(value.GetAsFloat());
	}
}

std::optional<SM_Value> CMSetting::IO::ReadGlobal()
{
	for (const auto iter : TESDataHandler::GetSingleton()->globalList)
		if (std::string(iter->name.CStr()) == global) return iter->data;
	return {};
}

void CMSetting::IO::WriteGlobal(const SM_Value& value) const
{
	for (const auto iter : TESDataHandler::GetSingleton()->globalList)
		if (std::string(iter->name.CStr()) == global) iter->data = value.GetAsFloat();
}

SM_Value CMSetting::IO::Read()
{
	if (g_saveValue) 
		if (const auto saved =		ReadSaved()) return saved.value();
	if (const auto ini =			ReadINI()) return ini.value();
	if (const auto xml =			ReadXML()) return xml.value();
	if (const auto global =			ReadGlobal()) return global.value();
	if (const auto gameSetting =	ReadGameSetting()) return gameSetting.value();
	if (const auto gameINI =		ReadGameSetting()) return gameINI.value();
	return defaultValue;
}

void CMSetting::IO::Write(const SM_Value& value) const
{
	if (g_saveValue) 
		WriteSaved(value);
	WriteINI(value);
	WriteXML(value);
	WriteGlobal(value);
	WriteGameSetting(value);
	WriteGameINI(value);
}