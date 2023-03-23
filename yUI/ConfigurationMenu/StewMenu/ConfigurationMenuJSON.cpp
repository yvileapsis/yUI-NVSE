#include "ConfigurationMenuStew.h"
#include "json.h"

class TagJSON : public SM_Tag { public: TagJSON(const nlohmann::basic_json<>& elem); };
class ModJSON : public SM_Mod { public: ModJSON(const nlohmann::basic_json<>& elem); };
class SettingJSON : public SM_Setting { public: SettingJSON(const nlohmann::basic_json<>& elem); };

template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
	return set;
}

SM_Value GetValueFromElement(const nlohmann::basic_json<>& elem)
{
	if (elem.is_number_integer() || elem.is_number_unsigned()) return elem.get<SInt32>();
	if (elem.is_string()) return elem.get<std::string>();
	if (elem.is_number_float()) return elem.get<Float64>();
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

SettingJSON::SettingJSON(const nlohmann::basic_json<>& elem)
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
		if (typeString == "choice")				type = kSettingType_Choice;
		else if (typeString == "control")		type = kSettingType_Control;
		else if (typeString == "slider")		type = kSettingType_Slider;
		else if (typeString == "subsetting")	type = kSettingType_Subsetting;
		else if (typeString == "font")			type = kSettingType_Font;
		else									type = kSettingType_None;
	}

	std::filesystem::path path;
	std::string category, value;
	if (elem.contains("path"))		path = elem["path"].get<std::string>();
	if (elem.contains("category"))	category = elem["category"].get<std::string>();
	if (elem.contains("value"))		value = elem["value"].get<std::string>();
	setting = std::make_tuple(path, category, value);

	if (elem.contains("valueDefault")) valueDefault = GetValueFromElement(elem["valueDefault"]);

	if (type == kSettingType_Choice)
	{
		SInt32 valueLess = 0;
		if (!elem.contains("values")) Log(true) << "WAAAAAAAAA, broke";
		else for (const auto& i : elem["values"])
		{
			SM_Value value;
			std::string name;
			std::string description;

			if (i.contains("value")) value = GetValueFromElement(i["value"]);
			else value = valueLess++;
			if (i.contains("name")) name = i["name"].get<std::string>();
			else name = GetStringFromValue(value);
			if (i.contains("description")) description = i["description"].get<std::string>();

			choice.emplace(value, std::make_pair(name, description));
		}
	}
	else if (type == kSettingType_Slider)
	{
		SM_Value valueMin, valueMax, valueDelta;

		if (elem.contains("valueMin")) valueMin = GetValueFromElement(elem["valueMin"]);
		else valueMin = MININT32;
		if (elem.contains("valueMax")) valueMax = GetValueFromElement(elem["valueMax"]);
		else valueMax = MAXINT32;
		if (elem.contains("valueDelta")) valueDelta = GetValueFromElement(elem["valueDelta"]);
		else valueMin = 1;

		slider = std::make_tuple(valueMin, valueMax, valueDelta);
	}
	else if (type == kSettingType_Control)
	{
		std::filesystem::path mousePath;
		std::string mouseCategory, mouseValue;
		if (elem.contains("mousePath"))			mousePath = elem["mousePath"].get<std::string>();
		if (elem.contains("mouseCategory"))		mouseCategory = elem["mouseCategory"].get<std::string>();
		if (elem.contains("mouseValue"))		mouseValue = elem["mouseValue"].get<std::string>();

		SM_Value mouseValueDefault;
		if (elem.contains("mouseValueDefault")) mouseValueDefault = GetValueFromElement(elem["mouseValueDefault"]);

		control[0] = std::make_pair(std::make_tuple(mousePath, mouseCategory, mouseValue), mouseValueDefault);

		std::filesystem::path controllerPath;
		std::string controllerCategory, controllerValue;
		if (elem.contains("controllerPath"))		controllerPath = elem["controllerPath"].get<std::string>();
		if (elem.contains("controllerCategory"))	controllerCategory = elem["controllerCategory"].get<std::string>();
		if (elem.contains("controllerValue"))		controllerValue = elem["controllerValue"].get<std::string>();

		SM_Value controllerValueDefault;
		if (elem.contains("controllerValueDefault")) controllerValueDefault = GetValueFromElement(elem["controllerValueDefault"]);

		control[1] = std::make_pair(std::make_tuple(controllerPath, controllerCategory, controllerValue), controllerValueDefault);
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