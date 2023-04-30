#pragma once

#include "Utilities.h"
#include "Menu.h"

#include <set>
#include <map>
#include <utility>
#include <variant>

class MCMMod;
class MCMItem;
void WriteMCMHooks();
void MainLoop();

class CMTag;
class CMSetting;

inline UInt32 g_LogLevel = 3;
inline UInt32 g_saveValue = 1;

class CMJSONElem;

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

	CMValue(const CMJSONElem& elem);

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

class CMObject
{
public:

	enum kType
	{
		kNone,
		kTag,
		kOption,
		kCategory,
		kSetting
	};

	SInt32 priority;

	std::string id; // TODO: make ID: type + mods alphab. + type + name

	std::string name;
	std::string shortName;

	std::string description;

	CMObject() = default;
	CMObject(const CMJSONElem& elem);

	virtual ~CMObject() = default;
	virtual std::string GetID() { return id; }
	virtual std::string GetName() { return !name.empty() ? name : id; }
	virtual std::string GetShortName() { return !shortName.empty() ? shortName : GetName(); }

	virtual std::string GetDescription() const { return description; }

	virtual UInt32 GetType() { return kNone; };
};

class CMTag : public CMObject
{
public:

	CMTag() = default;
	CMTag(const CMJSONElem& elem);
};

class CMOption : public CMObject
{
public:
	CMOption(const CMJSONElem& elem);
};

class CMCategory : public CMObject
{
public:
	std::string mod;
	std::string plugin;

	bool doublestacked;
	bool allTag;
	bool compatibilityMode;

	CMCategory(const CMJSONElem& elem);
	CMCategory(const MCMMod& item);

	std::string GetName() override { return !name.empty() ? name : "All"; }
	std::string GetShortName() override { return !shortName.empty() ? shortName : "All"; }
};

class CMSetting : public CMObject
{
public:
	class IO
	{
	public:

		class INI
		{
		public:
			std::filesystem::path file;
			std::string category;
			std::string setting;

			bool operator <(const INI& rhs) const
			{
				if (file != rhs.file)
					return file < rhs.file;

				if (category != rhs.category)
					return category < rhs.category;

				return setting < rhs.setting;
			}
		};

		class MCM
		{
			UInt8 modID;
			UInt8 tab;
			UInt8 option;
		};

		INI ini;

		std::string xml;

		std::string global;

		std::string gamesetting;

		std::string gameini;

		CMValue defaultValue;

		IO() = default;
		IO(const CMJSONElem& elem);

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

	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;

	CMSetting() {};
	CMSetting(const CMJSONElem& elem);
	CMSetting(const MCMMod& mod, const MCMItem& item);

	~CMSetting() override = default;

	std::string GetDescription() const override { return description; }

	static const char* GetTemplateAlt() { return "SettingNoneTemplate"; }
	virtual const char* GetTemplate() { return GetTemplateAlt(); }
	virtual const char* GetTypeName() { return "None"; }

	virtual CMSetting* Drag(Float32 value) { return this; };
	virtual CMSetting* Default() { return this; };
	virtual CMSetting* Display(Tile* tile) { return this; }

	virtual CMSetting* Click(Tile* tile) { return this; };

	enum ClickType
	{
		kValue,
		kValueAlt,
		kNext,
		kPrev,
		kUp,
		kDown
	};

	virtual CMSetting* ClickValue(Tile* tile, UInt32 option) { return this; };

	virtual bool IsCategory() { return false; }

	virtual std::vector<CMValue> GetValues() { return {}; };
	virtual void SetValues(const std::vector<CMValue>& values) {};

	void SetID();
};

class Category : public CMSetting
{
public:
	std::string categoryID;

	Category(const CMJSONElem& elem);
	Category(const MCMMod& mod);

	const char* GetTemplate() override { return "SettingSubsettingTemplate"; }
	const char* GetTypeName() override { return "Subsetting"; }

	Category* Default() override;
	Category* Click(Tile* tile) override;

	bool IsCategory() override { return true; }

	std::vector<CMValue> GetValues() override;
	void SetValues(const std::vector<CMValue>& values) override {};
};

class Choice : public CMSetting
{
public:
	IO setting;
	std::map<CMValue, std::string> choice;

	Choice(const CMJSONElem& elem);
	Choice(const MCMMod& mod, const MCMItem& item);

	const char* GetTemplate() override { return "SettingChoiceTemplate"; }
	const char* GetTypeName() override { return "Choice"; }


	CMValue GetPrev(const CMValue& value);
	CMValue GetNext(const CMValue& value);

	Choice* Default() override { setting.Default(); return this; }
	Choice* Display(Tile* tile) override;
	Choice* ClickValue(Tile* tile, UInt32 option) override;

	std::vector<CMValue> GetValues() override { return { setting.Read() }; };

