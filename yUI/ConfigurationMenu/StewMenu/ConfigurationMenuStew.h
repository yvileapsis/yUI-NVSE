#pragma once
#include "Containers.h"
#include "Utilities.h"
#include "InputField.h"
#include "HotkeyField.h"
#include <SimpleINILibrary.h>

#include <set>
#include <utility>
#include <variant>

typedef std::variant<SInt32, Float64, std::string> SM_Value;

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
	signed int value;

	const char* GetInternalName() { return settingName.c_str(); };
	const char* GetInternalCategory()
	{
		// if there's no internal category set, assume it's tweaks - done to avoid needlessly creating ~400 'Tweaks' strings since most tweak items will have that category
		if (!settingCategory.empty()) return settingCategory.c_str();
		return "Tweaks";
	};
	bool IsBoolean() { return !settingName.empty() && settingName[0] == 'b'; };
};

struct SM_Setting
{
	enum ElementType
	{
		kSettingType_None,
		kSettingType_Control,
		kSettingType_Choice,
		kSettingType_SliderInteger,
		kSettingType_SliderFloat,
		kSettingType_InputString,
		kSettingType_InputFloat,
	};

	struct INISetting
	{
		std::filesystem::path	path;
		std::string				category;
		std::string				value;
	};

	std::string name;
	std::string id;
	std::string description;

	ElementType type;

	INISetting setting;

	SInt32 priority = 0;

	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;

	SM_Value valueDefault;

	typedef std::unordered_map<SM_Value, std::pair<std::string, std::string>> SM_ValueOptions;

	SM_ValueOptions values;

	void SetDisplayedValue(Tile* tile);

	SM_Value ReadINI();
	void WriteINI(SM_Value value);

	const char* GetTemplate() {
		switch (type)
		{
		case kSettingType_Choice:
			return "SettingTemplateChoice";
//		case kSettingType_Slider:
//			return "SubSettingTemplate_Slider";
//		case kSettingType_StringInput:
//		case kSettingType_NumberInput:
//		case kSettingType_Hotkey:
//			return "SubSettingTemplate_Input";
//		case kSettingType_Options:
//			return "SubSettingTemplate_Dropdown";
		}
		return nullptr;
	};

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
	kStewMenu_Title = 0,
	kStewMenu_TweaksListBackground = 2,
	kStewMenu_SelectionText = 4,
	kStewMenu_SearchBar = 5,
	kStewMenu_SearchIcon = 6,
	kStewMenu_Exit = 7,
	kStewMenu_Back = 8,
	kModConfigurationMenu_ModListItem = 12,
	kStewMenu_CategoriesBackground = 13,
	kStewMenu_CategoryItem = 14,
	kStewMenu_CategoriesButton = 15,
	kStewMenu_CancelSearch = 16,
	kStewMenu_ActiveCategory = 17,
	kStewMenu_ToggleShowActive = 18,
	kStewMenu_SubSettingsListBackground = 19,
	kStewMenu_SubsettingItem = 20,

	kModConfigurationMenu_ChoiceText = 99,
	kStewMenu_SliderLeftArrow = 100,
	kStewMenu_SliderRightArrow = 101,
	kStewMenu_SliderDraggableRect = 102,
	kStewMenu_SubsettingInputFieldText = 103,
	kStewMenu_SubsettingInputFieldText_BoxBG = 104,
	kStewMenu_SliderText = 105,
};

enum FilterMode
{
	kFilterMode_ShowAll = 0,
	kFilterMode_ShowActive,
	kFilterMode_ShowInActive,
	kFilterMode_Count,
};

class ModConfigurationMenu : public Menu
{
private:
	void Free();
//	void LoadINIs();
	class ModsListBox : public ListBox<SM_Mod>
	{
	public:
		void Destroy()
		{
			for (const auto iter : list)
			{
				iter->object->~SM_Mod();
			}

			ListBox::Destroy();
		}
	};

