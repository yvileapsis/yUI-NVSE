#include "ConfigurationMenu.h"

#include "json.h"

class JSON : public nlohmann::basic_json<> { friend nlohmann::basic_json<>; };

template <typename T> std::vector<T> GetSetFromElement(const nlohmann::basic_json<>& elem)
{
	std::vector<T> set{};
	if (!elem.is_array()) set.push_back(elem.get<T>());
	else for (const auto& i : elem) set.push_back(i.get<T>());
	return set;
}

CMValue::CMValue(const JSON& elem)
{
	if (elem.is_number_integer() || elem.is_number_unsigned())
		Set(static_cast<SInt64>(elem.get<SInt32>()));;
	if (elem.is_string())
		Set(elem.get<std::string>());
	if (elem.is_number_float()) 
		Set(elem.get<Float64>());
}

CMSetting::IO::IO(const JSON& elem)
{
	std::filesystem::path pathGot;
	std::string categoryGot, valueGot;

	if (elem.contains("path"))		pathGot = elem["path"].get<std::string>();
	if (elem.contains("category"))	categoryGot = elem["category"].get<std::string>();
	if (elem.contains("value"))		valueGot = elem["value"].get<std::string>();

	ini = INI(pathGot, categoryGot, valueGot);

	if (elem.contains("default"))		defaultValue = CMValue(JSON(elem["default"]));
	if (elem.contains("xml"))			xml = elem["xml"].get<std::string>();
	if (elem.contains("global"))		global = elem["global"].get<std::string>();
	if (elem.contains("gamesetting"))	gamesetting = elem["gamesetting"].get<std::string>();
	if (elem.contains("gameini"))		gameini = elem["gameini"].get<std::string>();
}


CMObject::CMObject(const JSON& elem)
{
	if (elem.contains("id"))			id = elem["id"].get<std::string>();
	if (elem.contains("name"))			name = elem["name"].get<std::string>();
	if (elem.contains("description"))	description = elem["description"].get<std::string>();
	priority = 0;
}

CMTag::CMTag(const JSON& elem) : CMObject(elem)
{
}

CMCategory::CMCategory(const JSON& elem) : CMObject(elem), compatibilityMode(false)
{
	if (elem.contains("shortName")) shortName = elem["shortName"].get<std::string>();

	if (elem.contains("mod")) mod = elem["mod"].get<std::string>();
	if (elem.contains("plugin")) plugin = elem["plugin"].get<std::string>();
	if (elem.contains("doublestacked")) doublestacked = elem["doublestacked"].get<UInt32>();
	if (elem.contains("allTag")) allTag = elem["allTag"].get<UInt32>();
}

CMSetting::CMSetting(const JSON& elem) : CMObject(elem)
{
	if (elem.contains("shortName"))		shortName = elem["shortName"].get<std::string>();

	if (elem.contains("tags"))			tags.insert_range(GetSetFromElement<std::string>(elem["tags"]));
	if (elem.contains("categories"))	mods.insert_range(GetSetFromElement<std::string>(elem["categories"]));
}

CMSettingCategory::CMSettingCategory(const JSON& elem) : CMSetting(elem)
{
	categoryID = elem["category"].get<std::string>();
}

CMSettingSlider::CMSettingSlider(const JSON& elem) : CMSetting(elem)
{
	const auto& subElem = elem["slider"];
	setting = IO(JSON(subElem));

	if (subElem.contains("min")) min = CMValue(JSON(subElem["min"]));
	else min = (SInt32)MININT32;
	if (subElem.contains("max")) max = CMValue(JSON(subElem["max"]));
	else max = (SInt32)MAXINT32;
	if (subElem.contains("delta")) delta = CMValue(JSON(subElem["delta"]));
	else delta = (SInt32)1;
}

CMSettingChoice::CMSettingChoice(const JSON& elem) : CMSetting(elem)
{
	const auto& subElem = elem["choice"];
	setting = IO(JSON(subElem));

	SInt32 valueLess = 0;
	if (!subElem.contains("values")) {}
	else for (const auto& i : subElem["values"])
	{
		CMValue value;
		std::string name;
		std::string description;

		if (i.contains("value")) value = CMValue(JSON(i["value"]));
		else value = (SInt32)valueLess++;
		if (i.contains("id")) name = i["id"].get<std::string>();
		else name = (value).GetAsString();
		if (i.contains("description")) description = i["description"].get<std::string>();

		choice.emplace(value, name);
	}
}

CMSettingControl::CMSettingControl(const JSON& elem) : CMSetting(elem)
{
	const auto& subElem = elem["control"];
	keyboard = IO(JSON(subElem["keyboard"]));
	mouse = IO(JSON(subElem["mouse"]));
	controller = IO(JSON(subElem["controller"]));
}

CMSettingFont::CMSettingFont(const JSON& elem) : CMSetting(elem)
{
	const auto& subElem = elem["font"];
	font = IO(JSON(subElem));
	fontY = IO(JSON(subElem["fontY"]));
}