	void SetValues(const std::vector<CMValue>& values) override
	{
		if (!values.empty()) setting.Write(values[0]);
	};
};

class Slider : public CMSetting
{
public:
	IO setting;

	CMValue min;
	CMValue max;
	CMValue delta;

	Slider(const CMJSONElem& elem);
	Slider(const MCMMod& mod, const MCMItem& item);

	const char* GetTemplate() override { return "SettingSliderTemplate"; }
	const char* GetTypeName() override { return "Slider"; }

	CMValue GetPrev(const CMValue& value) const;
	CMValue GetNext(const CMValue& value) const;

	Slider* Drag(Float32 value) override;
	Slider* Default() override { setting.Default(); return this; }
	Slider* Display(Tile* tile) override;

	Slider* ClickValue(Tile* tile, UInt32 option) override;

	std::vector<CMValue> GetValues() override { return { setting.Read() }; };
	void SetValues(const std::vector<CMValue>& values) override
	{
		if (!values.empty()) setting.Write(values[0]);
	};
};

class Control : public CMSetting
{
public:
	IO keyboard;
	IO mouse;
	IO controller;

	Control(const CMJSONElem& elem);
	Control(const MCMMod& mod, const MCMItem& item);

	const char* GetTemplate() override { return "SettingControlTemplate"; }
	const char* GetTypeName() override { return "Control"; }

	Control* Default() override { keyboard.Default(); mouse.Default(); controller.Default(); return this; }
	Control* Display(Tile* tile) override;
	Control* ClickValue(Tile* tile, UInt32 option) override;

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

class Font : public CMSetting
{
public:
	IO font;
	IO fontY;

	Font(const CMJSONElem& elem);

	const char* GetTemplate() override { return "SettingFontTemplate"; }
	const char* GetTypeName() override { return "Font"; }

	static CMValue GetPrev(const CMValue& value);
	static CMValue GetNext(const CMValue& value);
	
	Font* Default() override { font.Default(); fontY.Default(); return this; }
	Font* Display(Tile* tile) override;

	Font* ClickValue(Tile* tile, UInt32 option) override;

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

class Input : public CMSetting
{
public:
	IO setting;

	Input(const CMJSONElem& elem);
};

extern const char* MenuPath;

// randomly selected from the unused menu codes between 1001 and 1084 inclusive (to be compatible with the menu visibility array)
constexpr auto MENU_ID = 1042;

struct HotkeyField
{
	CMSetting* setting = nullptr;
	UInt32 value = 0;
	bool frameDebounce = true;

	UInt32 GetPressedKey();
	void SetActive(CMSetting* active);
	void Update();
};

class ModConfigurationMenu : public Menu
{
public:
	enum TileIDs
	{
		kTileID_Back = 0,
		kTileID_Default = 1,
		kTileID_DeviceButton = 2,
		kTileID_SaveToJSON = 4,
		kTileID_LoadFromJSON = 5,

		kTileID_Title = 10,
		kTileID_SelectionText = 11,

		kTileID_SettingList = 20,
		kTileID_SettingListItem,
		kTileID_SettingText,
		kTileID_SettingLeftArrow,
		kTileID_SettingRightArrow,

		kTileID_ExtraList = 30,
		kTileID_ExtraListItem,
		kTileID_ExtraText,
		kTileID_ExtraLeftArrow,
		kTileID_ExtraRightArrow,

		kTileID_SettingValue = 100,
		kTileID_SettingPrev = 101,
		kTileID_SettingNext = 102,
		kTileID_SettingUp = 103,
		kTileID_SettingDown = 104,
	};


	ModConfigurationMenu();

	static ModConfigurationMenu* Create();

	~ModConfigurationMenu() override;

	void	SetTile(UInt32 tileID, Tile* activeTile) override;
	void	HandleClick(UInt32 tileID, Tile* activeTile) override;
	void	HandleUnclick(UInt32 tileID, Tile* activeTile) override;
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

	void	UpdateRightClick();

	void	HandleRightClick(UInt32 tileID, Tile* activeTile);
	void	HandleRightUnclick(UInt32 tileID, Tile* activeTile);

	union
	{
		Tile* tiles[11];
		struct
		{
			Tile* tileBackButton;
			Tile* menuTitle;

		};
	};

	std::map<std::string, std::unique_ptr<CMTag>>		mapTags;
	std::map<std::string, std::unique_ptr<CMCategory>>	mapCategories;
	std::map<std::string, std::unique_ptr<CMOption>>	mapOptions;

	std::set<std::unique_ptr<CMSetting>>				setSettings;

	CMTag* tagDefault = nullptr;

	class SettingList
	{
	public:
		SettingList() {}
		~SettingList() = default;

		ListBox<CMSetting> listBox;

		std::string categoryActive = "!NotEmpty";

		TileText* tagTile;
		std::set<std::string> tags;
		std::string tagActive;

