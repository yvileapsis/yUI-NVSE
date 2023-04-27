#include "ConfigurationMenu.h"

#include "json.h"

class CMJSONElem : public nlohmann::basic_json<> {};

template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
	return set;
}

CMValue::CMValue(const CMJSONElem& elem)
{
	if (elem.is_number_integer() || elem.is_number_unsigned())
		Set(static_cast<SInt64>(elem.get<SInt32>()));;
	if (elem.is_string())
		Set(elem.get<std::string>());
	if (elem.is_number_float()) 
		Set(elem.get<Float64>());
}

CMSetting::IO::IO(const CMJSONElem& elem)
{
	std::filesystem::path pathGot;
	std::string categoryGot, valueGot;

	if (elem.contains("path"))		pathGot = elem["path"].get<std::string>();
	if (elem.contains("category"))	categoryGot = elem["category"].get<std::string>();
	if (elem.contains("value"))		valueGot = elem["value"].get<std::string>();

	ini = std::make_tuple(pathGot, categoryGot, valueGot);

	if (elem.contains("default"))		defaultValue = CMValue(CMJSONElem(elem["default"]));
	if (elem.contains("xml"))			xml = elem["xml"].get<std::string>();
	if (elem.contains("global"))		global = elem["global"].get<std::string>();
	if (elem.contains("gamesetting"))	gamesetting = elem["gamesetting"].get<std::string>();
	if (elem.contains("gameini"))		gameini = elem["gameini"].get<std::string>();
}


CMObject::CMObject(const CMJSONElem& elem)
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();
	priority = 0;
}

CMTag::CMTag(const CMJSONElem& elem) : CMObject(elem)
{
}

CMCategory::CMCategory(const CMJSONElem& elem) : CMObject(elem)
{
	if (elem.contains("shortName"))		shortName = elem["shortName"].get<std::string>();

	if (elem.contains("mod"))			mod = elem["mod"].get<std::string>();
	if (elem.contains("plugin"))		plugin = elem["plugin"].get<std::string>();
	if (elem.contains("doublestacked"))	doublestacked = elem["doublestacked"].get<UInt32>();
	if (elem.contains("allTag"))		allTag = elem["allTag"].get<UInt32>();

}

CMSetting::CMSetting(const CMJSONElem& elem) : CMObject(elem)
{
	if (elem.contains("shortName"))		shortName = elem["shortName"].get<std::string>();

	if (elem.contains("tags"))			tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));
	if (elem.contains("categories"))	mods.insert_range(GetSetFromElement<std::string>(elem["categories"]));
}

void CMSetting::SetID()
{
	if (id.empty())
	{
		for (const auto mod : mods) id += mod;
		id += GetTypeName();
		id += R"(=')" + name + R"(')";
	}
}

Category::Category(const CMJSONElem& elem) : CMSetting(elem)
{
	categoryID = elem["category"].get<std::string>();
}

Slider::Slider(const CMJSONElem& elem) : CMSetting(elem)
{
	const auto& subElem = elem["slider"];
	setting = IO(CMJSONElem(subElem));

	if (subElem.contains("min")) min = CMValue(CMJSONElem(subElem["min"]));
	else min = (SInt32)MININT32;
	if (subElem.contains("max")) max = CMValue(CMJSONElem(subElem["max"]));
	else max = (SInt32)MAXINT32;
	if (subElem.contains("delta")) delta = CMValue(CMJSONElem(subElem["delta"]));
	else delta = (SInt32)1;
}

Choice::Choice(const CMJSONElem& elem) : CMSetting(elem)
{
	const auto& subElem = elem["choice"];
	setting = IO(CMJSONElem(subElem));

	SInt32 valueLess = 0;
	if (!subElem.contains("values")) {}
	else for (const auto& i : subElem["values"])
	{
		CMValue value;
		std::string name;
		std::string description;

		if (i.contains("value")) value = CMValue(CMJSONElem(i["value"]));
		else value = (SInt32)valueLess++;
		if (i.contains("id")) name = i["id"].get<std::string>();
		else name = (value).GetAsString();
		if (i.contains("description")) description = i["description"].get<std::string>();

		choice.emplace(value, name);
	}
}

Control::Control(const CMJSONElem& elem) : CMSetting(elem)
{
	const auto& subElem = elem["control"];
	keyboard = IO(CMJSONElem(subElem["keyboard"]));
	mouse = IO(CMJSONElem(subElem["mouse"]));
	controller = IO(CMJSONElem(subElem["controller"]));
}

Font::Font(const CMJSONElem& elem) : CMSetting(elem)
{
	const auto& subElem = elem["font"];
	font = IO(CMJSONElem(subElem));
	fontY = IO(CMJSONElem(subElem["fontY"]));
}

Input::Input(const CMJSONElem& elem) : CMSetting(elem), setting(elem)
{
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
			const auto tag = CMTag(CMJSONElem(elem));
			mapTags.emplace(tag.id, std::make_unique<CMTag>(tag));
		}

		if (!j.contains("categories") || !j["categories"].is_array())
			Log() << "JSON message: category array not detected in " + path.string();
		else for (const auto& elem : j["categories"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
		{
			const auto tag = CMCategory(CMJSONElem(elem));
			mapCategories.emplace(tag.id, std::make_unique<CMCategory>(tag));
		}


		if (!j.contains("settings") || !j["settings"].is_array())
			Log() << "JSON message: ySI tab array not detected in " + path.string();
		else for (const auto& elem : j["settings"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
		{
			std::unique_ptr<CMSetting> setting;

			if (elem.contains("category"))		setting = std::make_unique<Category>(CMJSONElem(elem));
			else if (elem.contains("choice"))	setting = std::make_unique<Choice>(CMJSONElem(elem));
			else if (elem.contains("slider"))	setting = std::make_unique<Slider>(CMJSONElem(elem));
			else if (elem.contains("control"))	setting = std::make_unique<Control>(CMJSONElem(elem));
			else if (elem.contains("font"))		setting = std::make_unique<Font>(CMJSONElem(elem));
			else								setting = std::make_unique<CMSetting>(CMJSONElem(elem));

			setSettings.emplace(std::move(setting));
		}

		for (const auto& iter : setSettings) iter->SetID();


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

	for (const auto& setting : settingsMain.listBox.list)
	{
		j[setting->object->GetID()].clear();
		for (const auto& value : setting->object->GetValues())
			if (value.IsString())
				j[setting->object->GetID()].push_back(value.GetAsString());
			else if (value.IsFloat())
				j[setting->object->GetID()].push_back(value.GetAsFloat());
			else
				j[setting->object->GetID()].push_back(value.GetAsInteger());
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

	for (const auto& setting : setSettings)
	{
		std::vector<CMValue> vector;
		for (const auto& iter : j[setting->GetID()])
			vector.push_back(CMValue(CMJSONElem(iter)));
		setting->SetValues(vector);
	}
}