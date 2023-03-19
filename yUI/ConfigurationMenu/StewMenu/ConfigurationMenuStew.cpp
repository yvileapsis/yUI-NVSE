#include "ConfigurationMenuStew.h"
#include <sstream>

#include "dinput8.h"
#include "functions.h"
#include "GameSettings.h"
#include "InterfaceManager.h"
#include "SafeWrite.h"
#include "SimpleINILibrary.h"

const char* MenuPath = "Data\\Menus\\untitledmenuproject.xml";

ModConfigurationMenu* g_stewMenu;

ModConfigurationMenu* ModConfigurationMenu::GetSingleton() { return g_stewMenu; };

_declspec(naked) bool IsControllerConnected()
{
	_asm
	{
		cmp byte ptr ds : [0x11D8C50] , 1 // isControllerDisabled
		je done
		cmp byte ptr ds : [0x11D8A84] , 0 // isControllerConnected
		done :
		setne al
		ret
	}
}

int g_bTweaksMenuShowSettingPathOnSubsettings = true;

void ModConfigurationMenu::SetTile(UInt32 tileID, Tile* tile)
{
	switch (tileID)
	{
	case kStewMenu_Title:
		this->menuTitle = tile;
		break;
	case kStewMenu_TweaksListBackground:
		this->tweaksListBackground = tile;
		break;
	case kStewMenu_SelectionText:
		this->selectionText = tile;
		break;
	case kStewMenu_Exit:
		this->doneTile = tile;
		break;
	case kStewMenu_SearchBar:
		this->searchBar.tile = tile;
		break;
	case kStewMenu_SearchIcon:
		this->searchIcon = tile;
		break;
	case kStewMenu_CategoriesBackground:
		this->categoriesBackground = tile;
		break;
	case kStewMenu_CategoriesButton:
		this->categoriesButton = tile;
		break;
	case kStewMenu_CancelSearch:
		this->searchCancel = tile;
		break;
	case kStewMenu_ActiveCategory:
		this->currentCategory = (TileText*)tile;
		break;
	case kStewMenu_ToggleShowActive:
		this->showActive = tile;
		break;
	case kStewMenu_SubSettingsListBackground:
		this->subSettingsListBackground = tile;
		break;
	}
}

void ModConfigurationMenu::HandleMousewheel(UInt32 tileID, Tile* tile)
{

}

void SetSliderDisplayedValues(Tile* sliderRect, SM_Setting* setting)
{
	/*
	char buf[0x40];
	if (setting->GetDataType() == SubSettingData::kSettingDataType_Integer)
	{
		sliderRect->SetFloat(kTileValue_user2, setting->data.valueInt);
		snprintf(buf, sizeof buf, "%d", setting->data.valueInt);
	}
	else
	{
		sliderRect->SetFloat(kTileValue_user2, setting->data.valueFloat);
		snprintf(buf, sizeof buf, "%.4f", setting->data.valueFloat);
	}

	if (auto sliderTextRect = sliderRect->parent->GetChildByID(kStewMenu_SliderText))
	{
		if (auto sliderText = sliderTextRect->GetChildByID(kStewMenu_SubsettingInputFieldText))
		{
			sliderText->SetString(kTileValue_string, buf);
		}
	}
	*/
	sliderRect->SetFloat(kTileValue_dragx, 0);
}

void ModConfigurationMenu::HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile)
{
	if (tileID == kStewMenu_SliderDraggableRect)
	{
		auto setting = this->settingsListBox.GetItemForTile(activeTile->parent);
	/*	if (setting->IsSlider())
		{
			auto newValue = activeTile->GetFloat(_ValueTrait);

			auto category = setting->settingCategory;
			auto name = setting->id;

			if (setting->GetDataType() == SubSettingData::kSettingDataType_Float)
			{
				setting->data.valueFloat = newValue;
				ini.SetDoubleValue(category.c_str(), name.c_str(), newValue);
			}
			else
			{
				setting->data.valueInt = newValue;
				ini.SetLongValue(category.c_str(), name.c_str(), newValue);
			}

			SetSliderDisplayedValues(activeTile, setting);

			this->TouchSubsetting(setting);
		}*/
	}

	if (tileID == kStewMenu_SliderLeftArrow || tileID == kStewMenu_SliderRightArrow)
	{
		auto timeClickHeld = InterfaceManager::GetSingleton()->timeLeftClickHeld;
		if (timeClickHeld > 0.3)
		{
			this->HandleActiveSliderArrows(tileID != kStewMenu_SliderLeftArrow);
		}
	}
}

void ModConfigurationMenu::SetCategoriesListActive(bool isVisible)
{
	this->tile->SetFloat("_IsCategoriesActive", isVisible);
}

bool ModConfigurationMenu::GetCategoriesListActive()
{
	return this->tile->GetFloat("_IsCategoriesActive");
}

void ModConfigurationMenu::HandleLeftClick(UInt32 tileID, Tile* clickedTile)
{
	if (clickedTile == categoriesListBox.scrollBar || clickedTile->parent == categoriesListBox.scrollBar)
	{
		this->isDraggingCategoriesSlider = true;
	}
}

bool ModConfigurationMenu::ToggleTweakInINI(SM_Mod* tweak, Tile* tile = nullptr)
{
	auto currentVal = tweak->value;
	if (currentVal != -1)
	{
		// make clicking on non-boolean top-level options (e.g. hotkeys) unset them or do nothing
		if (currentVal || tweak->IsBoolean())
		{
			auto newSettingVal = !currentVal;
			tweak->value = newSettingVal;

			this->TouchTweak(tweak);

			if (tile)
			{
				tile->SetFloat("_selected", newSettingVal);
			}
		}

		return true;
	}
	return false;
}

