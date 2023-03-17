#include "JSONHandler.h"
#include "ConfigurationMenuStew.h"
#include "json.h"



template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
	return set;
}

void HandleTag(nlohmann::basic_json<> elem)
{
	if (!elem.is_object())
	{
		Log() << "JSON error: Expected object";
		return;
	}

	JS_Tag tag{};

	if (elem.contains("id"))			tag.id = elem["id"].get<std::string>();
	if (elem.contains("name"))			tag.name = elem["name"].get<std::string>();
	if (elem.contains("description"))	tag.description = elem["description"].get<std::string>();

	ConfigurationMenu::GetSingleton()->g_Tags.push_back(std::make_unique<JS_Tag>(tag));
}

void HandleMod(nlohmann::basic_json<> elem)
{
	if (!elem.is_object())
	{
		Log() << "JSON error: Expected object";
		return;
	}

	JS_Mod mod{};

	if (elem.contains("id"))			mod.id = elem["id"].get<std::string>();
	if (elem.contains("name"))			mod.name = elem["name"].get<std::string>();
	if (elem.contains("description"))	mod.description = elem["description"].get<std::string>();

	if (elem.contains("version"))		mod.version = elem["version"].get<std::string>();

	if (elem.contains("tags"))			mod.tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));

	ConfigurationMenu::GetSingleton()->g_Mods.push_back(std::make_unique<JS_Mod>(mod));
}

void HandleSetting(nlohmann::basic_json<> elem)
{
	if (!elem.is_object())
	{
		Log() << "JSON error: Expected object";
		return;
	}

	JS_Setting setting{};

	if (elem.contains("id"))			setting.id = elem["id"].get<std::string>();
	if (elem.contains("name"))			setting.name = elem["name"].get<std::string>();
	if (elem.contains("description"))	setting.description = elem["description"].get<std::string>();

	if (elem.contains("tags"))			setting.tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));
	if (elem.contains("mods"))			setting.mods.insert_range(GetSetFromElement<std::string>(elem["mods"]));

	ConfigurationMenu::GetSingleton()->g_Settings.push_back(std::make_unique<JS_Setting>(setting));
}

void HandleJSON(const std::filesystem::path& path)
{
	Log() << "\nJSON message: reading  " + path.string();
	try
	{
		std::ifstream i(path);
		auto j = nlohmann::json::parse(i, nullptr, true, true);

		if (j.contains("tags") && j["tags"].is_array())
			for (const auto& elem : j["tags"]) HandleTag(elem);
		else
			Log() << "JSON message: ySI item array not detected in " + path.string();

		if (j.contains("mods") && j["mods"].is_array())
			for (const auto& elem : j["mods"]) HandleMod(elem);
		else
			Log() << "JSON message: ySI category array not detected in " + path.string();

		if (j.contains("settings") && j["settings"].is_array())
			for (const auto& elem : j["settings"]) HandleSetting(elem);
		else
			Log() << "JSON message: ySI tab array not detected in " + path.string();
	}
	catch (nlohmann::json::exception& e)
	{
		Log() << "JSON error: JSON file is incorrectly formatted! It will not be applied. " + path.string();
		Log() << FormatString("JSON error: %s\n", e.what());
	}
}


void InitTweaksListFromJSON()
{
	// handle settings from INIs in the "Tweaks\\MenuConfig\\" folder

	const auto dir = GetCurPath() + "\\Data\\menus\\";

	for (auto& iter : std::filesystem::directory_iterator(dir))
	{
		if (iter.is_directory()) continue;
		if (iter.path().extension().string() == ".json") HandleJSON(iter.path());
	}

	for (const auto& mod : ConfigurationMenu::GetSingleton()->g_Mods)
	{
		auto stewMenuItem = new SM_Mod(mod->name, mod->description, 0, mod->id, "", "");
		ConfigurationMenu::GetSingleton()->tweaksListBox.Insert(stewMenuItem, mod->name.c_str())->SetFloat(kTileValue_id, kConfigurationMenu_ModListItem);
	}

	/*
	for (const auto& setting : ConfigurationMenu::GetSingleton()->g_Settings)
	{
		auto subSetting = new SM_Setting(setting->name.c_str(), setting->description.c_str(), setting->id.c_str(), "");

		ConfigurationMenu::GetSingleton()->subSettingsListBox.Insert(subSetting);
	}

	
	for (const auto& setting : g_Settings)
	{
		auto subSetting = new SM_Setting(setting->name.c_str(), setting->description.c_str(), setting->id.c_str(), "");

		for (const auto& mod : ConfigurationMenu::GetSingleton()->tweaksListBox.list)
		{
			mod->object->AddSubsetting(subSetting);
		}
	}
	*/
	/*
	 * 
	auto category = elem.contains("category") ? elem["category"].get<std::string>() : "Other";
	menu->allCategories.insert((char*)category.c_str());

	SubSettingData::ElementType type = SubSettingData::kSettingType_Boolean;

	if (e.contains("type"))
	{
		auto typeStr = e["type"].get<std::string>();
		type = SubSettingData::StrToType(typeStr[0]);
	}

	auto name = e.contains("name") ? e["name"].get<std::string>() : "";
	id = e.contains("id") ? e["id"].get<std::string>() : "";
	description = e.contains("description") ? e["description"].get<std::string>() : "";
	auto id = e.contains("id") ? e["id"].get<std::string>() : "";

	auto subSetting = new SM_Setting(name.c_str(), description.c_str(), id.c_str(), id.c_str());

	subSetting->data.SetType(type);

	stewMenuItem->AddSubsetting(subSetting);
	 */

}
