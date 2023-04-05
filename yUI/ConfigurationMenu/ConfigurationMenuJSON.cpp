#include "ConfigurationMenu.h"
#include "json.h"

class TagJSON : public CMTag { public: TagJSON(const nlohmann::basic_json<>& elem); };
class ModJSON : public CMMod { public: ModJSON(const nlohmann::basic_json<>& elem); };
class SettingJSON : public CMSetting { public: SettingJSON(const nlohmann::basic_json<>& elem); };

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
	return static_cast<SInt32>(0);
}

CMSetting::SettingSource GetSourceFromElement(const nlohmann::basic_json<>& elem, const std::string& path = "path",
                                               const std::string& category = "category", const std::string&
                                               value = "value", const std::string& valueDefault = "valueDefault")
{
	CMSetting::SettingSource source{};

	std::filesystem::path pathGot;
	std::string categoryGot, valueGot;

	if (elem.contains(path))		pathGot = elem[path].get<std::string>();
	if (elem.contains(category))	categoryGot = elem[category].get<std::string>();
	if (elem.contains(value))		valueGot = elem[value].get<std::string>();

	source.setting = std::make_tuple(pathGot, categoryGot, valueGot);

	if (elem.contains(valueDefault)) source.defaultValue = GetValueFromElement(elem[valueDefault]);

	return source;
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

	if (type == kSettingType_Choice)
	{
		Choice choice{};

		choice.setting = GetSourceFromElement(elem);

		SInt32 valueLess = 0;
		if (!elem.contains("values")) {}
		else for (const auto& i : elem["values"])
		{
			SM_Value value;
			std::string name;
			std::string description;

			if (i.contains("value")) value = GetValueFromElement(i["value"]);
			else value = valueLess++;
			if (i.contains("name")) name = i["name"].get<std::string>();
			else name = (value).GetAsString();
			if (i.contains("description")) description = i["description"].get<std::string>();

			choice.choice.emplace(value, std::make_pair(name, description));
		}

		data = std::make_unique<Choice>(choice);
	}
	else if (type == kSettingType_Slider)
	{
		Slider slider{};

		SM_Value valueMin, valueMax, valueDelta;

		slider.setting = GetSourceFromElement(elem);

		if (elem.contains("valueMin")) valueMin = GetValueFromElement(elem["valueMin"]);
		else valueMin = (SInt32) MININT32;
		if (elem.contains("valueMax")) valueMax = GetValueFromElement(elem["valueMax"]);
		else valueMax = (SInt32)MAXINT32;
		if (elem.contains("valueDelta")) valueDelta = GetValueFromElement(elem["valueDelta"]);
		else valueMin = (SInt32) 1;

		slider.slider = std::make_tuple(valueMin, valueMax, valueDelta);

		data = std::make_unique<Slider>(slider);
	}
	else if (type == kSettingType_Control)
	{
		Control control{};

		control.keyboard = GetSourceFromElement(elem);
		control.mouse = GetSourceFromElement(elem, "mousePath", "mouseCategory", "mouseValue", "mouseValueDefault");
		control.controller = GetSourceFromElement(elem, "controllerPath", "controllerCategory", "controllerValue", "controllerValueDefault");

		data = std::make_unique<Control>(control);
	}
	else if (type == kSettingType_Font)
	{
		Font font{};

		font.font = GetSourceFromElement(elem);
		font.fontY = GetSourceFromElement(elem, "fontYPath", "fontYCategory", "fontYValue", "fontYDefault");

		data = std::make_unique<Font>(font);
	}
	else
	{
		None none{};

		data = std::make_unique<None>(none);
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
		{
			const auto tag = TagJSON(elem);
			g_Tags.emplace(tag.id, std::make_unique<CMTag>(tag));
		}

		if (!j.contains("mods") || !j["mods"].is_array())
			Log() << "JSON message: ySI category array not detected in " + path.string();
		else for (const auto& elem : j["mods"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else 
			g_Mods.push_back(std::make_unique<CMMod>(ModJSON(elem)));

		if (!j.contains("settings") || !j["settings"].is_array())
			Log() << "JSON message: ySI tab array not detected in " + path.string();
		else for (const auto& elem : j["settings"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
			g_Settings.push_back(std::make_unique<CMSetting>(SettingJSON(elem)));
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