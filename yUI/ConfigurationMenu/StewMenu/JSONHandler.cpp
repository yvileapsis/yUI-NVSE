#include "JSONHandler.h"
#include "StewMenu.h"
#include "json.h"

class Tag
{
public:
	std::string id;
	std::string name;
	std::string description;
};

class Mod
{
public:
	std::string id;
	std::string name;
	std::string description;
	std::unordered_set<std::string> tags;
	std::string version;
};

class Setting
{
public:
	enum Type
	{
		kNone,
		kChoice,
		kInteger,
		kFloat,
	};

	std::string id;
	std::string name;
	std::string description;
	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;
	std::string condition;
	Type type = kNone;

};

class SettingChoice : public Setting
{
	std::unordered_map<SInt32, std::string> values;
	std::string path;
	SInt32 defaultValue;
};


inline std::vector<std::unique_ptr<Tag>>		g_Tags;
inline std::vector<std::unique_ptr<Mod>>		g_Mods;
inline std::vector<std::unique_ptr<Setting>>	g_Settings;

void HandleTag(nlohmann::basic_json<> elem)
{
	if (!elem.is_object())
	{
		Log() << "JSON error: Expected object";
		return;
	}

	Tag tag{};

	if (elem.contains("id"))			tag.id = elem["id"].get<std::string>();
	if (elem.contains("name"))			tag.name = elem["id"].get<std::string>();
	if (elem.contains("description"))	tag.description = elem["description"].get<std::string>();

	g_Tags.push_back(std::make_unique<Tag>(tag));
}

void HandleMod(nlohmann::basic_json<> elem)
{
	if (!elem.is_object())
	{
		Log() << "JSON error: Expected object";
		return;
	}

	Mod mod{};

	if (elem.contains("id"))			mod.id = elem["id"].get<std::string>();
	if (elem.contains("name"))			mod.name = elem["id"].get<std::string>();
	if (elem.contains("description"))	mod.description = elem["description"].get<std::string>();

	if (elem.contains("version"))		mod.version = elem["version"].get<std::string>();

	g_Mods.push_back(std::make_unique<Mod>(mod));
}

void HandleSetting(nlohmann::basic_json<> elem)
{
	if (!elem.is_object())
	{
		Log() << "JSON error: Expected object";
		return;
	}

	Setting setting{};

	if (elem.contains("id"))			setting.id = elem["id"].get<std::string>();
	if (elem.contains("name"))			setting.name = elem["id"].get<std::string>();
	if (elem.contains("description"))	setting.description = elem["description"].get<std::string>();

	g_Settings.push_back(std::make_unique<Setting>(setting));
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

	for (const auto& mod : g_Mods)
	{
		auto stewMenuItem = new StewMenuItem(mod->name.c_str(), mod->description.c_str(), 0, mod->id.c_str(), "", "");
		StewMenu::GetSingleton()->tweaksListBox.Insert(stewMenuItem, mod->name.c_str())->SetFloat(kTileValue_id, kStewMenu_TweakListItem);
	}

	for (const auto& setting : g_Settings)
	{
		auto subSetting = new StewMenuSubsettingItem(setting->name.c_str(), setting->description.c_str(), setting->id.c_str(), "");

		for (const auto& mod : StewMenu::GetSingleton()->tweaksListBox.list)
		{
			mod->object->AddSubsetting(subSetting);
		}
	}

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
	internalName = e.contains("internalName") ? e["internalName"].get<std::string>() : "";
	description = e.contains("description") ? e["description"].get<std::string>() : "";
	auto internalCategory = e.contains("internalCategory") ? e["internalCategory"].get<std::string>() : "";

	auto subSetting = new StewMenuSubsettingItem(name.c_str(), description.c_str(), internalName.c_str(), internalCategory.c_str());

	subSetting->data.SetType(type);

	stewMenuItem->AddSubsetting(subSetting);
	 */

}
