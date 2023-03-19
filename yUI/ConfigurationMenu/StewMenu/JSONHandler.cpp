#include "ConfigurationMenuStew.h"
#include "json.h"

class TagJSON : public SM_Tag
{
public:
	TagJSON(const nlohmann::basic_json<>& elem);
};

class ModJSON : public SM_Mod
{
public:
	ModJSON(const nlohmann::basic_json<>& elem);
};

class SettingJSON : public SM_Setting
{
public:
	SettingJSON(const nlohmann::basic_json<>& elem);
};

template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
	return set;
}

TagJSON::TagJSON(const nlohmann::basic_json<>& elem)
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();
}

ModJSON::ModJSON(const nlohmann::basic_json<>& elem)
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();

	if (elem.contains("version"))		version = elem["version"].get<std::string>();

	if (elem.contains("tags"))			tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));
}

SettingJSON::SettingJSON(const nlohmann::basic_json<>& elem) : SM_Setting()
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();

	if (elem.contains("tags"))			tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));
	if (elem.contains("mods"))			mods.insert_range(GetSetFromElement<std::string>(elem["mods"]));

	if (!elem.contains("type"))			type = kSettingType_None;
	else
	{
		const auto typeString = elem["type"].get<std::string>();
		if (typeString == "choice")			type = kSettingType_ChoiceInteger;
		else if (typeString == "control")	type = kSettingType_Control;
		else if (typeString == "integer")	type = kSettingType_SliderInteger;
		else if (typeString == "float")		type = kSettingType_SliderFloat;
	}

	if (type == kSettingType_ChoiceInteger)
	{
		SInt32 valueLess = 0;
		if (!elem.contains("values")) Log(true) << "WAAAAAAAAA, broke";
		else for (const auto& i : elem["values"])
		{
			SInt32 value;
			std::string name;
			std::string description;

			if (i.contains("value")) value = i["value"].get<SInt32>();
			else value = valueLess++;
			if (i.contains("name")) name = i["name"].get<std::string>();
			else name = std::to_string(value);
			if (i.contains("description")) description = i["description"].get<std::string>();

			values.emplace(value, std::make_tuple(name, description));
		}
	}
}

void ModConfigurationMenu::ReadJSON(const std::filesystem::path& path)
{
	Log() << "\nJSON message: reading  " + path.string();
	try
	{
		std::ifstream i(path);
		auto j = nlohmann::json::parse(i, nullptr, true, true);

		if (!j.contains("tags") || !j["tags"].is_array())
			Log() << "JSON message: ySI item array not detected in " + path.string();
		else for (const auto& elem : j["tags"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
			g_Tags.push_back(std::make_unique<SM_Tag>(TagJSON(elem)));

		if (!j.contains("mods") || !j["mods"].is_array())
			Log() << "JSON message: ySI category array not detected in " + path.string();
		else for (const auto& elem : j["mods"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else 
			g_Mods.push_back(std::make_unique<SM_Mod>(ModJSON(elem)));

		if (!j.contains("settings") || !j["settings"].is_array())
			Log() << "JSON message: ySI tab array not detected in " + path.string();
		else for (const auto& elem : j["settings"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
			g_Settings.push_back(std::make_unique<SM_Setting>(SettingJSON(elem)));
	}
	catch (nlohmann::json::exception& e)
	{
		Log() << "JSON error: JSON file is incorrectly formatted! It will not be applied. " + path.string();
		Log() << FormatString("JSON error: %s\n", e.what());
	}
}

void ModConfigurationMenu::ReadJSONForPath(const std::filesystem::path& path)
{
	for (auto& iter : std::filesystem::directory_iterator(path))
	{
		if (iter.is_directory()) continue;
		if (iter.path().extension().string() == ".json") ReadJSON(iter.path());
	}
}