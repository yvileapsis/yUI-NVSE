#pragma once
#include <map>

#include "Containers.h"
#include "Utilities.h"
#include "InputField.h"

#include <set>
#include <utility>
#include <variant>

#include "Menus.h"

typedef std::variant<SInt32, Float64, std::string> SM_Value;

inline std::map<SM_Value, std::string> fontMap;

inline std::string GetStringFromValue(const SM_Value& value)
{
	if (std::holds_alternative<std::string>(value)) return std::get<std::string>(value);
	if (std::holds_alternative<Float64>(value)) return std::to_string(std::get<Float64>(value));
	if (std::holds_alternative<SInt32>(value)) return std::to_string(std::get<SInt32>(value));
	return "";
}

inline Float64 GetFloatFromValue(const SM_Value& value)
{
	if (std::holds_alternative<Float64>(value)) return std::get<Float64>(value);
	if (std::holds_alternative<SInt32>(value)) return std::get<SInt32>(value);
	return 0;
}

inline SM_Value operator+(const SM_Value& left, const SM_Value& right)
{
	if (std::holds_alternative<SInt32>(left) && std::holds_alternative<SInt32>(right)) 
		return std::get<SInt32>(left) + std::get<SInt32>(right);
	return GetFloatFromValue(left) + GetFloatFromValue(right);
}

inline SM_Value operator-(const SM_Value& left, const SM_Value& right)
{
	if (std::holds_alternative<SInt32>(left) && std::holds_alternative<SInt32>(right))
		return std::get<SInt32>(left) - std::get<SInt32>(right);
	return GetFloatFromValue(left) - GetFloatFromValue(right);
}


class SM_Tag
{
public:
	std::string id;
	std::string name;
	std::string description;
};

class SM_Mod
{
public:
	std::string id;
	std::string version;

	std::unordered_set<std::string> tags;

	std::string name;
	std::string category;
	std::string description;

	std::string settingPath;
	std::string settingCategory;
	std::string settingName;

	signed int priority;
};

struct SM_Setting
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

		void Next(const bool forward) override;
		void Last(const bool forward) override;
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

		void Next(const bool forward) override;
		void Last(const bool forward) override;
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

		void Next(const bool forward) override;
		void Last(const bool forward) override;
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

	std::string name;
	std::string id;
	std::string description;

	SInt32 priority = 0;

	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;
};

struct SubsettingList : TList<SM_Setting>
{
	void Destroy()
	{
		for (auto iter : *this)
		{
//			auto item = iter;
//			iter->destroy;
		}
		this->DeleteAll();
	}
};

extern const char* MenuPath;

void ShowTweaksMenu();
void WriteValueToINIs(const char* category, const char* name, const char* value);

// randomly selected from the unused menu codes between 1001 and 1084 inclusive (to be compatible with the menu visibility array)
constexpr auto MENU_ID = 1042;

enum TileIDs
{
	kModConfigurationMenu_Title = 0,
	kModConfigurationMenu_ModList = 2,

	kModConfigurationMenu_SelectionText = 4,

	kModConfigurationMenu_SearchBar = 5,
	kModConfigurationMenu_SearchIcon = 6,

	kModConfigurationMenu_Exit = 7,
	kModConfigurationMenu_Back = 8,

	kModConfigurationMenu_ModListItem = 12,

	kModConfigurationMenu_CategoriesBackground = 13,
	kModConfigurationMenu_CategoryItem = 14,
	kModConfigurationMenu_CategoriesButton = 15,

	kModConfigurationMenu_CancelSearch = 16,

	kModConfigurationMenu_ActiveCategory = 17,
	kModConfigurationMenu_ToggleShowActive = 18,

	kModConfigurationMenu_SettingList = 19,
	kModConfigurationMenu_SettingListItem = 20,


	kModConfigurationMenu_DeviceButton = 30,

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
	void Free();

	typedef ListBox<SM_Mod> ModsListBox;
	typedef ListBox<char> CategoryListBox;