		bool main = false;
		bool allTag = false;
		bool doublestacked = false;

		Float32 alphaStart = 0;
		Float32 alphaTarget = 0;
		Float32 duration = 0;
		UInt32	startTime = 0;
		bool	displayAfterTimer = false;

		void UpdateTagString();

		void operator++();
		void operator--();
		void operator<<(const std::string& tag);
		void operator<<=(const std::string& tag);

		void UpdateSettingsList();
		void DisplaySettings();
		void Update();
		void Display(const std::string& newCategory, bool main, bool allTag, bool doublestacked);
		void Hide();
	};

	SettingList settingsMain;
	SettingList settingsExtra;

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

		void operator<<=(const SettingList& listBox) const
		{
			const auto menu = GetSingleton();
			if (menu->mapTags.contains(listBox.tagActive) && menu->mapTags[listBox.tagActive].get())
				Set(menu->mapTags[listBox.tagActive].get()->GetDescription());
			else Set("");
		}

		void operator<<=(const std::string& str) const
		{
//			Set(str);
		}
	};

	Description description;

	OSInputGlobals::ControlType controlDevice = OSInputGlobals::kControlType_Keyboard;

	std::vector<std::string> categoryHistory;

	std::map<CMValue, std::string> fontMap;

	class ControlHandler
	{
	public:
		Control* setting = nullptr;

		bool debounce = false;

		static UInt32 GetKey()
		{
			const auto input = OSInputGlobals::GetSingleton();
			for (int i = OSInputGlobals::_Escape; i < OSInputGlobals::Delete_ + 20; ++i)
				if (input->GetKeyState(i, OSInputGlobals::isPressed))
					return i;
			return 0;
		}

		static UInt32 GetController()
		{
			const auto input = OSInputGlobals::GetSingleton();
			for (int i = OSInputGlobals::kXboxCtrl_DPAD_UP; i < OSInputGlobals::kXboxCtrl_R3; ++i)
				if (input->GetControllerValue(i, OSInputGlobals::isPressed))
					return i;
			return 0;
		}

		static UInt32 GetMouse()
		{
			const auto input = OSInputGlobals::GetSingleton();
			for (int i = OSInputGlobals::LeftMouse; i < OSInputGlobals::MouseWheelDown; ++i)
				if (input->GetKeyState(i, OSInputGlobals::isPressed))
					return i;
			return 0;
		}

		bool IsActive()
		{
			return setting;
		}

		void UpdateSetting()
		{
			const auto menu = GetSingleton();
			const auto tile = menu->settingsMain.listBox.GetTileFromItem(setting);
			setting->Display(tile);
		}

		void SetKeyboard(UInt32 key)
		{
			if (key == OSInputGlobals::_Escape) key = 0;

			setting->keyboard.Write((SInt32) key);
			UpdateSetting();
			setting = nullptr;
		}

		void SetMouse(UInt32 key)
		{
			setting->mouse.Write((SInt32)key);
			UpdateSetting();
			setting = nullptr;
		}

		void SetController(UInt32 key)
		{
			setting->controller.Write((SInt32)key);
			UpdateSetting();
			setting = nullptr;
		}

		bool HandleControl()
		{
			if (IsActive())
			{
				if (const auto key = GetKey())
				{
					SetKeyboard(key);
					return true;
				}
				if (const auto key = GetMouse())
				{
					SetMouse(key);
					return true;
				}
				if (const auto key = GetController()) 
				{
//					SetController(key);
					return true;
				}
			}
			return false;
		}
	};

	ControlHandler controlHandler;

	InputField subSettingInput;

	HotkeyField hotkeyInput;
	


	InputField searchBar;
	FILETIME lastXMLWriteTime;

	bool HasTiles();
	void Close();
	void RefreshFilter();
	void ReloadMenuXML();
	bool XMLHasChanges();

	void ClickItem(Tile* mod);
	void ClickExtra(Tile* mod);
	void ClickValue(Tile* value, CMSetting::ClickType option = CMSetting::kValue);
	void Drag(Tile* mod);
	void FilterMods();
	void FilterSettings();

	void SaveModJSON(CMSetting* mod);
	void LoadModJSON(CMSetting* mod);

	void SetActiveSubsettingValueFromInput();

	void ReadJSON(const std::filesystem::path& path);
	void ReadJSONForPath(const std::filesystem::path& path);
	void ReadMCM();

	static ModConfigurationMenu* GetSingleton();

	static ModConfigurationMenu* ReloadMenu();
	void ShowMenuFirstTime();


	void SaveToJSON();
	void LoadFromJSON();

	void Back();
	void Device();
	void Default();

	std::vector<CMSetting*> GetSettingsForString(std::string str);

	void DisplaySettings(std::string id);

	void UpdateEscape();
};

//static_assert(sizeof(ModConfigurationMenu) == 436);
