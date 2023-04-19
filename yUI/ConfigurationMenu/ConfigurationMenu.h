#pragma once

#include "Utilities.h"
#include "Menu.h"

#include <set>
#include <map>
#include <utility>
#include <variant>

class CMTag;
class CMSetting;

inline UInt32 g_LogLevel = 3;
inline UInt32 g_saveValue = 1;

struct InputField
{
	// used for filtering which characters are valid
	enum InputType
	{
		kInputType_String = 0,
		kInputType_Integer,
		kInputType_Float
	};

	Tile* tile;
	std::string input;
	bool isActive;
	bool isCaretShown;
	SInt16 caretIndex;
	UInt32 lastCaretUpdateTime;
	UInt8 inputType;

	void Init();
	void Set(std::string str);

	std::string GetText();
	UInt16 GetLen();
	void UpdateCaretDisplay();
	void SetActive(bool active);
	bool IsKeyValid(UInt32 key);
	bool HandleSpecialInputKey(UInt32 key);
	void HandleKeyboardShortcuts(UInt32 key);
	bool HandleKey(UInt32 key);
	void Update();
	void Free();
};

class CMValue
{
	enum Type
	{
		kNone = 0,
		kInteger = 1,
		kFloat = 2,
		kString = 3
	};

	Type type = kNone;
	SInt64 intval = 0;
	Float64 floatval = 0;
	std::string stringval;

public:
	CMValue() = default;
	CMValue(const SInt32 value) : type(kInteger), intval(value) {}
	explicit CMValue(const UInt32 value) : type(kInteger), intval(value) {}
	CMValue(const Float64 value) : type(kFloat), floatval(value) {}
	CMValue(std::string value) : type(kString), stringval(std::move(value)) {}

	bool IsInteger() const { return type == kInteger; }
	bool IsFloat() const { return type == kFloat; }
	bool IsString() const { return type == kString; }

	SInt32 GetAsInteger() const
	{
		if (IsInteger()) return intval;
		if (IsFloat()) return floatval;
		return 0;
	}

	Float64 GetAsFloat() const
	{
		if (IsInteger()) return intval;
		if (IsFloat()) return floatval;
		return 0;
	}

	std::string GetAsString() const
	{
		if (IsInteger()) return std::to_string(intval);
		if (IsFloat()) return std::to_string(floatval);
		if (IsString()) return stringval;
		return "";
	}

	operator SInt32 () const { return GetAsInteger(); }
	explicit operator Float64 () const { return GetAsFloat(); }
	operator std::string () const { return GetAsString(); }

	CMValue Set(SInt64 value)
	{
		type = kInteger;
		intval = value;
		return GetAsInteger();
	}

	CMValue Set(Float64 value)
	{
		type = kFloat;
		floatval = value;
		return GetAsFloat();
	}

	CMValue Set(std::string value)
	{
		type = kString;
		stringval = value;
		return GetAsString();
	}

	CMValue operator+(const CMValue& right) const
	{
		if (IsString() && right.IsString())
			return GetAsString() + right.GetAsString();
		if (IsInteger() && right.IsInteger())
			return GetAsInteger() + right.GetAsInteger();
		return GetAsFloat() + right.GetAsFloat();
	}

	CMValue operator-(const CMValue& right) const
	{
		if (IsInteger() && right.IsInteger())
			return GetAsInteger() - right.GetAsInteger();
		return GetAsFloat() - right.GetAsFloat();
	}
};

class CMTag
{
public:
	std::string id; // TODO: make ID: type + mods alphab. + type + name
	std::string name;
	std::string nameShort;
	std::string description;

	SInt32 priority;

	CMTag() = default;
	CMTag(std::string id, std::string name, std::string description)
	: id(std::move(id)), name(std::move(name)), description(std::move(description))
	{}

	virtual std::string GetID() { return id; }
	virtual std::string GetName() { return !name.empty() ? name : id; }
	virtual std::string GetDescription() const { return FormatString("%s\n- %s", description.c_str(), " "); }
};

class CMOption : public CMTag
{
public:
};


class CMSetting : public CMTag
{
public:
	class IO
	{
	public:
		typedef std::tuple<std::filesystem::path, std::string, std::string> INI;
		INI ini;