	typedef ListBox<char> CategoryListBox;


	class SettingsListBox : public ListBox<SM_Setting>
	{
	public:
		Tile* Insert(SM_Setting* item)
		{
			const auto tile = ListBox::Insert(item, item->name.c_str(), nullptr, item->GetTemplate());

			tile->SetFloat(kTileValue_id, kStewMenu_SubsettingItem);

			return tile;
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
		touchedSubsettings.Init();
		touchedTweaks.Init();

		filterMode = FilterMode::kFilterMode_ShowAll;
		isDraggingCategoriesSlider = false;
		activeInputSubsetting = nullptr;
		activeHotkeySubsetting = nullptr;

		lastXMLWriteTime.dwLowDateTime = 0;
		lastXMLWriteTime.dwHighDateTime = 0;

		ini.SetUnicode();

//		LoadINIs();
	};
	~ModConfigurationMenu() {};

	virtual void	Destructor(bool doFree);
	virtual void	SetTile(UInt32 tileID, Tile* value);
	virtual void	HandleLeftClick(UInt32 tileID, Tile* activeTile);
	virtual void	HandleClick(UInt32 tileID, Tile* clickedButton);
	virtual void	HandleMouseover(UInt32 tileID, Tile* activeTile);
	virtual void	HandleUnmouseover(UInt32 tileID, Tile* tile);
	virtual void	PostDragTileChange(UInt32 tileID, Tile* newTile, Tile* activeTile) {};
	virtual void	PreDragTileChange(UInt32 tileID, Tile* oldTile, Tile* activeTile) {};
	virtual void	HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile);
	virtual void	OnClickHeld(UInt32 tileID, Tile* activeTile) {};
	virtual void	HandleMousewheel(UInt32 tileID, Tile* tile);
	virtual void	Update(void);
	virtual bool	HandleKeyboardInput(UInt32 inputChar);
	virtual UInt32	GetID(void);
	virtual bool	HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState); // isHeld, isPressed etc.
	virtual bool	HandleControllerInput(int a2, Tile* tile);
	virtual void	OnUpdateUserTrait(int tileVal) {};
	virtual void	HandleControllerConnectOrDisconnect(bool isControllerConnected);

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
	CategoryListBox categoriesListBox;
	SettingsListBox settingsListBox;
	InputField subSettingInput;
	HotkeyField hotkeyInput;
	SM_Setting* activeInputSubsetting;
	SM_Setting* activeHotkeySubsetting;
	SM_Mod* activeTweak;
	InputField searchBar;
	CSimpleIniA ini;
	FILETIME lastXMLWriteTime;
	UInt8 filterMode;
	TList<SM_Setting> touchedSubsettings;
	TList<SM_Mod> touchedTweaks;

	std::set<char*> allCategories;
	std::string selectedCategory;

	// debounce to prevent the categories list closing after dragging the slider
	bool isDraggingCategoriesSlider;

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
	void CycleFilterMode();
	void AddCategory(char* category);
	bool HandleActiveSliderArrows(bool isRightArrow, float scale = 0.05F);
	void SetActiveTweak(SM_Mod* item);
	void SetTweakDescriptionTileString(SM_Mod* item);
	void SetSubsettingDescriptionTileString(SM_Setting* setting);
	bool ToggleTweakInINI(SM_Mod* item, Tile* tile);
	bool IsSubsettingInputValid();
	void SetActiveSubsettingValueFromInput();
	void SetInHotkeyMode(bool isActive);
	bool GetInHotkeyMode();
	void SetCursorPosTraits();

	void TouchTweak(SM_Mod* tweak);	
	void TouchSubsetting(SM_Setting* setting);
	void WriteTweakToINI(SM_Mod* tweak);
	void WriteSubsettingToINI(SM_Setting* setting);
	void WriteAllChangesToINIs();
	void RefreshActiveTweakSelectionSquare();

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