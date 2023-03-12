#pragma once
#include <set>

#include "StewMenuItem.h"
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
extern const char* TweaksINIPath;
extern const char* MenuPath;

void ShowTweaksMenu();
void InitTweaksListFromJSON();
void SetDisplayedValuesForSubsetting(Tile* tile, StewMenuSubsettingItem* setting);
void SetListBoxesKeepSelectedWhenNonActive(bool isActive);
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
	kStewMenu_TweakListItem = 12,
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

class StewMenu : public Menu
{
private:
	void Free();
	void LoadINIs();
	class TweakListBox : public ListBox<StewMenuItem>
	{
	public:
		void Destroy()
		{
			for (auto iter : this->list)
			{
				StewMenuItem* item = iter->object;
				item->Free();
			}

			ListBox::Destroy();
		}
	};

	typedef ListBox<char> CategoryListBox;


	class SubSettingsListBox : public ListBox<StewMenuSubsettingItem>
	{
	public:
		Tile* Insert(StewMenuSubsettingItem* item)
		{
			auto tile = ListBox::Insert(item, item->GetName(), nullptr, item->GetTemplate());
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
	StewMenu()
	{
		memset(tiles, 0, sizeof(tiles));
		tweaksListBox.Init();
		tweaksListBox.flags &= ~TweakListBox::kFlag_RecalculateHeightsOnInsert;
		categoriesListBox.Init();
		searchBar.Init();
		subSettingInput.Init();
		hotkeyInput.Init();
		subSettingsListBox.Init();
		touchedSubsettings.Init();
		touchedTweaks.Init();

		activeSubSettingsList = &subSettingsListBox;

		_SelectedTrait = Tile::TraitNameToID("_selected");
		_TextAlphaTrait = Tile::TraitNameToID("_TextAlpha");
		_IsSearchActiveTrait = Tile::TraitNameToID("_IsSearchActive");
		_IsCategoriesActiveTrait = Tile::TraitNameToID("_IsCategoriesActive");
		_FilterModeTrait = Tile::TraitNameToID("_FilterMode");
		_CursorXTrait = Tile::TraitNameToID("_CursorX");
		_CursorYTrait = Tile::TraitNameToID("_CursorY");
		_MinTrait = Tile::TraitNameToID("_min");
		_MaxTrait = kTileValue_user0;
		_SetVTrait = kTileValue_user2;
		_ValueTrait = Tile::TraitNameToID("_Value");
		_ValueStringTrait = Tile::TraitNameToID("_ValueString");
		_BackgroundVisibleTrait = Tile::TraitNameToID("_BackgroundVisible");

		filterMode = FilterMode::kFilterMode_ShowAll;
		isDraggingCategoriesSlider = false;
		selectedCategory = nullptr;
		activeInputSubsetting = nullptr;
		activeHotkeySubsetting = nullptr;

		lastXMLWriteTime.dwLowDateTime = 0;
		lastXMLWriteTime.dwHighDateTime = 0;

		HANDLE tweaksXMLHandle = CreateFile(MenuPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (tweaksXMLHandle != INVALID_HANDLE_VALUE)
		{
			GetFileTime(tweaksXMLHandle, nullptr, nullptr, &lastXMLWriteTime);
			CloseHandle(tweaksXMLHandle);
		}

		ini.SetUnicode();

		LoadINIs();

		SetListBoxesKeepSelectedWhenNonActive(true);
	};
	~StewMenu() {};

	virtual void	Destructor(bool doFree);
	virtual void	SetTile(UInt32 tileID, Tile* value);
	virtual void	HandleLeftClickPress(UInt32 tileID, Tile* activeTile);
	virtual void	HandleClick(SInt32 tileID, Tile* clickedTile);
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

	TweakListBox tweaksListBox;
	CategoryListBox categoriesListBox;
	SubSettingsListBox subSettingsListBox;
	SubSettingsListBox* activeSubSettingsList;
	InputField subSettingInput;
	HotkeyField hotkeyInput;
	StewMenuSubsettingItem* activeInputSubsetting;
	StewMenuSubsettingItem* activeHotkeySubsetting;
	StewMenuItem* activeTweak;
	InputField searchBar;
	CSimpleIniA ini;
	FILETIME lastXMLWriteTime;
	UInt8 filterMode;
	TList<StewMenuSubsettingItem> touchedSubsettings;
	TList<StewMenuItem> touchedTweaks;

	std::set<char*> allCategories;
	const char* selectedCategory;

	// debounce to prevent the categories list closing after dragging the slider
	bool isDraggingCategoriesSlider;

	bool HasTiles();
	static void InitHooks();
	static StewMenu* Create();
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
	void SetActiveTweak(StewMenuItem* item);
	void SetTweakDescriptionTileString(StewMenuItem* item);
	void SetSubsettingDescriptionTileString(StewMenuSubsettingItem* setting);
	bool ToggleTweakInINI(StewMenuItem* item, Tile* tile);
	void SetSubsettingValueFromINI(StewMenuSubsettingItem* item);
	bool IsSubsettingInputValid();
	void SetActiveSubsettingValueFromInput();
	void SetInHotkeyMode(bool isActive);
	bool GetInHotkeyMode();
	void SetCursorPosTraits();

	void TouchTweak(StewMenuItem* tweak);	
	void TouchSubsetting(StewMenuSubsettingItem* setting);
	void WriteTweakToINI(StewMenuItem* tweak);
	void WriteSubsettingToINI(StewMenuSubsettingItem* setting);
	void WriteAllChangesToINIs();
	void RefreshActiveTweakSelectionSquare();

	static StewMenu* GetSingleton();

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
	if (true)
	{
		if (true)
		{
			ShowTweaksMenu();
		}
	}


	// return the StartMenu to the settings menu, to prevent a visual bug since we don't open a sub-menu
	CdeclCall(0x7D0700); // MenuButton:Settings
}

// wraps a call that adds the settings menu to the start menu 
inline void __fastcall addTweaksButton(BSSimpleArray<StartMenuOption*>* startMenuOptions, void* edx, StartMenuOption** settingsMenuItem)
{
	startMenuOptions->Append(settingsMenuItem);

	StartMenuOption* tweaksButton = StartMenuOption::Create("Tweaks", TweaksButtonCallback, StartMenuOption::kSettingsMenu);
	startMenuOptions->Append(&tweaksButton);
}