		std::string xml;

		std::string global;

		std::string gamesetting;

		std::string gameini;

		CMValue defaultValue;

		std::optional<CMValue> ReadINI();
		void WriteINI(const CMValue& value) const;

		std::optional<CMValue> ReadXML();
		void WriteXML(const CMValue& value) const;

		std::optional<CMValue> ReadGameSetting();
		void WriteGameSetting(const CMValue& value) const;

		std::optional<CMValue> ReadGameINI();
		void WriteGameINI(const CMValue& value) const;

		std::optional<CMValue> ReadGlobal();
		void WriteGlobal(const CMValue& value) const;

		CMValue Read();
		void Write(const CMValue& value) const;

		void Default();
	};

	class None
	{
	public:
		virtual ~None() = default;

		static const char* GetTemplateAlt() { return "SettingNoneTemplate"; }
		virtual const char* GetTemplate() { return GetTemplateAlt(); }
		virtual const char* GetTypeName() { return "None"; }

		virtual bool IsCategory() { return false; }

		virtual void Display(Tile* tile) {}

		virtual void Next(const bool forward = true) {};
		virtual void Last(const bool forward = true) {};

		virtual void Default() {};

		virtual std::vector<CMValue> GetValues() { return {}; };
		virtual void SetValues(const std::vector<CMValue>& values) {};

		virtual void Click(Tile* tile) {};
	};

	class Category : public None
	{
	public:
		std::string mod;
		std::string plugin;
		bool doublestacked;
		bool allTag;
		std::string category;

		const char* GetTemplate() override { return "SettingSubsettingTemplate"; }
		const char* GetTypeName() override { return "Subsetting"; }

		bool IsCategory() override { return true; }

		void Default() override;

		std::vector<CMValue> GetValues() override;
		void SetValues(const std::vector<CMValue>& values) override {};

		void Click(Tile* tile) override;

	};

	class Choice : public None
	{
	public:
		IO setting;
		std::map<CMValue, std::string> choice;

		const char* GetTemplate() override { return "SettingChoiceTemplate"; }
		const char* GetTypeName() override { return "Choice"; }

		void Display(Tile* tile) override;

		CMValue GetPrev(const CMValue& value);
		CMValue GetNext(const CMValue& value);

		void Next(bool forward) override;
		void Last(bool forward) override;

		void Default() override { setting.Default(); }

		std::vector<CMValue> GetValues() override { return { setting.Read() }; };

		void SetValues(const std::vector<CMValue>& values) override
		{
			if (!values.empty()) setting.Write(values[0]);
		};
	};

	class Slider : public None
	{
	public:
		IO setting;

		CMValue min;
		CMValue max;
		CMValue delta;

		const char* GetTemplate() override { return "SettingSliderTemplate"; }
		const char* GetTypeName() override { return "Slider"; }

		void Display(Tile* tile) override;

		CMValue GetPrev(const CMValue& value) const;
		CMValue GetNext(const CMValue& value) const;

		void Next(bool forward) override;
		void Last(bool forward) override;

		void Default() override { setting.Default(); }

		std::vector<CMValue> GetValues() override { return { setting.Read() }; };
		void SetValues(const std::vector<CMValue>& values) override
		{
			if (!values.empty()) setting.Write(values[0]);
		};
	};

	class Control : public None
	{
	public:
		IO keyboard;
		IO mouse;
		IO controller;

		const char* GetTemplate() override { return "SettingControlTemplate"; }
		const char* GetTypeName() override { return "Control"; }

		void Display(Tile* tile) override;

		void Default() override { keyboard.Default(); mouse.Default(); controller.Default(); }

		std::vector<CMValue> GetValues() override { return { keyboard.Read(), mouse.Read(), controller.Read() }; };

		void SetValues(const std::vector<CMValue>& values) override
		{
			if (values.size() >= 3)
			{
				keyboard.Write(values[0]);
				mouse.Write(values[1]);
				controller.Write(values[2]);
			}
		};
	};

	class Font : public None
	{
	public:
		IO font;
		IO fontY;

		const char* GetTemplate() override { return "SettingFontTemplate"; }
		const char* GetTypeName() override { return "Font"; }

