#include "ConfigurationMenu.h"

#include "json.h"

class TagJSON : public CMTag { public: TagJSON(const nlohmann::basic_json<>& elem); };
class SettingJSON : public CMSetting { public: SettingJSON(const nlohmann::basic_json<>& elem); };


template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
	return set;
}

class CMValueJSON : public SM_Value { public : CMValueJSON(const nlohmann::basic_json<>& elem) : SM_Value()
{
	if (elem.is_number_integer() || elem.is_number_unsigned())
		Set(static_cast<SInt64>(elem.get<SInt32>()));;
	if (elem.is_string())
		Set(elem.get<std::string>());
	if (elem.is_number_float()) 
		Set(elem.get<Float64>());
} };

class CMSettingIOJSON : public CMSetting::IO { public: CMSettingIOJSON(const nlohmann::basic_json<>& elem)
{
	std::filesystem::path pathGot;
	std::string categoryGot, valueGot;

	if (elem.contains("path"))		pathGot = elem["path"].get<std::string>();
	if (elem.contains("category"))	categoryGot = elem["category"].get<std::string>();
	if (elem.contains("value"))		valueGot = elem["value"].get<std::string>();

	ini = std::make_tuple(pathGot, categoryGot, valueGot);

	if (elem.contains("default"))		defaultValue = CMValueJSON(elem["default"]);
	if (elem.contains("xml"))			xml = elem["xml"].get<std::string>();
	if (elem.contains("global"))		global = elem["global"].get<std::string>();
	if (elem.contains("gamesetting"))	gamesetting = elem["gamesetting"].get<std::string>();
	if (elem.contains("gameini"))		gameini = elem["gameini"].get<std::string>();
} };

class SettingNoneJSON : public CMSetting::None { public: SettingNoneJSON() = default; };

class SettingCategoryJSON : public CMSetting::Category { public: SettingCategoryJSON(const nlohmann::basic_json<>& elem)
{
	if (elem.contains("mod"))			mod = elem["mod"].get<std::string>();
	if (elem.contains("plugin"))		plugin = elem["plugin"].get<std::string>();
	if (elem.contains("category"))		category = elem["category"].get<std::string>();

	if (elem.contains("doublestacked"))	doublestacked = elem["doublestacked"].get<UInt32>();
	if (elem.contains("allTag"))		allTag = elem["allTag"].get<UInt32>();
}};

class SettingSliderJSON : public CMSetting::Slider { public: SettingSliderJSON(const nlohmann::basic_json<>& elem)
{
	setting = CMSettingIOJSON(elem);

	if (elem.contains("min")) min = CMValueJSON(elem["min"]);
	else min = (SInt32)MININT32;
	if (elem.contains("max")) max = CMValueJSON(elem["max"]);
	else max = (SInt32)MAXINT32;
	if (elem.contains("delta")) delta = CMValueJSON(elem["delta"]);
	else delta = (SInt32)1;
} };

class SettingChoiceJSON : public CMSetting::Choice { public: SettingChoiceJSON(const nlohmann::basic_json<>& elem)
{
	setting = CMSettingIOJSON(elem);

	SInt32 valueLess = 0;
	if (!elem.contains("values")) {}
	else for (const auto& i : elem["values"])
	{
		SM_Value value;
		std::string name;
		std::string description;

		if (i.contains("value")) value = CMValueJSON(i["value"]);
		else value = (SInt32) valueLess++;
		if (i.contains("id")) name = i["id"].get<std::string>();
		else name = (value).GetAsString();
		if (i.contains("description")) description = i["description"].get<std::string>();

		choice.emplace(value, name);
	}
}};

class SettingControlJSON : public CMSetting::Control { public: SettingControlJSON(const nlohmann::basic_json<>& elem)
{
	keyboard = CMSettingIOJSON(elem["keyboard"]);
	mouse = CMSettingIOJSON(elem["mouse"]);
	controller = CMSettingIOJSON(elem["controller"]);
} };

class SettingFontJSON : public CMSetting::Font { public: SettingFontJSON(const nlohmann::basic_json<>& elem)
{
	font = CMSettingIOJSON(elem);
	fontY = CMSettingIOJSON(elem["fontY"]);
}};

class SettingInputJSON : public CMSetting::Input { public: SettingInputJSON(const nlohmann::basic_json<>& elem); };

TagJSON::TagJSON(const nlohmann::basic_json<>& elem)
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();
}

SettingJSON::SettingJSON(const nlohmann::basic_json<>& elem)
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("shortName"))		nameShort = elem["shortName"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();

	if (elem.contains("tags"))			tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));
	if (elem.contains("categories"))	mods.insert_range(GetSetFromElement<std::string>(elem["categories"]));

	if (elem.contains("category"))		data = std::make_unique<Category>(SettingCategoryJSON(elem["category"]));
	else if (elem.contains("choice"))	data = std::make_unique<Choice>(SettingChoiceJSON(elem["choice"]));
	else if (elem.contains("slider"))	data = std::make_unique<Slider>(SettingSliderJSON(elem["slider"]));
	else if (elem.contains("control"))	data = std::make_unique<Control>(SettingControlJSON(elem["control"]));
	else if (elem.contains("font"))		data = std::make_unique<Font>(SettingFontJSON(elem["font"]));
	else								data = std::make_unique<None>(SettingNoneJSON());

	if (id.empty())
	{
		for (const auto mod : mods) id += mod;
		id += data->GetTypeName();
		id += R"(=')" + name + R"(')";
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

void ModConfigurationMenu::SaveModJSON(CMSetting* mod)
{
	nlohmann::basic_json<> j;
	const std::filesystem::path& path = GetCurPath() + R"(\Data\Config\ConfigurationMenu\)" + mod->GetID() + ".json";
	std::ofstream i(path);

	for (const auto& setting : GetSettingsForString(((CMSetting::Category*) mod->data.get())->category))
	{
		j[setting->GetID()].clear();
		for (const auto& value : setting->GetValues())
			if (value.IsString())
				j[setting->GetID()].push_back(value.GetAsString());
			else if (value.IsFloat())
				j[setting->GetID()].push_back(value.GetAsFloat());
			else
				j[setting->GetID()].push_back(value.GetAsInteger());
	}


	i << j;
	i.close();
}

void ModConfigurationMenu::LoadModJSON(CMSetting* mod)
{
	nlohmann::basic_json<> j;
	const std::filesystem::path& path = GetCurPath() + R"(\Data\Config\ConfigurationMenu\)" + mod->GetID() + ".json";
	std::ifstream i(path);

	i >> j;

	for (const auto& setting : g_Settings)
	{
		std::vector<SM_Value> vector;
		for (const auto& iter : j[setting->GetID()])
			vector.push_back(CMValueJSON(iter));
		setting->SetValues(vector);
	}
}
