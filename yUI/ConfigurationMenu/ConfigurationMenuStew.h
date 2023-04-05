#pragma once

#include "Utilities.h"
#include "Menus.h"

#include <set>
#include <map>
#include <utility>
#include <variant>


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

class SM_Tag
{
public:
	std::string id;
	std::string name;
	std::string description;

	SInt32 priority;

	SM_Tag() = default;
	SM_Tag(std::string id, std::string name, std::string description)
	: id(std::move(id)), name(std::move(name)), description(std::move(description))
	{}

	virtual std::string GetID() { return id; }
	virtual std::string GetName() { return !name.empty() ? name : id; }
	virtual std::string GetDescription() { return FormatString("%s\n- %s", description.c_str(), " "); }
};

class SM_Mod : public SM_Tag
{
public:

	std::unordered_set<std::string> tags;
	std::string version;
};

struct SM_Setting : public SM_Tag
{
	class SettingSource
	{
	public:
		typedef std::tuple<std::filesystem::path, std::string, std::string> INISetting;
		INISetting setting;
		// TODO: gamesetting and global
		SM_Value defaultValue;

		SM_Value ReadINI();
		void WriteINI(const SM_Value& value);
	};

	typedef std::map<SM_Value, std::pair<std::string, std::string>> SM_ValueChoice;
	typedef std::tuple<SM_Value, SM_Value, SM_Value> SM_ValueSlider;

	class None
	{
	public:
		virtual ~None() = default;

		virtual const char* GetTemplate() { return "SettingNoneTemplate"; }

		virtual void Write(const SM_Value& value) {}
		virtual void Display(Tile* tile) {}

		virtual void Next(const bool forward = true) {};
		virtual void Last(const bool forward = true) {};
	};

	class Subsetting : public None
	{
		const char* GetTemplate() override { return "SettingSubsettingTemplate"; }

	};

	class Choice : public None
	{
	public:
		SettingSource setting;
		SM_ValueChoice choice;

		const char* GetTemplate() override { return "SettingChoiceTemplate"; }

		void Write(const SM_Value& value) override { setting.WriteINI(value); }
		void Display(Tile* tile) override;

		SM_Value GetPrev(const SM_Value& value);
		SM_Value GetNext(const SM_Value& value);

		void Next(bool forward) override;
		void Last(bool forward) override;
	};

	class Slider : public None
	{
	public:
		SettingSource setting;
		SM_ValueSlider slider;

		const char* GetTemplate() override { return "SettingSliderTemplate"; }

		void Write(const SM_Value& value) override { setting.WriteINI(value); }
		void Display(Tile* tile) override;

		SM_Value GetPrev(const SM_Value& value) const;
		SM_Value GetNext(const SM_Value& value) const;

		void Next(bool forward) override;
		void Last(bool forward) override;
	};

	class Control : public None
	{
	public:
		SettingSource keyboard;
		SettingSource mouse;
		SettingSource controller;

		const char* GetTemplate() override { return "SettingControlTemplate"; }

		void Display(Tile* tile) override;
	};

	class Font : public None
	{
	public:
		SettingSource font;
		SettingSource fontY;

		const char* GetTemplate() override { return "SettingFontTemplate"; }

		void Write(const SM_Value& value) override { font.WriteINI(value); fontY.WriteINI(value); }
		void Display(Tile* tile) override;

		static SM_Value GetPrev(const SM_Value& value);
		static SM_Value GetNext(const SM_Value& value);

		void Next(bool forward) override;
		void Last(bool forward) override;
	};

	class Input : public None
	{
	public:
		SettingSource setting;
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

	SM_Setting* Next(const bool forward = true) { data->Next(forward); return this; };
	SM_Setting* Last(const bool forward = true) { data->Last(forward); return this; }
	SM_Setting* Display(Tile* tile) { data->Display(tile); return this; };
	const char* GetTemplate() const { return data->GetTemplate(); }

	ElementType type;
	std::unique_ptr<None> data;

	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;

	std::string GetDescription() override
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
	kModConfigurationMenu_CancelSearch = 16,

	kModConfigurationMenu_ModList = 11,
	kModConfigurationMenu_ModListItem = 12,