	class SettingsListBox : public ListBox<SM_Setting>
	{
	public:
		Tile* Insert(SM_Setting* item)
		{
//			const auto tile = ListBox::Insert(item, item->name.c_str(), nullptr, item->GetTemplate());

			const auto tile = ListBox::InsertAlt(item, item->name.c_str(), item->GetTemplate());

			SortAlt(tile);

			tile->tile->SetFloat(kTileValue_id, kModConfigurationMenu_SettingListItem);

			return tile->tile;
		}
	};

public:
	ModConfigurationMenu()
	{
		memset(tiles, 0, sizeof(tiles));
		modsListBox.Init();
		modsListBox.flags &= ~ModsListBox::kFlag_RecalculateHeightsOnInsert;
		categoriesListBox.Init();
		searchBar.Init();
		subSettingInput.Init();
		hotkeyInput.Init();
		settingsListBox.Init();
//		settingsListBox.flags &= ~ModsListBox::kFlag_RecalculateHeightsOnInsert;


		touchedSubsettings.Init();
		touchedTweaks.Init();

		isDraggingCategoriesSlider = false;
		activeInputSubsetting = nullptr;
		activeHotkeySubsetting = nullptr;

		lastXMLWriteTime.dwLowDateTime = 0;
		lastXMLWriteTime.dwHighDateTime = 0;

	};
	~ModConfigurationMenu() {};

	void	Destructor(bool doFree) override;
	void	SetTile(UInt32 tileID, Tile* value) override;
	void	HandleLeftClick(UInt32 tileID, Tile* activeTile) override;
	void	HandleClick(UInt32 tileID, Tile* clickedButton) override;
	void	HandleMouseover(UInt32 tileID, Tile* activeTile) override;
	void	HandleUnmouseover(UInt32 tileID, Tile* tile) override;
	void	PostDragTileChange(UInt32 tileID, Tile* newTile, Tile* activeTile) override {};
	void	PreDragTileChange(UInt32 tileID, Tile* oldTile, Tile* activeTile) override {};
	void	HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile) override;
	void	OnClickHeld(UInt32 tileID, Tile* activeTile) override {};
	void	HandleMousewheel(UInt32 tileID, Tile* tile) override;
	void	Update(void) override;
	bool	HandleKeyboardInput(UInt32 inputChar) override;
	UInt32	GetID() override;
	bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState) override; // isHeld, isPressed etc.
	bool	HandleControllerInput(int a2, Tile* tile) override;
	void	OnUpdateUserTrait(int tileVal) override {};
	void	HandleControllerConnectOrDisconnect(bool isControllerConnected) override;

	union
	{
		Tile* tiles[11];
		struct
		{
			Tile* doneTile;
			Tile* menuTitle;
			Tile* tweaksListBackground;
			Tile* categoriesBackground;

			Tile* categoriesButton;
			TileText* currentCategory;
			Tile* selectionText;
			Tile* searchIcon;

			Tile* searchCancel;
			Tile* showActive;
			Tile* subSettingsListBackground;
		};
	};

	std::vector<std::unique_ptr<SM_Tag>>		g_Tags;
	std::vector<std::unique_ptr<SM_Mod>>		g_Mods;
	std::vector<std::unique_ptr<SM_Setting>>	g_Settings;

	ModsListBox modsListBox;
	SettingsListBox settingsListBox;
	CategoryListBox categoriesListBox;

	InputField subSettingInput;

	HotkeyField hotkeyInput;

	SM_Setting* activeInputSubsetting;
	SM_Setting* activeHotkeySubsetting;

	std::string activeTab;
	std::string activeTag;

	SM_Mod* activeTweak;

	InputField searchBar;
	FILETIME lastXMLWriteTime;

	TList<SM_Setting> touchedSubsettings;
	TList<SM_Mod> touchedTweaks;


	// debounce to prevent the categories list closing after dragging the slider
	bool isDraggingCategoriesSlider;

	OSInputGlobals::ControlType controlDevice = OSInputGlobals::kControlType_Keyboard;

	bool HasTiles();
	static ModConfigurationMenu* Create() { return new ModConfigurationMenu(); };
	void Close();
	void RefreshFilter();
	void ReloadMenuXML();
	bool XMLHasChanges();
	void SetInSearchMode(bool isSearchMode);
	bool GetInSearchMode();
	void SetInSubsettingInputMode(bool isActive);
	bool GetInSubsettingInputMode();
	bool IsSearchSuspended();
	void SetCategoriesListActive(bool isVisible);
	bool GetCategoriesListActive();
	void ClearAndCloseSearch();
	void DisplaySettings(std::string tab);
	void SetActiveMod(SM_Mod* mod);
	bool IsSubsettingInputValid();
	void SetActiveSubsettingValueFromInput();
	void SetInHotkeyMode(bool isActive);
	bool GetInHotkeyMode();

	void ReadJSON(const std::filesystem::path& path);
	void ReadJSONForPath(const std::filesystem::path& path);

	static ModConfigurationMenu* GetSingleton();

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}

	void operator delete(void* ptr)
	{
		FormHeapFree(ptr);
	}
};