void ModConfigurationMenu::HandleClick(UInt32 tileID, Tile* clickedTile) 
{
	switch (tileID)
	{
	case kStewMenu_Exit:
	{
		this->Close();
		break;
	}

	case kModConfigurationMenu_ModListItem:
	{
		auto item = modsListBox.GetItemForTile(clickedTile);

		this->SetActiveTweak(item);
		PlayGameSound("UIMenuOK");
/*
		if (!ToggleTweakInINI(tweak, clickedTile))
		{
			PlayGameSound("UIMenuCancel");
			Log(true, Log::kConsole) << FormatString("ModConfigurationMenu: Could not resolve setting: [%s] %s", tweak->GetInternalCategory(), tweak->GetInternalName());
		}*/
		break;
	}

	case kStewMenu_CategoriesButton:
	{
		SetCategoriesListActive(!GetCategoriesListActive());
		break;
	}

	case kStewMenu_CategoryItem:
	{
		// clear all the selected traits from the categories

		auto categoryStr = categoriesListBox.GetItemForTile(clickedTile);
		if (categoryStr)
		{
			this->currentCategory->SetString(kTileValue_string, categoryStr);
		}
		else
		{
			// when clicking <All>
			this->currentCategory->SetString(kTileValue_string, clickedTile->GetValue(kTileValue_string)->str);
		}
		this->selectedCategory = categoryStr;

		categoriesListBox.SaveScrollPosition();
		this->SetCategoriesListActive(false);
		this->RefreshFilter();
		break;
	}
	case kStewMenu_SearchIcon:
	{
		this->SetInSearchMode(true);
		break;
	}
	case kStewMenu_CancelSearch:
	{
		if (this->GetInSearchMode() || this->IsSearchSuspended())
		{
			this->ClearAndCloseSearch();
		}
		else
		{
			this->SetInSearchMode(true);
		}
		break;
	}

	case kStewMenu_ToggleShowActive:
	{
		this->CycleFilterMode();
		break;
	}

	case kStewMenu_SubsettingItem:
	{
		// remove the caret from the original tile if one exists
		this->SetInSubsettingInputMode(false);

		auto setting = settingsListBox.GetItemForTile(clickedTile);

			/*
		if (setting->IsInputField())
		{
			auto inputFieldTextTile = clickedTile->GetChildByID(kStewMenu_SubsettingInputFieldText);
			subSettingInput.tile = inputFieldTextTile;
			if (inputFieldTextTile)
			{
				auto strVal = inputFieldTextTile->GetValue(kTileValue_string);
				auto str = strVal ? strVal->str : "";
				subSettingInput.Set(str);

				activeInputSubsetting = setting;
				this->SetInSubsettingInputMode(true);

				this->TouchSubsetting(setting);
			}
			else
			{
				Log(true, Log::kConsole) << FormatString("ModConfigurationMenu: Failed to find input text tile (ID: %d)", kStewMenu_SubsettingInputFieldText);
			}
		}
		else if (setting->IsCheckboxField())
		{
			bool newSettingValue = setting->data.valueInt == 0;
			setting->data.valueInt = newSettingValue;
			clickedTile->SetFloat(_SelectedTrait, newSettingValue);
			ini.SetLongValue(setting->settingCategory.c_str(), setting->id.c_str(), newSettingValue);

			this->TouchSubsetting(setting);
		}
		else if (setting->IsDropdown())
		{
			this->HandleActiveSliderArrows(true);
		}
		else if (setting->IsHotkeyField())
		{
			activeHotkeySubsetting = setting;
			auto inputFieldTextTile = clickedTile->GetChildByID(kStewMenu_SubsettingInputFieldText);
			if (inputFieldTextTile)
			{
				hotkeyInput.tile = inputFieldTextTile;
				this->SetInHotkeyMode(true);
				this->TouchSubsetting(setting);
			}
		}*/
		break;
	}

	case kModConfigurationMenu_ChoiceText:
	case kStewMenu_SliderLeftArrow:
	case kStewMenu_SliderRightArrow:
	{
		this->HandleActiveSliderArrows(tileID != kStewMenu_SliderLeftArrow);
		break;
	}
	case kStewMenu_SliderText:
	{
		// remove the caret from the original tile if one exists
		this->SetInSubsettingInputMode(false);

		if (auto tile = clickedTile->GetParentByID(kStewMenu_SubsettingItem))
		{
			auto inputFieldTextTile = clickedTile->GetChildByID(kStewMenu_SubsettingInputFieldText);
			subSettingInput.tile = inputFieldTextTile;
			auto setting = settingsListBox.GetItemForTile(tile);
			/*
			if (setting->IsSlider())
			{
				auto strVal = inputFieldTextTile->GetValue(kTileValue_string);
				auto str = strVal ? strVal->str : "";
				subSettingInput.Set(str);

				activeInputSubsetting = setting;
				this->SetInSubsettingInputMode(true);

				this->TouchSubsetting(setting);
			}*/
		}
		break;
	}
	}
}

void ModConfigurationMenu::SetInSearchMode(bool isSearchMode)
{
	searchBar.SetActive(isSearchMode);

	bool isSuspendedSearch = !isSearchMode && !searchBar.GetText().empty();
	this->tile->SetFloat("_IsSearchActive", isSuspendedSearch ? 2 : isSearchMode);

	if (isSearchMode)
	{
		this->SetInSubsettingInputMode(false);
	}
}

bool ModConfigurationMenu::GetInSearchMode()
{
	return searchBar.isActive;
}

bool ModConfigurationMenu::IsSearchSuspended()
{
	return !searchBar.isActive && !searchBar.GetText().empty();
}

// disable the warnings for 'discarding return value of function with 'nodiscard' attribute' 
// since we only need the parse length of the stol and stof calls
#pragma warning( push )
#pragma warning( disable: 4834 )
bool ModConfigurationMenu::IsSubsettingInputValid()
{
	/*
	auto dataType = activeInputSubsetting->GetDataType();
	switch (dataType)
	{
	case SubSettingData::kSettingDataType_String:
	{
		break;
	}
	case SubSettingData::kSettingDataType_Integer:
	{
		try
		{
			size_t parseLen = 0;
			std::stol(subSettingInput.GetText(), &parseLen);

			if (parseLen != subSettingInput.GetLen())
			{
				// reached if user types something like 12dkaodk, since the function will parse the 12, and then return it
				return false;
			}
		}
		catch (...)
		{
			return false;
		}
		break;
	}
	case SubSettingData::kSettingDataType_Float:
	{
		try
		{
			size_t parseLen = 0;
			std::stof(subSettingInput.GetText(), &parseLen);

			if (parseLen != subSettingInput.GetLen())
			{
				return false;
			}
		}
		catch (...)
		{
			return false;
		}
		break;
	}
	}*/
	return true;
}
#pragma warning( pop )