	kModConfigurationMenu_SettingList = 19,
	kModConfigurationMenu_SettingListItem = 20,

	kModConfigurationMenu_CategoryLeftArrow = 13,
	kModConfigurationMenu_CategoryText = 14,
	kModConfigurationMenu_CategoryRightArrow = 15,

	kModConfigurationMenu_ActiveCategory = 17,
	kModConfigurationMenu_ToggleShowActive = 18,

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
private:
	void* operator new(size_t size) { return GameHeapAlloc<void>(size); }
	void operator delete(void* ptr) { GameHeapFree(ptr); }

public:
	ModConfigurationMenu()
	{
		memset(tiles, 0, sizeof tiles);
		modsListBox.Init();
		modsListBox.flags &= ~ListBox<SM_Mod>::kFlag_RecalculateHeightsOnInsert;

		searchBar.Init();
		subSettingInput.Init();
		hotkeyInput.Init();
		settingsListBox.Init();
//		settingsListBox.flags &= ~ModsListBox::kFlag_RecalculateHeightsOnInsert;

		lastXMLWriteTime.dwLowDateTime = 0;
		lastXMLWriteTime.dwHighDateTime = 0;

	};
	~ModConfigurationMenu()
	{
		modsListBox.Destroy();

		settingsListBox.Destroy();
		searchBar.Free();
		subSettingInput.Free();
		hotkeyInput.Free();

		Menu::Free();

		OSInputGlobals::GetSingleton()->SetDebounceMenuMode(false);
	};

	void	Destructor(bool doFree) override;
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

	static ModConfigurationMenu* Create() { return new ModConfigurationMenu(); };

	union
	{
		Tile* tiles[11];
		struct
		{
			Tile* doneTile;
			Tile* menuTitle;
			Tile* tweaksListBackground;

			TileText* currentCategory;
			Tile* selectionText;
			Tile* searchIcon;

			Tile* searchCancel;
			Tile* showActive;
			Tile* subSettingsListBackground;

			Tile* categoriesMods;
			Tile* categoriesSettings;
		};
	};

	std::map<std::string, std::unique_ptr<SM_Tag>>	g_Tags;
	std::vector<std::unique_ptr<SM_Mod>>			g_Mods;
	std::vector<std::unique_ptr<SM_Setting>>		g_Settings;

	SM_Mod* activeMod;
	ListBox<SM_Mod> modsListBox;
	std::set<std::string> modTags;
	std::string activeModTag;

	SM_Setting* activeSetting;
	ListBox<SM_Setting> settingsListBox;
	std::set<std::string> settingTags;
	std::string activeSettingTag;


	InputField subSettingInput;

	HotkeyField hotkeyInput;

	std::map<SM_Value, std::string> fontMap;

	InputField searchBar;
	FILETIME lastXMLWriteTime;
	
	OSInputGlobals::ControlType controlDevice = OSInputGlobals::kControlType_Keyboard;

	bool HasTiles();
	void Close();
	void RefreshFilter();
	void ReloadMenuXML();
	bool XMLHasChanges();
	void SetInSearchMode(bool isSearchMode);
	bool GetInSearchMode();
	void SetInSubsettingInputMode(bool isActive);
	bool GetInSubsettingInputMode();
	bool IsSearchSuspended();
	void ClearAndCloseSearch();
	void DisplaySettings(std::string tab);

	void SetActiveMod(SM_Mod* mod);
	void SetActiveModTag(std::string tag);
	void SetActiveSettingTag(std::string tag);

	std::string GetModNextTag();
	std::string GetModPrevTag();

	std::string GetSettingNextTag();
	std::string GetSettingPrevTag();

	bool IsSubsettingInputValid();
	void SetActiveSubsettingValueFromInput();
	void SetInHotkeyMode(bool isActive);
	bool GetInHotkeyMode();

	void ReadJSON(const std::filesystem::path& path);
	void ReadJSONForPath(const std::filesystem::path& path);

	static ModConfigurationMenu* GetSingleton();

	static ModConfigurationMenu* ReloadMenu();
	void ShowTweaksMenu();

};