		void Display(Tile* tile) override;

		static CMValue GetPrev(const CMValue& value);
		static CMValue GetNext(const CMValue& value);

		void Next(bool forward) override;
		void Last(bool forward) override;

		void Default() override { font.Default(); fontY.Default(); }

		std::vector<CMValue> GetValues() override { return { font.Read(), fontY.Read() }; };
		void SetValues(const std::vector<CMValue>& values) override
		{
			if (values.size() >= 2)
			{
				font.Write(values[0]);
				fontY.Write(values[1]);
			}
		};
	};

	class Input : public None
	{
	public:
		IO setting;
	};

	enum ElementType
	{
		kSettingType_None,
		kSettingType_Control,
		kSettingType_Choice,
		kSettingType_Slider,
		kSettingType_Font,
		kSettingType_Subsetting,
		kSettingType_Input,
	};

	CMSetting* Next(const bool forward = true) { data->Next(forward); return this; };
	CMSetting* Last(const bool forward = true) { data->Last(forward); return this; }

	CMSetting* Default() { data->Default(); return this; };

	CMSetting* Display(Tile* tile) { data->Display(tile); return this; };
	const char* GetTemplate() const { return data->GetTemplate(); }

	CMSetting* Click(Tile* tile) { data->Click(tile); return this; }

	std::vector<CMValue> GetValues() const { return data->GetValues(); }
	void SetValues(const std::vector<CMValue>& values) const { return data->SetValues(values); }

	std::unique_ptr<None> data;

	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;

	std::string GetDescription() const override
	{
		return description;
	}
};

extern const char* MenuPath;

void WriteValueToINIs(const char* category, const char* name, const char* value);

// randomly selected from the unused menu codes between 1001 and 1084 inclusive (to be compatible with the menu visibility array)
constexpr auto MENU_ID = 1042;

struct HotkeyField
{
	Tile* tile;
	int value;
	bool isActive;
	bool frameDebounce;	// ignore inputs for the frame it's set active to allow clicking on the input with Spacebar/Enter etc.

	void Init();
	int GetPressedKey();
	void SetActive(bool active);
	int Update();
	void Free();
};

class ModConfigurationMenu : public Menu
{
public:
	enum TileIDs
	{
		kModConfigurationMenu_Title = 0,

		kModConfigurationMenu_SelectionText = 4,

		kModConfigurationMenu_SearchBar = 5,
		kModConfigurationMenu_SearchIcon = 6,

		kModConfigurationMenu_Exit = 7,
		kModConfigurationMenu_Back = 8,
		kModConfigurationMenu_DeviceButton = 30,
		kModConfigurationMenu_Default = 31,
		kModConfigurationMenu_SaveToJSON = 32,
		kModConfigurationMenu_LoadFromJSON = 33,

		kModConfigurationMenu_ModList = 11,
		kModConfigurationMenu_ModListItem = 12,

		kModConfigurationMenu_SettingList = 19,
		kModConfigurationMenu_SettingListItem = 20,

		kModConfigurationMenu_CategoryLeftArrow = 13,
		kModConfigurationMenu_CategoryText = 14,
		kModConfigurationMenu_CategoryRightArrow = 15,

		kModConfigurationMenu_SettingCategoryLeftArrow = 23,
		kModConfigurationMenu_SettingCategoryText = 24,
		kModConfigurationMenu_SettingCategoryRightArrow = 25,


		kModConfigurationMenu_ChoiceText = 99,
		kModConfigurationMenu_SliderLeftArrow = 100,
		kModConfigurationMenu_SliderRightArrow = 101,
		kModConfigurationMenu_SliderDraggableRect = 102,
		kModConfigurationMenu_SubsettingInputFieldText = 103,
		kModConfigurationMenu_SubsettingInputFieldText_BoxBG = 104,
		kModConfigurationMenu_SliderText = 105,
	};


	ModConfigurationMenu();

	static ModConfigurationMenu* Create();

	~ModConfigurationMenu() override;