void ModConfigurationMenu::SetInSubsettingInputMode(bool isActive)
{
	auto tile = subSettingInput.tile;
	if (tile)
	{
		if (tile->parent)
		{
			if (auto boxBackground = tile->parent->GetChildByID(kStewMenu_SubsettingInputFieldText_BoxBG))
			{
				boxBackground->SetFloat("_BackgroundVisible", isActive);
			}
		}
	}

	if (isActive)
	{
		subSettingInput.inputType = activeInputSubsetting->type;
	}
	else
	{
		if (activeInputSubsetting)
		{
			if (this->IsSubsettingInputValid())
			{
				this->SetActiveSubsettingValueFromInput();
				this->RefreshActiveTweakSelectionSquare();


				/*
				if (activeInputSubsetting->IsSlider())
				{
					if (tile->parent)
					{
						if (auto sliderTile = tile->parent->parent)
						{
							if (auto sliderRect = sliderTile->GetChildByID(kStewMenu_SliderDraggableRect))
							{
								// update the sliders
								SetSliderDisplayedValues(sliderRect, activeInputSubsetting);

								// recalculate the min/max values
								auto minValue = sliderRect->GetFloat(_MinTrait);
								auto maxValue = sliderRect->GetFloat(_MaxTrait);
								auto value = sliderRect->GetFloat(_ValueTrait);

								sliderRect->SetFloat(_MinTrait, min(minValue, value));
								sliderRect->SetFloat(_MaxTrait, max(maxValue, value));
							}
						}
					}
				}*/
			}
			else
			{
				if (tile->parent)
				{
					// set the input string back to the current value of the setting
					activeInputSubsetting->SetDisplayedValue(tile->parent);

					// ensure the input field has the same value so UpdateCaretDisplay doesn't reset the string
					auto strVal = tile->GetValue(kTileValue_string);
					auto str = strVal ? strVal->str : "";
					subSettingInput.Set(str);
				}
			}

			if (tile)
			{
				tile->SetFloat(kTileValue_systemcolor, 1);
			}
		}

		activeInputSubsetting = nullptr;
	}

	subSettingInput.SetActive(isActive);
}

bool ModConfigurationMenu::GetInSubsettingInputMode()
{
	return subSettingInput.isActive;
}

void ModConfigurationMenu::SetInHotkeyMode(bool isActive)
{
	auto tile = hotkeyInput.tile;
	if (tile)
	{
		if (tile->parent)
		{
			if (auto boxBackground = tile->parent->GetChildByID(kStewMenu_SubsettingInputFieldText_BoxBG))
			{
				boxBackground->SetFloat("_BackgroundVisible", isActive);
			}
		}
	}

	if (activeHotkeySubsetting)
	{
		if (isActive)
		{
			hotkeyInput.value = activeHotkeySubsetting->data.valueInt;
			if (tile)
			{
				tile->SetString(kTileValue_string, "...");
			}
		}
		else
		{
			activeHotkeySubsetting->data.valueInt = hotkeyInput.value;
			if (tile->parent)
			{
				activeHotkeySubsetting->SetDisplayedValue(tile->parent);
			}
			activeHotkeySubsetting = nullptr;
		}
	}

	hotkeyInput.SetActive(isActive);
}

bool ModConfigurationMenu::GetInHotkeyMode()
{
	return hotkeyInput.isActive;
}

bool __cdecl FilterTweaksOnActiveTrait(SM_Mod* item)
{
	auto filterMode = ModConfigurationMenu::GetSingleton()->filterMode;
	if (filterMode == FilterMode::kFilterMode_ShowAll) return false;

	if (item->value == 0)
	{
		return filterMode == kFilterMode_ShowActive;
	}

	return filterMode == kFilterMode_ShowInActive;
}

bool __cdecl FilterOnSelectedCategory(SM_Mod* item)
{
	const auto selectedCategory = ModConfigurationMenu::GetSingleton()->selectedCategory;
	return !selectedCategory.empty() && item->category != selectedCategory;
}

bool __cdecl HideItemsNotMatchingFilterString(SM_Mod* item)
{
	const auto searchStr = ModConfigurationMenu::GetSingleton()->searchBar.GetText();
	if (item->name.find(searchStr) != std::string::npos) return false;
	if (item->settingName.find(searchStr) != std::string::npos) return false;
	if (item->description.find(searchStr) != std::string::npos) return false;
	return true;
}

bool __cdecl TweakFilter(SM_Mod* item)
{
	if (FilterTweaksOnActiveTrait(item)) return true;
	if (HideItemsNotMatchingFilterString(item)) return true;
	return FilterOnSelectedCategory(item);
}

void ModConfigurationMenu::RefreshFilter()
{
	modsListBox.Filter(TweakFilter);

	auto textColor = modsListBox.GetNumVisibleItems() ? 1 : 2;
	searchBar.tile->SetFloat(kTileValue_systemcolor, textColor);

	if (auto selectedTile = modsListBox.GetTileFromItem(&activeTweak))
	{
		// if the selected tweak is filtered out
		if (selectedTile->GetFloat(kTileValue_listindex) < 0)
		{
			this->SetActiveTweak(nullptr);
		}
	}
}

void ModConfigurationMenu::CycleFilterMode()
{
	++filterMode;
	if (filterMode >= kFilterMode_Count) filterMode = kFilterMode_ShowAll;
	this->tile->SetFloat("_FilterMode", filterMode);
	this->RefreshFilter();
}

int reloadTweaksMenuFrameDelay;
void __fastcall ReloadTweaksMenuInOneFrameHook(void* startMenu)
{
	ThisCall(0x7D5830, startMenu);
	if (!--reloadTweaksMenuFrameDelay)
	{
		WriteRelCall(0x7CFA8C, 0x7D5830);
		ShowTweaksMenu();
	}
}

// reloads the entire menu by closing it, and writing a hook in StartMenu::Update which waits a couple frames and then reopens the menu
void ModConfigurationMenu::ReloadMenuXML()
{
	this->Close();
	WriteRelCall(0x7CFA8C, UInt32(ReloadTweaksMenuInOneFrameHook));
	reloadTweaksMenuFrameDelay = 2;
}

