#pragma once

#include "Utilities.h"
#include "Menu.h"

#include <set>
#include <map>
#include <utility>
#include <variant>

class CMTag;
class CMMod;
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

class SM_Value
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
	SM_Value() = default;
	SM_Value(const SInt32 value) : type(kInteger), intval(value) {}
	explicit SM_Value(const UInt32 value) : type(kInteger), intval(value) {}
	SM_Value(const Float64 value) : type(kFloat), floatval(value) {}
	SM_Value(std::string value) : type(kString), stringval(std::move(value)) {}

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

	SM_Value operator+(const SM_Value& right) const
	{
		if (IsString() && right.IsString())
			return GetAsString() + right.GetAsString();
		if (IsInteger() && right.IsInteger())
			return GetAsInteger() + right.GetAsInteger();
		return GetAsFloat() + right.GetAsFloat();
	}

	SM_Value operator-(const SM_Value& right) const
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

class CMMod : public CMTag
{
public:

	std::unordered_set<std::string> tags;
	std::string version;

	static const char* GetTemplate() { return "ModTemplate"; }
	static void Display(Tile* tile) {}
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

		SM_Value defaultValue;

		std::optional<SM_Value> ReadINI();
		void WriteINI(const SM_Value& value) const;

		std::optional<SM_Value> ReadXML();
		void WriteXML(const SM_Value& value) const;

		std::optional<SM_Value> ReadGameSetting();
		void WriteGameSetting(const SM_Value& value) const;

		std::optional<SM_Value> ReadGameINI();
		void WriteGameINI(const SM_Value& value) const;

		std::optional<SM_Value> ReadGlobal();
		void WriteGlobal(const SM_Value& value) const;

		SM_Value Read();
		void Write(const SM_Value& value) const;
	};

	// TODO: tagify this
	typedef std::map<SM_Value, std::pair<std::string, std::string>> SM_ValueChoice;
	typedef std::tuple<SM_Value, SM_Value, SM_Value> SM_ValueSlider;

	class None
	{
	public:
		virtual ~None() = default;

		static const char* GetTemplateAlt() { return "SettingNoneTemplate"; }
		virtual const char* GetTemplate() { return GetTemplateAlt(); }
		virtual const char* GetTypeName() { return "None"; }

		virtual void Display(Tile* tile) {}

		virtual void Next(const bool forward = true) {};
		virtual void Last(const bool forward = true) {};

		virtual std::vector<SM_Value> GetValues() { return {}; };
		virtual void SetValues(const std::vector<SM_Value>& values) {};
	};

	class Subsetting : public None
	{
		std::string mod;
		const char* GetTemplate() override { return "SettingSubsettingTemplate"; }
		const char* GetTypeName() override { return "Subsetting"; }

		std::vector<SM_Value> GetValues() override {
			return {};
//			GetSettingsForString(std::string str);
		};
		void SetValues(const std::vector<SM_Value>& values) override {};
	};

	class Choice : public None
	{
	public:
		IO setting;
		SM_ValueChoice choice;

		const char* GetTemplate() override { return "SettingChoiceTemplate"; }
		const char* GetTypeName() override { return "Choice"; }

		void Display(Tile* tile) override;

		SM_Value GetPrev(const SM_Value& value);
		SM_Value GetNext(const SM_Value& value);

		void Next(bool forward) override;
		void Last(bool forward) override;

		std::vector<SM_Value> GetValues() override { return { setting.Read() }; };

		void SetValues(const std::vector<SM_Value>& values) override
		{
			if (!values.empty()) setting.Write(values[0]);
		};
	};

	class Slider : public None
	{
	public:
		IO setting;
		SM_ValueSlider slider;

		const char* GetTemplate() override { return "SettingSliderTemplate"; }
		const char* GetTypeName() override { return "Slider"; }

		void Display(Tile* tile) override;

		SM_Value GetPrev(const SM_Value& value) const;
		SM_Value GetNext(const SM_Value& value) const;

		void Next(bool forward) override;
		void Last(bool forward) override;

		std::vector<SM_Value> GetValues() override { return { setting.Read() }; };
		void SetValues(const std::vector<SM_Value>& values) override
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

		std::vector<SM_Value> GetValues() override { return { keyboard.Read(), mouse.Read(), controller.Read() }; };

		void SetValues(const std::vector<SM_Value>& values) override
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

		static SM_Value GetPrev(const SM_Value& value);
		static SM_Value GetNext(const SM_Value& value);

		void Next(bool forward) override;
		void Last(bool forward) override;

		std::vector<SM_Value> GetValues() override { return { font.Read(), fontY.Read() }; };
		void SetValues(const std::vector<SM_Value>& values) override
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
	CMSetting* Display(Tile* tile) { data->Display(tile); return this; };
	const char* GetTemplate() const { return data->GetTemplate(); }

	std::vector<SM_Value> GetValues() const { return data->GetValues(); }
	void SetValues(const std::vector<SM_Value>& values) const { return data->SetValues(values); }

	ElementType type;
	std::unique_ptr<None> data;

	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;

	std::string GetDescription() const override
	{
		return FormatString("%s\n- %s", description.c_str(), "settingName.c_str()");
	}
};