	void	SetTile(UInt32 tileID, Tile* activeTile) override;
	void	HandleLeftClick(UInt32 tileID, Tile* activeTile) override;
	void	HandleClick(UInt32 tileID, Tile* activeTile) override;
	void	HandleMouseover(UInt32 tileID, Tile* activeTile) override;
	void	HandleUnmouseover(UInt32 tileID, Tile* activeTile) override;
	void	PostDragTileChange(UInt32 tileID, Tile* newTile, Tile* activeTile) override {};
	void	PreDragTileChange(UInt32 tileID, Tile* oldTile, Tile* activeTile) override {};
	void	HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile) override;
	void	OnClickHeld(UInt32 tileID, Tile* activeTile) override {};
	void	HandleMousewheel(UInt32 tileID, Tile* activeTile) override;
	void	Update() override;
	bool	HandleKeyboardInput(UInt32 key) override;
	UInt32	GetID() override;
	bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState) override; // isHeld, isPressed etc.
	bool	HandleControllerInput(int code, Tile* activeTile) override;
	void	OnUpdateUserTrait(int tileVal) override {};
	void	HandleControllerConnectOrDisconnect(bool isControllerConnected) override;

	union
	{
		Tile* tiles[11];
		struct
		{
			Tile* doneTile;
			Tile* menuTitle;

			Tile* modsListBackground;
			Tile* settingsListBackground;
		};
	};

	std::map<std::string, std::unique_ptr<CMTag>>	g_Tags;
	std::vector<std::unique_ptr<CMSetting>>		g_Settings;


	class ListBoxWithFilter
	{
	public:
		ListBoxWithFilter() {}
		~ListBoxWithFilter() = default;

		ListBox<CMSetting> listBox;

		std::string categoryActive = "!NotEmpty";

		TileText* tagTile;
		std::set<std::string> tags;
		std::string tagActive;

		void UpdateTagString();

		void operator++();
		void operator--();
		void operator<<(const std::string& tag);
		void operator<<=(const std::string& tag);

		void Display(const std::string& category, bool display, bool all, bool doublestacked = false);
		void Click(Tile* clickedTile)
		{
			listBox.GetItemForTile(clickedTile)->Click(clickedTile);
		}
	};

	CMTag* tagDefault = nullptr;

	ListBoxWithFilter modsListBox;
	ListBoxWithFilter settingsListBox;

	class Description
	{
	public:
		Tile* tile;

		void Set(const std::string& str) const
		{
			if (tile) tile->Set(kTileValue_string, str);
		}

		void operator<<=(const CMSetting* setting) const
		{
			Set(setting->GetDescription());
		}

		void operator<<=(const ListBoxWithFilter& listBox) const
		{
			const auto menu = GetSingleton();
			if (menu->g_Tags.contains(listBox.tagActive) && menu->g_Tags[listBox.tagActive].get())
				Set(menu->g_Tags[listBox.tagActive].get()->GetDescription());
			else Set("");
		}

		void operator<<=(const std::string& str) const
		{
			Set(str);
		}
	};
	Description description;


	InputField subSettingInput;

	HotkeyField hotkeyInput;

	std::map<CMValue, std::string> fontMap;

	InputField searchBar;
	FILETIME lastXMLWriteTime;
	
	OSInputGlobals::ControlType controlDevice = OSInputGlobals::kControlType_Keyboard;

	std::vector<std::string> categoryHistory;

	bool HasTiles();
	void Close();
	void RefreshFilter();
	void ReloadMenuXML();
	bool XMLHasChanges();

	void ClickMod(Tile* mod);
	void ClickSetting(Tile* mod);

	void FilterMods();
	void FilterSettings();

	void SaveModJSON(CMSetting* mod);
	void LoadModJSON(CMSetting* mod);

	void SetActiveSubsettingValueFromInput();
	bool GetInHotkeyMode();

	void ReadJSON(const std::filesystem::path& path);
	void ReadJSONForPath(const std::filesystem::path& path);

	static ModConfigurationMenu* GetSingleton();

	static ModConfigurationMenu* ReloadMenu();
	void ShowMenuFirstTime();


	void SaveToJSON();
	void LoadFromJSON();

	void Back();
	void Device();
	void Default();

	std::vector<CMSetting*> GetSettingsForString(std::string str);
};


//static_assert(sizeof(ModConfigurationMenu) == 436);