bool ModConfigurationMenu::XMLHasChanges()
{
	if (lastXMLWriteTime.dwLowDateTime == 0 && lastXMLWriteTime.dwHighDateTime == 0)
	{
		return true;
	}

	HANDLE tweaksXMLHandle = CreateFile(MenuPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (tweaksXMLHandle != INVALID_HANDLE_VALUE)
	{
		FILETIME lastWriteTime;
		if (GetFileTime(tweaksXMLHandle, nullptr, nullptr, &lastWriteTime))
		{
			CloseHandle(tweaksXMLHandle);
			return lastXMLWriteTime.dwLowDateTime != lastWriteTime.dwLowDateTime || lastXMLWriteTime.dwHighDateTime != lastWriteTime.dwHighDateTime;
		}

		CloseHandle(tweaksXMLHandle);
	}
	return true;
}

void ModConfigurationMenu::ClearAndCloseSearch()
{
	searchBar.Set("");
	this->SetInSearchMode(false);
	this->RefreshFilter();
}

bool ModConfigurationMenu::HandleKeyboardInput(UInt32 key) 
{
	if (IsControlHeld())
	{
		if ((key | 0x20) == 'f')
		{
			this->SetInSearchMode(!this->GetInSearchMode());
			return true;
		}
		else if ((key | 0x20) == 'r')
		{
			if (this->GetInSubsettingInputMode())
			{
				subSettingInput.Set("");
			}
			else if (this->GetInSearchMode() || this->IsSearchSuspended())
			{
				this->ClearAndCloseSearch();
			}
			return true;
		}
	}

	if (this->GetInSearchMode())
	{
		if (searchBar.HandleKey(key))
		{
			this->RefreshFilter();
			return true;
		}
	}
	else if (this->GetInSubsettingInputMode())
	{
		if (key == kInputCode_Enter)
		{
			// handle it here instead of in the InputField since we want to update the setting string
			this->SetInSubsettingInputMode(false);
			return true;
		}
		else if (key != '`' && key != '\t')
		{
			if (subSettingInput.HandleKey(key))
			{
				subSettingInput.tile->SetFloat(kTileValue_systemcolor, this->IsSubsettingInputValid() ? 1 : 2);
			}
			else
			{
				PlayGameSound("UIMenuCancel");
			}
			return true;
		}
	}
	else if (this->GetInHotkeyMode())
	{
		// ignore all keyboard input while in hotkey mode, hotkeys are checked in ModConfigurationMenu::Update to allow checking function keys etc.
		return true;
	}
	else if ((key | 0x20) == 'r')
	{
		if (IsShiftHeld())
		{
//			this->WriteAllChangesToINIs();

//			ReloadINIs();
		}
		else
		{
			ReloadMenuXML();
		}
		return true;
	}

	return false;
}

UInt32 ModConfigurationMenu::GetID(void)
{
	return UInt32(MENU_ID);
}

bool ModConfigurationMenu::HandleActiveSliderArrows(bool isRightArrow, float scale)
{
	if (auto tile = this->settingsListBox.selected)
	{
		auto setting = this->settingsListBox.GetItemForTile(tile);

		/*
		if (setting->IsSlider())
		{
			if (auto sliderRect = tile->GetChildByID(kStewMenu_SliderDraggableRect))
			{
				auto minValue = sliderRect->GetFloat(_MinTrait);
				auto maxValue = sliderRect->GetFloat(_MaxTrait);

				auto currentSettingValue = sliderRect->GetFloat(_ValueTrait);
				auto addend = (maxValue - minValue) * scale;
				if (!isRightArrow) addend = -addend;
				auto newValue = max(minValue, min(maxValue, currentSettingValue + addend));

				auto category = setting->settingCategory;
				auto name = setting->id;

				if (setting->GetDataType() == SubSettingData::kSettingDataType_Float)
				{
					setting->data.valueFloat = newValue;
					ini.SetDoubleValue(category.c_str(), name.c_str(), newValue);
				}
				else
				{
					if (isRightArrow) newValue = ceil(newValue);
					setting->data.valueInt = newValue;
					ini.SetLongValue(category.c_str(), name.c_str(), newValue);
				}

				SetSliderDisplayedValues(sliderRect, setting);

				this->TouchSubsetting(setting);

				return true;
			}
		}
		else if (setting->IsDropdown())
		{
			setting->data.DropdownSelectNext(isRightArrow);

			SetDisplayedValuesForSubsetting(tile, setting);
			this->SetSubsettingDescriptionTileString(setting);

			ini.SetLongValue(setting->settingCategory.c_str(), setting->id.c_str(), setting->data.valueInt);
			this->TouchSubsetting(setting);

			return true;
		}*/
	}

	return false;
}

bool ModConfigurationMenu::HandleSpecialKeyInput(MenuSpecialKeyboardInputCode code, float keyState)
{
	switch (code)
	{
	case kMenu_Tab:
	{
		if (this->GetInSearchMode())
		{
			this->SetInSearchMode(false);
		}
		else if (this->GetCategoriesListActive())
		{
			this->SetCategoriesListActive(false);
		}
		else if (this->GetInSubsettingInputMode())
		{
			this->SetInSubsettingInputMode(false);
		}
		else
		{
			this->Close();
		}
		return true;
	}
	case kMenu_ShiftLeft:
	{
		if (modsListBox.GetSelectedTile() && categoriesListBox.GetNumVisibleItems())
		{
			this->SetCategoriesListActive(true);
			modsListBox.SaveScrollPosition();
			this->HandleMouseover(kStewMenu_CategoriesBackground, nullptr);
			categoriesListBox.RestorePosition();
		}
		else if (settingsListBox.GetSelectedTile() && modsListBox.GetNumVisibleItems())
		{
			settingsListBox.SaveScrollPosition();
			this->HandleMouseover(kStewMenu_TweaksListBackground, nullptr);
			modsListBox.RestorePosition();
		}
		return true;
	}
	case kMenu_ShiftRight:
	{
		if (categoriesListBox.GetSelectedTile() && modsListBox.GetNumVisibleItems())
		{
			this->SetCategoriesListActive(false);
			categoriesListBox.SaveScrollPosition();
			this->HandleMouseover(kStewMenu_TweaksListBackground, nullptr);
			modsListBox.RestorePosition();
		}
		else if (modsListBox.GetSelectedTile() && settingsListBox.GetNumVisibleItems())
		{
			modsListBox.SaveScrollPosition();
			this->HandleMouseover(kStewMenu_SubSettingsListBackground, nullptr);
			settingsListBox.RestorePosition();
		}
		return true;
	}
	case kMenu_LeftArrow:
	case kMenu_RightArrow:
	{
		if (this->HandleActiveSliderArrows(code == kMenu_RightArrow))
		{
			return true;
		}
		break;
	}
	case kMenu_PageDown:
	case kMenu_PageUp:
	{
		bool isRightArrow = code == kMenu_PageUp;

		if (IsControllerConnected())
		{
			// page up and page down are inverted on controller for some reason
			isRightArrow = !isRightArrow;
		}

		return this->HandleActiveSliderArrows(isRightArrow, 0.2);
	}
	}
	return false;
}

bool ModConfigurationMenu::HandleControllerInput(int code, Tile* tile)
{
	this->SetInSearchMode(false);
	if (this->GetInSubsettingInputMode() && code != OSInputGlobals::kXboxCtrl_BUTTON_A)
	{
		this->SetInSubsettingInputMode(false);
	}

	if (code == OSInputGlobals::kXboxCtrl_L3 || code == OSInputGlobals::kXboxCtrl_R3)
	{
		this->HandleClick(kStewMenu_ToggleShowActive, nullptr);
		return true;
	}

	return false;
}

void ModConfigurationMenu::SetActiveSubsettingValueFromInput()
{
	/*
	auto category = activeInputSubsetting->settingCategory;
	auto name = activeInputSubsetting->id;

	auto dataType = activeInputSubsetting->GetDataType();
	switch (dataType)
	{
	case SubSettingData::kSettingDataType_String:
	{
		activeInputSubsetting->data.valueStr = subSettingInput.GetText();
		ini.SetValue(category.c_str(), name.c_str(), activeInputSubsetting->data.valueStr.c_str());
		break;
	}
	case SubSettingData::kSettingDataType_Integer:
	{
		activeInputSubsetting->data.valueInt = std::stol(subSettingInput.GetText());
		ini.SetLongValue(category.c_str(), name.c_str(), activeInputSubsetting->data.valueInt);
		break;
	}
	case SubSettingData::kSettingDataType_Float:
	{
		activeInputSubsetting->data.valueFloat = std::stof(subSettingInput.GetText());
		ini.SetDoubleValue(category.c_str(), name.c_str(), activeInputSubsetting->data.valueFloat);
		break;
	}
	}*/
}

void ModConfigurationMenu::SetSubsettingValueFromINI(SM_Setting* setting)
{
//	auto category = setting->settingCategory;
	auto name = setting->id;
	bool isInvalidSetting = false;
	/*
	switch (setting->type)
	{
	case SubSettingData::kSettingDataType_Integer:
	{
		setting->data.valueInt = ini.GetLongValue(category.c_str(), name.c_str(), -14356);
		if (setting->data.valueInt == -14356)
		{
			setting->data.valueInt = ini.GetDoubleValue(category.c_str(), name.c_str(), -14356);
			if (setting->data.valueInt == -14356)
			{
				setting->data.valueInt = 0;
				isInvalidSetting = true;
			}
		}
		break;
	}
	case SubSettingData::kSettingDataType_Float:
	{
		setting->data.valueFloat = ini.GetDoubleValue(category.c_str(), name.c_str(), -14356);
		if (setting->data.valueFloat == -14356)
		{
			setting->data.valueFloat = 0;
			isInvalidSetting = true;
		}
		break;
	}
	case SubSettingData::kSettingDataType_String:
	{
		if (setting->data.valueStr.c_str())
		{
			GameHeapFree(setting->data.valueStr.c_str());
		}
		auto str = ini.GetValue(category.c_str(), name.c_str(), nullptr);
		if (!str)
		{
			str = "";
			isInvalidSetting = true;
		}
		setting->data.valueStr = str;
		break;
	}
	}

	if (isInvalidSetting)
	{
		Log(true, Log::kConsole) << FormatString("ModConfigurationMenu: Failed to resolve subsetting: [%s] %s", setting->settingCategory.c_str(), setting->id.c_str());
	}*/
}
/*
void SetDisplayedValuesForSubsetting(Tile* tile, SM_Setting* setting)
{
	char buf[0x40];

	auto dataType = setting->GetDataType();
	switch (setting->GetElementType())
	{
	case SubSettingData::kSettingType_Choice:
	{
		tile->SetFloat("_selected", setting->data.valueInt);
		break;
	}
	case SubSettingData::kSettingType_Slider:
	{
		if (auto sliderRect = tile->GetChildByID(kStewMenu_SliderDraggableRect))
		{
			SetSliderDisplayedValues(sliderRect, setting);
		}
		break;
	}
	case SubSettingData::kSettingType_StringInput:
	{
		if (auto textField = tile->GetChildByID(kStewMenu_SubsettingInputFieldText))
		{
			textField->SetString(kTileValue_string, setting->data.valueStr.c_str());
		}
		break;
	}
	case SubSettingData::kSettingType_NumberInput:
	{
		if (auto textField = tile->GetChildByID(kStewMenu_SubsettingInputFieldText))
		{
			if (dataType == SubSettingData::kSettingDataType_Integer)
			{
				snprintf(buf, sizeof buf, "%d", setting->data.valueInt);
			}
			else
			{
				snprintf(buf, sizeof buf, "%.4f", setting->data.valueFloat);
			}
			textField->SetString(kTileValue_string, buf, 1);
		}
		break;
	}
	case SubSettingData::kSettingType_Options:
	{
		if (auto textField = tile->GetChildByID(kModConfigurationMenu_ChoiceText))
		{
			textField->SetString(kTileValue_string, setting->data.GetDropdownValue());
		}
		break;
	}
	case SubSettingData::kSettingType_Hotkey:
	{
		if (auto textField = tile->GetChildByID(kStewMenu_SubsettingInputFieldText))
		{
			auto hotkeyStr = ScancodeToString(setting->data.valueInt);
			textField->SetString(kTileValue_string, hotkeyStr.c_str(), 1);
		}
		break;
	}
	}
}
*/

void SM_Setting::SetDisplayedValue(Tile* tile)
{
	if (type == kSettingType_Choice)
	{
		const auto value = ReadINI(setting);
		if (std::holds_alternative<SInt32>(value))
		{
			tile->SetString(kTileValue_user0, values[std::get<SInt32>(value)].first.c_str());
		}
	}
}

void ModConfigurationMenu::SetActiveTweak(SM_Mod* tweak)
{
	this->SetInSubsettingInputMode(false);
	this->SetInHotkeyMode(false);
	subSettingInput.tile = nullptr;
	hotkeyInput.tile = nullptr;

	settingsListBox.FreeAllTiles();
	activeTweak = tweak;

	if (tweak)
	{
		for (const auto& it : g_Settings)
		{
			if (!it->mods.contains(tweak->id)) continue;

//			this->SetSubsettingValueFromINI(it);
			const auto tile = settingsListBox.Insert(it.get());
			it->SetDisplayedValue(tile);
		}
	}
}

void ModConfigurationMenu::SetTweakDescriptionTileString(SM_Mod* tweak)
{
	this->selectionText->SetString(kTileValue_string, FormatString("%s\n- %s", tweak->description.c_str(), tweak->settingName).c_str());
}

void ModConfigurationMenu::SetSubsettingDescriptionTileString(SM_Setting* setting)
{
//	auto description = setting->description;
	/*if (setting->IsDropdown())
	{
		
		description = FormatString(!description.empty() ? "%s\r\n%s" : "%s%s", description, setting->data.GetDropdownDescription());
	}*/

	if (g_bTweaksMenuShowSettingPathOnSubsettings)
	{
//		description = FormatString(!description.empty() ? "%s\n[%s]\n%s" : "%s[%s]\n%s", description, setting->settingCategory.c_str(), setting->id.c_str());
	}

//	this->selectionText->SetString(kTileValue_string, description.c_str());
}

void ModConfigurationMenu::HandleMouseover(UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_ModListItem:
	{

		modsListBox.GetItemForTile(activeTile);

		auto item = modsListBox.GetItemForTile(activeTile);
		this->SetTweakDescriptionTileString(item);
//		this->SetActiveTweak(item);
//		PlayGameSound("UIMenuFocus");

		break;
	}

	case kStewMenu_SliderDraggableRect:
	case kStewMenu_SliderLeftArrow:
	case kStewMenu_SliderRightArrow:
	{
		auto item = settingsListBox.GetItemForTile(activeTile->parent);
		this->SetSubsettingDescriptionTileString(item);
		break;
	}

	case kStewMenu_SubsettingItem:
	{
		auto item = settingsListBox.GetItemForTile(activeTile);
		this->SetSubsettingDescriptionTileString(item);
		break;
	}

	case kStewMenu_CategoryItem:
	{
		PlayGameSound("UIMenuFocus");
		break;
	}

	case kStewMenu_TweaksListBackground:
	{
		break;
	}

	case kStewMenu_CategoriesBackground:
	{
		break;
	}

	case kStewMenu_SubSettingsListBackground:
	{
		break;
	}
	case kModConfigurationMenu_ChoiceText:
	{
		break;
	}
	}
}

