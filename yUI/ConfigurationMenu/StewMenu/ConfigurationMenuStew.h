#pragma once
#include <set>

#include "SM_Mod.h"
#include "SM_Setting.h"
#include "InputField.h"
#include "HotkeyField.h"
#include <SimpleINILibrary.h>

static UInt32 _SelectedTrait;
static UInt32 _TextAlphaTrait;
static UInt32 _IsSearchActiveTrait;
static UInt32 _IsCategoriesActiveTrait;
static UInt32 _FilterModeTrait;
static UInt32 _CursorXTrait;
static UInt32 _CursorYTrait;
static UInt32 _MaxTrait;
static UInt32 _MinTrait;
static UInt32 _SetVTrait;
static UInt32 _ValueTrait;
static UInt32 _ValueStringTrait;
static UInt32 _BackgroundVisibleTrait;
extern const char* MenuPath;

void ShowTweaksMenu();
void InitTweaksListFromJSON();
void SetDisplayedValuesForSubsetting(Tile* tile, SM_Setting* setting);
void SetListBoxesKeepSelectedWhenNonActive(bool isActive);
void WriteValueToINIs(const char* category, const char* name, const char* value);

// randomly selected from the unused menu codes between 1001 and 1084 inclusive (to be compatible with the menu visibility array)
constexpr auto MENU_ID = 1042;

class JS_Tag
{
public:
	std::string id;
	std::string name;
	std::string description;
};

class JS_Mod
{
public:
	std::string id;
	std::string name;
	std::string description;
	std::unordered_set<std::string> tags;
	std::string version;
};

class JS_Setting
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
	SInt32 priority = 0;
	std::unordered_set<std::string> tags;
	std::unordered_set<std::string> mods;
	std::string condition;
	Type type = kNone;

};

class JS_SettingChoice : public JS_Setting
{
	std::unordered_map<SInt32, std::string> values;
	std::string path;
	SInt32 defaultValue;
};



enum TileIDs
{
	kStewMenu_Title = 0,
	kStewMenu_TweaksListBackground = 2,
	kStewMenu_SelectionText = 4,
	kStewMenu_SearchBar = 5,
	kStewMenu_SearchIcon = 6,
	kStewMenu_Exit = 7,
	kStewMenu_Back = 8,
	kConfigurationMenu_ModListItem = 12,
	kStewMenu_CategoriesBackground = 13,
	kStewMenu_CategoryItem = 14,
	kStewMenu_CategoriesButton = 15,
	kStewMenu_CancelSearch = 16,
	kStewMenu_ActiveCategory = 17,
	kStewMenu_ToggleShowActive = 18,
	kStewMenu_SubSettingsListBackground = 19,
	kStewMenu_SubsettingItem = 20,

	kStewMenu_CycleText = 99,
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

class ConfigurationMenu : public Menu
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
			auto tile = ListBox::Insert(item, item->name.c_str(), nullptr, item->GetTemplate());
			tile->SetFloat(kTileValue_id, kStewMenu_SubsettingItem);
			if (item->IsSlider())
			{
				if (auto sliderRect = tile->GetChildByID(kStewMenu_SliderDraggableRect))
				{
					if (item->GetDataType() == SubSettingData::kSettingDataType_Float)
					{
						auto currentVal = item->data.valueFloat;
						sliderRect->SetFloat(_MinTrait, min(currentVal, item->data.minValue));
						sliderRect->SetFloat(_MaxTrait, max(currentVal, item->data.maxValue));
					}
					else
					{
						auto currentVal = item->data.valueInt;
						sliderRect->SetFloat(_MinTrait, min(currentVal, item->data.minValue));
						sliderRect->SetFloat(_MaxTrait, max(currentVal, item->data.maxValue));
					}
				}
			}
			else
			{
				tile->SetFloat(_MinTrait, item->data.minValue);
				tile->SetFloat(_MaxTrait, item->data.maxValue);
			}

			return tile;
		}
	};

public:
	ConfigurationMenu()
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

		SetListBoxesKeepSelectedWhenNonActive(true);
	};
	~ConfigurationMenu() {};

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

	std::vector<std::unique_ptr<JS_Tag>>		g_Tags;
	std::vector<std::unique_ptr<JS_Mod>>		g_Mods;
	std::vector<std::unique_ptr<JS_Setting>>	g_Settings;

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
	static void InitHooks();
	static ConfigurationMenu* Create();
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
	void SetSubsettingValueFromINI(SM_Setting* item);
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

	static ConfigurationMenu* GetSingleton();

	void* operator new(size_t size)
	{
		return FormHeapAlloc(size);
	}

	void operator delete(void* ptr)
	{
		FormHeapFree(ptr);
	}
};

inline void TweaksButtonCallback()
{
	ShowTweaksMenu();
	// return the StartMenu to the settings menu, to prevent a visual bug since we don't open a sub-menu
	//CdeclCall(0x7D0700); // MenuButton:Settings
}

// wraps a call that adds the settings menu to the start menu 
inline void __fastcall addTweaksButton(BSSimpleArray<StartMenuOption*>* startMenuOptions, void* edx, StartMenuOption** settingsMenuItem)
{
	startMenuOptions->Append(settingsMenuItem);

	StartMenuOption* tweaksButton = StartMenuOption::Create("Mods", TweaksButtonCallback, StartMenuOption::kMainMenu + StartMenuOption::kPauseMenu);
	startMenuOptions->Append(&tweaksButton);
}