CMSettingInput::CMSettingInput(const JSON& elem) : CMSetting(elem), setting(elem)
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
			const auto tag = CMTag(JSON(elem));
			mapTags.emplace(tag.id, std::make_unique<CMTag>(tag));
		}

		if (!j.contains("categories") || !j["categories"].is_array())
			Log() << "JSON message: category array not detected in " + path.string();
		else for (const auto& elem : j["categories"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
		{
			const auto tag = CMCategory(JSON(elem));
			mapCategories.emplace(tag.id, std::make_unique<CMCategory>(tag));
		}


		if (!j.contains("settings") || !j["settings"].is_array())
			Log() << "JSON message: ySI tab array not detected in " + path.string();
		else for (const auto& elem : j["settings"]) if (!elem.is_object())
			Log() << "JSON error: Expected object";
		else
		{
			std::unique_ptr<CMSetting> setting;

			if (elem.contains("category"))		setting = std::make_unique<CMSettingCategory>(JSON(elem));
			else if (elem.contains("choice"))	setting = std::make_unique<CMSettingChoice>(JSON(elem));
			else if (elem.contains("slider"))	setting = std::make_unique<CMSettingSlider>(JSON(elem));
			else if (elem.contains("control"))	setting = std::make_unique<CMSettingControl>(JSON(elem));
			else if (elem.contains("font"))		setting = std::make_unique<CMSettingFont>(JSON(elem));
			else								setting = std::make_unique<CMSetting>(JSON(elem));

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

void ModConfigurationMenu::SaveModJSON(std::string mod)
{
	nlohmann::basic_json<> j{};
	const std::filesystem::path& path = GetCurPath() + R"(\Data\Config\ConfigurationMenu\)" + mod + ".json";
	std::ofstream i(path);

	j.clear();

	for (const auto& setting : settingsMain.listBox.list)
	{
		setting->object->SaveJSON(reinterpret_cast<JSON&>(j));
	}

	i << j;
	i.close();
}

void ModConfigurationMenu::LoadModJSON(std::string mod)
{
	nlohmann::basic_json<> j;
	const std::filesystem::path& path = GetCurPath() + R"(\Data\Config\ConfigurationMenu\)" + mod + ".json";
	std::ifstream i(path);

	i >> j;

	for (const auto& setting : settingsMain.listBox.list)
	{
		setting->object->LoadJSON(JSON(j));
	}

	/*
	for (const auto& setting : setSettings)
	{
		std::vector<CMValue> vector;
		for (const auto& iter : j[setting->GetID()])
			vector.push_back(CMValue(JSON(iter)));
		setting->SetValues(vector);
	}*/
}

void CMSettingCategory::SaveJSON(JSON& elem)
{
	for (const auto setting : ModConfigurationMenu::GetSingleton()->GetSettingsForString(categoryID))
		setting->SaveJSON(reinterpret_cast<JSON&>(elem[GetID()]));
}


void CMSettingCategory::LoadJSON(const JSON& elem)
{
	for (const auto setting : ModConfigurationMenu::GetSingleton()->GetSettingsForString(categoryID))
		setting->LoadJSON(JSON(elem[GetID()]));
}

void CMSettingChoice::SaveJSON(JSON& elem)
{
	const auto value = setting.Read();

	if (value.IsString())
		elem[GetID()] = value.GetAsString();
	else if (value.IsFloat())
		elem[GetID()] = value.GetAsFloat();
	else
		elem[GetID()] = value.GetAsInteger();
	
}

void CMSettingChoice::LoadJSON(const JSON& elem)
{
	setting.Write(CMValue(JSON(elem[GetID()])));
}

void CMSettingSlider::SaveJSON(JSON& elem)
{
	const auto value = setting.Read();

	if (value.IsString())
		elem[GetID()] = value.GetAsString();
	else if (value.IsFloat())
		elem[GetID()] = value.GetAsFloat();
	else
		elem[GetID()] = value.GetAsInteger();
}

void CMSettingSlider::LoadJSON(const JSON& elem)
{
	setting.Write(CMValue(JSON(elem[GetID()])));
}

void CMSettingControl::SaveJSON(JSON& elem)
{
	const auto keyboardValue = keyboard.Read();

	if (keyboardValue.IsString())
		elem[GetID()]["keyboard"] = keyboardValue.GetAsString();
	else if (keyboardValue.IsFloat())
		elem[GetID()]["keyboard"] = keyboardValue.GetAsFloat();
	else
		elem[GetID()]["keyboard"] = keyboardValue.GetAsInteger();

	const auto mouseValue = mouse.Read();

	if (mouseValue.IsString())
		elem[GetID()]["mouse"] = mouseValue.GetAsString();
	else if (mouseValue.IsFloat())
		elem[GetID()]["mouse"] = mouseValue.GetAsFloat();
	else
		elem[GetID()]["mouse"] = mouseValue.GetAsInteger();

	const auto controllerValue = controller.Read();

	if (controllerValue.IsString())
		elem[GetID()]["controller"] = controllerValue.GetAsString();
	else if (controllerValue.IsFloat())
		elem[GetID()]["controller"] = controllerValue.GetAsFloat();
	else
		elem[GetID()]["controller"] = controllerValue.GetAsInteger();
}

void CMSettingControl::LoadJSON(const JSON& elem)
{
	keyboard.Write(CMValue(JSON(elem[GetID()]["keyboard"])));
	mouse.Write(CMValue(JSON(elem[GetID()]["mouse"])));
	controller.Write(CMValue(JSON(elem[GetID()]["controller"])));
}

void CMSettingFont::SaveJSON(JSON& elem)
{
	const auto fontValue = font.Read();

	if (fontValue.IsString())
		elem[GetID()]["font"] = fontValue.GetAsString();
	else if (fontValue.IsFloat())
		elem[GetID()]["font"] = fontValue.GetAsFloat();
	else
		elem[GetID()]["font"] = fontValue.GetAsInteger();

	const auto fontYValue = fontY.Read();

	if (fontYValue.IsString())
		elem[GetID()]["fontY"] = fontYValue.GetAsString();
	else if (fontYValue.IsFloat())
		elem[GetID()]["fontY"] = fontYValue.GetAsFloat();
	else
		elem[GetID()]["fontY"] = fontYValue.GetAsInteger();
}

void CMSettingFont::LoadJSON(const JSON& elem)
{
	font.Write(CMValue(JSON(elem[GetID()]["font"])));
	fontY.Write(CMValue(JSON(elem[GetID()]["fontY"])));
}