void ModConfigurationMenu::HandleUnmouseover(UInt32 tileID, Tile* tile)
{
//	modsListBox.SetSelected(nullptr);
//	settingsListBox.SetSelected(nullptr);
	selectionText->SetString(kTileValue_string, "");
}

void ModConfigurationMenu::Free() 
{

	modsListBox.Destroy();
	categoriesListBox.Destroy();
	settingsListBox.Destroy();
	searchBar.Free();
	subSettingInput.Free();
	hotkeyInput.Free();
	ini.Reset();

	allCategories.clear();
	touchedSubsettings.RemoveAll();
	touchedTweaks.RemoveAll();

	Menu::Free();

	OSInputGlobals::GetSingleton()->SetDebounceMenuMode(false);
};

void CopyValuesFrom(CSimpleIni& to, CSimpleIni& from)
{
	std::list<CSimpleIni::Entry> categoryList;
	std::list<CSimpleIni::Entry> keyList;
	from.GetAllSections(categoryList);
	for (auto categoryEntry : categoryList)
	{
		const char* category = categoryEntry.pItem;
		from.GetAllKeys(category, keyList);
		for (auto keyEntry : keyList)
		{
			const char* key = keyEntry.pItem;
			auto value = from.GetValue(category, key);
			to.SetValue(category, key, value, nullptr, true);
		}
	}
}