extern const char* MenuPath;

void WriteValueToINIs(const char* category, const char* name, const char* value);

// randomly selected from the unused menu codes between 1001 and 1084 inclusive (to be compatible with the menu visibility array)
constexpr auto MENU_ID = 1042;

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
//	void* operator new(size_t size) { return GameHeapAlloc<void>(size); }
//	void operator delete(void* ptr) { GameHeapFree(ptr); }

	ModConfigurationMenu();

	static ModConfigurationMenu* Create();

	~ModConfigurationMenu() override;

//	void	Destructor(bool doFree) override;
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
	std::vector<std::unique_ptr<CMMod>>			g_Mods;
	std::vector<std::unique_ptr<CMSetting>>		g_Settings;

	template <typename Item> class ListBoxWithFilter : public ListBox<Item>
	{
	public:
		std::set<std::string> tags;
		std::string tagActive;
		TileText* tagTile;

		void UpdateTagString();
		void ClearTags();

		void operator++();
		void operator--();
		void operator<<(const std::string& tag);
		void operator<<=(const std::string& tag);
		ListBoxItem<Item>* operator<<=(Item* tag);
	};

	CMTag* tagDefault = nullptr;

	ListBoxWithFilter<CMMod> modsListBox;
	ListBoxWithFilter<CMSetting> settingsListBox;

	class Description
	{
	public:
		Tile* tile;

		void Set(const std::string& str) const
		{
			tile->Set(kTileValue_string, str);
		}

		void operator<<=(const CMSetting* setting) const
		{
			Set(setting->GetDescription());
		}

		void operator<<=(const CMMod* mod) const
		{
			Set(mod->GetDescription());
		}

		template <typename Item> void operator<<=(const ListBoxWithFilter<Item>& listBox) const
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

	std::map<SM_Value, std::string> fontMap;

	InputField searchBar;
	FILETIME lastXMLWriteTime;
	
	OSInputGlobals::ControlType controlDevice = OSInputGlobals::kControlType_Keyboard;

	CMMod* activeMod;
	CMSetting* activeSetting;

	bool HasTiles();
	void Close();
	void RefreshFilter();
	void ReloadMenuXML();
	bool XMLHasChanges();

	void DisplayMods();
	void DisplaySettings(std::string tab);

	void SelectMod(CMMod* mod);
	void SelectSetting(CMSetting* mod);

	void ClickMod(Tile* mod);
	void ClickSetting(Tile* mod);

	void FilterMods();
	void FilterSettings();

	void SaveModJSON(CMMod* mod);
	void LoadModJSON(CMMod* mod);

	void SetActiveSubsettingValueFromInput();
	bool GetInHotkeyMode();

	void ReadJSON(const std::filesystem::path& path);
	void ReadJSONForPath(const std::filesystem::path& path);

	static ModConfigurationMenu* GetSingleton();

	static ModConfigurationMenu* ReloadMenu();
	void ShowTweaksMenu();

	void Back();
	void Device();
	void Default();

	std::vector<CMSetting*> GetSettingsForString(std::string str);
};


//static_assert(sizeof(ModConfigurationMenu) == 436);