/*
void ModConfigurationMenu::LoadINIs()
{
	char INIPath[MAX_PATH];
	auto errVal = ini.LoadFile(TweaksINIPath);
	if (errVal == SI_Error::SI_FILE)
	{
		Log(true, Log::kConsole) << FormatString("ModConfigurationMenu:: Failed to read INI");
		return;
	}

	
	if (g_bMultiINISupport)
	{
		CSimpleIni tempINI;
		tempINI.SetUnicode();

		char* namePtr = StrLenCopy(INIPath, tweaksINIsPath, 30); // keep pointer "namePtr" to the end of the string
		MemCopy(namePtr, "*.ini", 6); // and append "*.ini" to INIPath

		for (DirectoryIterator dirIter(INIPath); dirIter; ++dirIter)
		{
			if (!dirIter.IsFile()) continue;
			StrCopy(namePtr, dirIter.Get());

			tempINI.LoadFile(INIPath);
			CopyValuesFrom(ini, tempINI);

			tempINI.Reset();
		}
	}
}

void ModConfigurationMenu::WriteTweakToINI(SM_Mod* tweak)
{
	char buf[0x40];
	snprintf(buf, sizeof buf, "%d", tweak->value);
	WriteValueToINIs(tweak->GetInternalCategory(), tweak->GetInternalName(), buf);
}

void ModConfigurationMenu::WriteSubsettingToINI(SM_Setting* setting)
{
	std::string str;
	switch (setting->GetDataType()) {
	case SubSettingData::kSettingDataType_Float:
	{
		str = FormatString("%.6f", setting->data.valueFloat);
		break;
	}
	case SubSettingData::kSettingDataType_Integer:
	{
		str = FormatString("%d", setting->data.valueInt);
		break;
	}
	case SubSettingData::kSettingDataType_String:
	{
		str = setting->data.valueStr;
		break;
	}
	default:
	{
		return;
	}
	}

	WriteValueToINIs(setting->GetInternalCategory(), setting->GetInternalName(), str.c_str());
}
*/

/*

bool IniContainsValue(const char* category, const char* name, const char* fileName)
{
	char buf[4];
	UInt8 numCharactersRead = GetPrivateProfileString(category, name, "", buf, sizeof buf, fileName);
	return numCharactersRead != 0;
}

void WriteValueToINIs(const char* category, const char* name, const char* newValue)
{
	char INIPath[MAX_PATH];
	if (g_bMultiINISupport)
	{
		// loop over all the INIs in reverse order and pick the first one that has the key/name in the ini

		TList<const char> iniPaths;
		iniPaths.Init(nullptr);

		char* namePtr = StrLenCopy(INIPath, tweaksINIsPath, 30);
		MemCopy(namePtr, "*.ini", 6);

		// create a list of the INIs in reverse order
		DirectoryIterator dirIter(INIPath);
		for (; dirIter; ++dirIter)
		{
			if (!dirIter.IsFile()) continue;
			iniPaths.Insert(GameHeapStrdup(dirIter.Get()));
		}

		for (const auto iter : iniPaths)
		{
			StrCopy(namePtr, iter);
			if (IniContainsValue(category, name, INIPath))
			{
				WritePrivateProfileString(category, name, newValue, INIPath);
				iniPaths.DeleteAll();
				return;
			}
		}

		iniPaths.DeleteAll();
	}
	
	WritePrivateProfileString(category, name, newValue, TweaksINIPath);
}*/

void ModConfigurationMenu::Destructor(bool doFree) 
{
	this->Free();
	if (doFree) GameHeapFree(this);
	g_stewMenu = nullptr;
}

int g_bShownTweaksMenuReloadWarning = false;
void RestartGameWarningCallback()
{
	enum { kDontShowThisAgain = 1 };

	if (InterfaceManager::GetSingleton()->GetMessageBoxResult() == kDontShowThisAgain)
	{
		g_bShownTweaksMenuReloadWarning = 1;
//		WriteValueToINIs("INI", "bShownTweaksMenuReloadWarning", " 1");
	}
}

void ModConfigurationMenu::WriteAllChangesToINIs()
{
	for (auto iter : touchedTweaks)
	{
//		this->WriteTweakToINI(iter);
	}

	for (auto iter : touchedSubsettings)
	{
//		this->WriteSubsettingToINI(iter);
	}
}

void ModConfigurationMenu::Close()
{
	if (!g_bShownTweaksMenuReloadWarning)
	{
		if (!touchedTweaks.Empty() || !touchedSubsettings.Empty())
		{
			ShowMessageBox("You must restart the game for changes to be applied!", 0, 0, RestartGameWarningCallback, 0, 0x17, 0, 0, (char*)0x103934C, "Don't show this again", NULL); // OK
		}
	}

	this->WriteAllChangesToINIs();

	Menu::Close();
}

void ModConfigurationMenu::HandleControllerConnectOrDisconnect(bool isControllerConnected)
{

}

void ModConfigurationMenu::SetCursorPosTraits()
{
	auto im = InterfaceManager::GetSingleton();

	static float lastX;
	static float lastY;

	auto xPos = im->cursorX;
	auto yPos = im->cursorY;
	if (xPos != lastX || yPos != lastY)
	{
		lastX = xPos;
		lastY = yPos;
		tile->SetFloat("_CursorX", im->cursorX);
		tile->SetFloat("_CursorY", im->cursorY);
	}
}

void ModConfigurationMenu::Update() 
{
	searchBar.Update();
	subSettingInput.Update();
	auto input = OSInputGlobals::GetSingleton();

	if (input->GetControlState(OSInputGlobals::Escape_, OSInputGlobals::isPressed) && !this->GetInHotkeyMode())
	{
		this->Close();
		return;
	}

	// close the search and categories list when clicking elsewhere
	if (input->GetMouseState(OSInputGlobals::kLeftClick, OSInputGlobals::isDepressed))
	{
		auto activeTileID = -1;
		if (auto tile = InterfaceManager::GetSingleton()->GetActiveTile())
		{
			activeTileID = tile->GetFloat(kTileValue_id);
		}

		if (activeTileID != kStewMenu_SearchIcon && activeTileID != kStewMenu_CancelSearch)
		{
			this->SetInSearchMode(false);
		}

		if (activeTileID != kStewMenu_CategoriesButton && !isDraggingCategoriesSlider)
		{
			this->SetCategoriesListActive(false);
		}

		if (activeTileID != kStewMenu_SubsettingItem && activeTileID != kStewMenu_SliderText)
		{
			if (this->GetInSubsettingInputMode())
			{
				this->SetInSubsettingInputMode(false);
			}
		}

		isDraggingCategoriesSlider = false;
	}

	if (this->GetInHotkeyMode())
	{
		auto pressedKey = hotkeyInput.Update();
		if (pressedKey)
		{
			if (pressedKey != Tilde && activeHotkeySubsetting)
			{
				if (pressedKey == _Escape)
				{
					PlayGameSound("UIMenuCancel");
					pressedKey = 0;
				}
				else
				{
					PlayGameSound("UIPopUpMessageGeneral");
				}

				activeHotkeySubsetting->data.valueInt = pressedKey;
//				ini.SetLongValue(activeHotkeySubsetting->settingCategory.c_str(), activeHotkeySubsetting->id.c_str(), pressedKey);
				this->RefreshActiveTweakSelectionSquare();
			}
			this->SetInHotkeyMode(false);
		}
	}

	if (IsControllerConnected())
	{
		auto rightStickX = XINPUT_GAMEPAD_EX::GetCurrent()->sThumbRX;
		if (rightStickX)
		{
			int deadzoneRS = *(UInt32*)0x9455F5;
			if (abs(rightStickX) > deadzoneRS)
			{
				if (auto activeSubsetting = settingsListBox.GetSelected())
				{
					/*
					if (activeSubsetting->IsSlider())
					{
						float ratio = abs(rightStickX) / 32767.0F;
						this->HandleActiveSliderArrows(rightStickX > 0, ratio * 0.001);
					}*/
				}
			}
		}
	}

	SetCursorPosTraits();
}


void __fastcall OnAltTabReloadStewMenu(OSGlobals* osGlobals, void* edx, int isActive)
{
	ThisCall(0x871C90, osGlobals, isActive);
	ModConfigurationMenu* menu = ModConfigurationMenu::GetSingleton();
	if (menu && menu->XMLHasChanges())
	{
		menu->ReloadMenuXML();
	}
}

bool ModConfigurationMenu::HasTiles()
{
	for (Tile* tile : this->tiles)
	{
		if (!tile) return false;
	}

	if (!searchBar.tile) return false;

	return true;
}

void __cdecl SetTweakSelectedBoxIfEnabled(Tile* tile, SM_Mod* tweak)
{
	auto ini = &ModConfigurationMenu::GetSingleton()->ini;
	auto isEnabled = ini->GetLongValue(tweak->GetInternalCategory(), tweak->GetInternalName(), -10063);
	tweak->value = isEnabled;
	if (isEnabled != -10063)
	{
		tile->SetFloat("_selected", isEnabled != 0);
	}
	else
	{
		tile->SetString(kTileValue_clicksound, "");
		tile->SetFloat("_selected", false);
		tile->SetFloat("_TextAlpha", 128);
	}
}

void ModConfigurationMenu::RefreshActiveTweakSelectionSquare()
{
	if (activeTweak)
	{
		if (auto tile = modsListBox.GetTileFromItem(&activeTweak))
		{
			SetTweakSelectedBoxIfEnabled(tile, activeTweak);
		}
	}
}

signed int __cdecl CompareTweaksAlphabetically(ListBoxItem<SM_Mod>* a, ListBoxItem<SM_Mod>* b)
{
	auto itemA = a->object;
	auto itemB = b->object;

	if (itemA->priority == itemB->priority)
	{
		return (itemA->name <=> itemB->name)._Value;
	}

	return itemA->priority > itemB->priority ? -1 : 1;
}

static bool hasAddedNewTrait = false;

void ShowTweaksMenu()
{
	Tile* prevMenu = Menu::GetTileMenu(MENU_ID);
	if (prevMenu) prevMenu->Destroy(true);

	// TODO: Font setup, also LOL stewie running fucking console lmao
	// setup the menu font (requires JIP LN NVSE)
//	consoleItfc->RunScriptLine2("SetFontFile 42 \"textures\\fonts\\Monofonto_STn.fnt\"", nullptr, true);

	const auto newDepth = StdCall<Float32>(0xA1DFB0);

	if (!hasAddedNewTrait)
	{
		hasAddedNewTrait = true;
		CdeclCall(0x9FF8A0, "&UMPMenu;", MENU_ID);
	}

	// allow hot-reloading the menu even if UIO is installed
	UInt32 previousResolveXMLFile = DetourRelCall(0xA01B87, 0xA01E20);

	ModConfigurationMenu::Create();
	Tile* tile = InterfaceManager::GetSingleton()->menuRoot->ReadXML(MenuPath);
	WriteRelCall(0xA01B87, previousResolveXMLFile);

	auto menu = (ModConfigurationMenu*)tile->GetParentMenu();

	if (!menu)
	{
		Log(true, Log::kConsole) << FormatString("MENUS: Stew Menu Creation Failed...");
		return;
	}

	if (menu->GetID() != MENU_ID)
	{
		Log(true, Log::kConsole) << FormatString(
			"MENUS: Expected Stew Menu to have <class> %d </class> but it had class %d...", MENU_ID, menu->GetID());
		menu->Destructor(true);
		return;
	}

	menu->RegisterTile(tile, 0);

	if (!menu->HasTiles())
	{
		menu->Close();
		Log(true, Log::kConsole) << FormatString("MENUS: Stew Menu is missing some expected tiles...");
		return;
	}

	g_stewMenu = menu;
	float stackingType = tile->GetFloat(kTileValue_stackingtype);
	if (stackingType == 6006.0 || stackingType == 102.0)
	{
		tile->SetFloat(kTileValue_depth, newDepth, 1);
	}

	Setting* sSettings = (Setting*)0x11D1FE0;
	menu->menuTitle->SetString(kTileValue_string, FormatString("WAAAAAAAAAAAAAAA %s", sSettings->data.str).c_str());

	auto tweaksListBox = &menu->modsListBox;
	tweaksListBox->parentTile = menu->tweaksListBackground;
	tweaksListBox->templateName = "TweakTemplate";
	tweaksListBox->scrollBar = tweaksListBox->parentTile->GetByTraitName("child(lb_scrollbar)");

	auto categoriesListBox = &menu->categoriesListBox;
	categoriesListBox->parentTile = menu->categoriesBackground;
	categoriesListBox->templateName = "CategoryTemplate";
	categoriesListBox->scrollBar = categoriesListBox->parentTile->GetByTraitName("child(lb_scrollbar)");

	auto subSettingsListBox = &menu->settingsListBox;
	subSettingsListBox->parentTile = menu->subSettingsListBackground;
	subSettingsListBox->templateName = "SubSettingTemplate_Slider";
	subSettingsListBox->scrollBar = subSettingsListBox->parentTile->GetByTraitName("child(lb_scrollbar)");

	menu->ReadJSONForPath(GetCurPath() + R"(\Data\menus\)");

	for (const auto& mod : menu->g_Mods)
	{
		const auto tile = menu->modsListBox.Insert(mod.get(), mod->name.c_str());
		tile->SetFloat(kTileValue_id, kModConfigurationMenu_ModListItem);
	}

	tweaksListBox->ForEach(SetTweakSelectedBoxIfEnabled);
	tweaksListBox->Sort(CompareTweaksAlphabetically);

	Tile* listTile = categoriesListBox->Insert(nullptr, "All");
	if (listTile) listTile->SetFloat(kTileValue_id, kStewMenu_CategoryItem);

	for (const auto iter : menu->allCategories)
	{
		char* categoryName = iter;
		listTile = categoriesListBox->Insert(categoryName, categoryName);
		if (listTile)
		{
			listTile->SetFloat(kTileValue_id, kStewMenu_CategoryItem);
		}
	}

	ModConfigurationMenu::GetSingleton()->SetActiveTweak(tweaksListBox->list.first.data->object);

	menu->HideTitle(false);
}

void ModConfigurationMenu::AddCategory(char* category)
{
	allCategories.insert(category);
}

void ModConfigurationMenu::TouchTweak(SM_Mod* tweak)
{
	if (!this->touchedTweaks.contains(tweak))
	{
		touchedTweaks.Insert(tweak);
	}
}

void ModConfigurationMenu::TouchSubsetting(SM_Setting* setting)
{
	if (!this->touchedSubsettings.contains(setting))
	{
		touchedSubsettings.Insert(setting);
	}
}

void patchAddTweaksButton()
{

}