#include "ConfigurationMenuStew.h"
#include <sstream>

#include "dinput8.h"
#include "functions.h"
#include "GameSettings.h"
#include "InterfaceManager.h"
#include "SafeWrite.h"

const char* MenuPath = R"(Data\Menus\ConfigurationMenu.xml)";

ModConfigurationMenu* g_ConfigurationMenu;

ModConfigurationMenu* ModConfigurationMenu::GetSingleton() { return g_ConfigurationMenu; };

UInt32 ModConfigurationMenu::GetID() { return static_cast<UInt32>(MENU_ID); }

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

void ModConfigurationMenu::SetTile(UInt32 tileID, Tile* tile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_Title:
		this->menuTitle = tile;
		break;
	case kModConfigurationMenu_ModList:
		this->tweaksListBackground = tile;
		break;
	case kModConfigurationMenu_SelectionText:
		this->selectionText = tile;
		break;
	case kModConfigurationMenu_Exit:
		this->doneTile = tile;
		break;
	case kModConfigurationMenu_SearchBar:
		this->searchBar.tile = tile;
		break;
	case kModConfigurationMenu_SearchIcon:
		this->searchIcon = tile;
		break;
	case kModConfigurationMenu_CategoriesBackground:
		this->categoriesBackground = tile;
		break;
	case kModConfigurationMenu_CategoriesButton:
		this->categoriesButton = tile;
		break;
	case kModConfigurationMenu_CancelSearch:
		this->searchCancel = tile;
		break;
	case kModConfigurationMenu_ActiveCategory:
		this->currentCategory = (TileText*)tile;
		break;
	case kModConfigurationMenu_ToggleShowActive:
		this->showActive = tile;
		break;
	case kModConfigurationMenu_SettingList:
		this->subSettingsListBackground = tile;
		break;
	}
}

void ModConfigurationMenu::HandleMousewheel(UInt32 tileID, Tile* tile)
{

}

void ModConfigurationMenu::HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile)
{
	if (tileID == kModConfigurationMenu_SliderDraggableRect)
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

	if (tileID == kModConfigurationMenu_SliderLeftArrow || tileID == kModConfigurationMenu_SliderRightArrow)
	{
		if (InterfaceManager::GetSingleton()->timeLeftClickHeld > 0.3)
		{
			const auto option = activeTile->parent;
			settingsListBox.GetItemForTile(option)->Next(tileID != kModConfigurationMenu_SliderLeftArrow)->Display(option);
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

void SetTweakNotSelected(Tile* tile, SM_Mod* tweak)
{
	tile->SetFloat("_selected", 0);
}

void ModConfigurationMenu::HandleClick(UInt32 tileID, Tile* clickedTile) 
{
	switch (tileID)
	{
	case kModConfigurationMenu_DeviceButton:
	{
		if (controlDevice == OSInputGlobals::kControlType_Keyboard) controlDevice = OSInputGlobals::kControlType_Mouse;
		else if (controlDevice == OSInputGlobals::kControlType_Mouse) controlDevice = OSInputGlobals::kControlType_Joystick;
		else if (controlDevice == OSInputGlobals::kControlType_Joystick) controlDevice = OSInputGlobals::kControlType_Keyboard;
		tile->SetFloat("_controlDevice", controlDevice);
		break;
	}
	case kModConfigurationMenu_Exit:
	{
		Close();
//		StartMenu::GetSingleton()->HandleClick(1, nullptr);
		break;
	}

	case kModConfigurationMenu_ModListItem:
	{

		auto mod = modsListBox.GetItemForTile(clickedTile);

		modsListBox.ForEach(SetTweakNotSelected);

		clickedTile->parent->SetFloat("_selected", 1);
		clickedTile->SetFloat("_selected", 1);

		SetActiveMod(mod);

		break;
	}

	case kModConfigurationMenu_CategoriesButton:
	{
		SetCategoriesListActive(!GetCategoriesListActive());
		break;
	}

	case kModConfigurationMenu_CategoryItem:
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

		categoriesListBox.SaveScrollPosition();
		this->SetCategoriesListActive(false);
		this->RefreshFilter();
		break;
	}
	case kModConfigurationMenu_SearchIcon:
	{
		this->SetInSearchMode(true);
		break;
	}
	case kModConfigurationMenu_CancelSearch:
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

	case kModConfigurationMenu_ToggleShowActive:
	{
		break;
	}

	case kModConfigurationMenu_SettingListItem:
	{
		// remove the caret from the original tile if one exists
		this->SetInSubsettingInputMode(false);

		auto setting = settingsListBox.GetItemForTile(clickedTile);

		if (setting->type == setting->kSettingType_Subsetting)
		{
			DisplaySettings(setting ? setting->id : "");
		}

			/*
		if (setting->IsInputField())
		{
			auto inputFieldTextTile = clickedTile->GetChildByID(kModConfigurationMenu_SubsettingInputFieldText);
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
				Log(true, Log::kConsole) << FormatString("ModConfigurationMenu: Failed to find input text tile (ID: %d)", kModConfigurationMenu_SubsettingInputFieldText);
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
			auto inputFieldTextTile = clickedTile->GetChildByID(kModConfigurationMenu_SubsettingInputFieldText);
			if (inputFieldTextTile)
			{
				hotkeyInput.tile = inputFieldTextTile;
				this->SetInHotkeyMode(true);
				this->TouchSubsetting(setting);
			}
		}*/
		break;
	}

	case kModConfigurationMenu_SliderLeftArrow:
	{
		const auto option = clickedTile->parent;
		if (!IsShiftHeld())
			settingsListBox.GetItemForTile(option)->Next(false)->Display(option);
		else
			settingsListBox.GetItemForTile(option)->Last(false)->Display(option);
		break;
	}
	case kModConfigurationMenu_ChoiceText:
	{
		const auto option = clickedTile->parent;
		settingsListBox.GetItemForTile(option)->Next(!IsShiftHeld())->Display(option);
		break;
	}
	case kModConfigurationMenu_SliderRightArrow:
	{
		const auto option = clickedTile->parent;
		if (!IsShiftHeld())
			settingsListBox.GetItemForTile(option)->Next()->Display(option);
		else
			settingsListBox.GetItemForTile(option)->Last()->Display(option);
		break;
	}
	case kModConfigurationMenu_SliderText:
	{
		// remove the caret from the original tile if one exists
		this->SetInSubsettingInputMode(false);

		if (auto tile = clickedTile->GetParentByID(kModConfigurationMenu_SettingListItem))
		{
			auto inputFieldTextTile = clickedTile->GetChildByID(kModConfigurationMenu_SubsettingInputFieldText);
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

void ModConfigurationMenu::SetInSubsettingInputMode(bool isActive)
{
	auto tile = subSettingInput.tile;
	if (tile)
	{
		if (tile->parent)
		{
			if (auto boxBackground = tile->parent->GetChildByID(kModConfigurationMenu_SubsettingInputFieldText_BoxBG))
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

				/*
				if (activeInputSubsetting->IsSlider())
				{
					if (tile->parent)
					{
						if (auto sliderTile = tile->parent->parent)
						{
							if (auto sliderRect = sliderTile->GetChildByID(kModConfigurationMenu_SliderDraggableRect))
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
					//activeInputSubsetting->Display(tile->parent);

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
/*	auto tile = hotkeyInput.tile;
	if (tile)
	{
		if (tile->parent)
		{
			if (auto boxBackground = tile->parent->GetChildByID(kModConfigurationMenu_SubsettingInputFieldText_BoxBG))
			{
				boxBackground->SetFloat("_BackgroundVisible", isActive);
			}
		}
	}

	if (activeHotkeySubsetting)
	{
		if (isActive)
		{
//			hotkeyInput.value = activeHotkeySubsetting->data.valueInt;
			if (tile)
			{
				tile->SetString(kTileValue_string, "...");
			}
		}
		else
		{
//			activeHotkeySubsetting->data.valueInt = hotkeyInput.value;
			if (tile->parent)
			{
				activeHotkeySubsetting->Display(tile->parent);
			}
			activeHotkeySubsetting = nullptr;
		}
	}

	hotkeyInput.SetActive(isActive);*/
}

bool ModConfigurationMenu::GetInHotkeyMode()
{
	return hotkeyInput.isActive;
}

bool __cdecl HideItemsNotMatchingFilterString(SM_Mod* item)
{
	const auto searchStr = ModConfigurationMenu::GetSingleton()->searchBar.GetText();
	if (item->name.find(searchStr) != std::string::npos) return false;
	if (item->settingName.find(searchStr) != std::string::npos) return false;
	if (item->description.find(searchStr) != std::string::npos) return false;
	return true;
}

void ModConfigurationMenu::RefreshFilter()
{
//	modsListBox.Filter(TweakFilter);

	auto textColor = modsListBox.GetNumVisibleItems() ? 1 : 2;
	searchBar.tile->SetFloat(kTileValue_systemcolor, textColor);

	if (auto selectedTile = modsListBox.GetTileFromItem(&activeTweak))
	{
		// if the selected tweak is filtered out
		if (selectedTile->GetFloat(kTileValue_listindex) < 0)
		{
			this->SetActiveMod(nullptr);
		}
	}
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
			this->HandleMouseover(kModConfigurationMenu_CategoriesBackground, nullptr);
			categoriesListBox.RestorePosition();
		}
		else if (settingsListBox.GetSelectedTile() && modsListBox.GetNumVisibleItems())
		{
			settingsListBox.SaveScrollPosition();
			this->HandleMouseover(kModConfigurationMenu_ModList, nullptr);
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
			this->HandleMouseover(kModConfigurationMenu_ModList, nullptr);
			modsListBox.RestorePosition();
		}
		else if (modsListBox.GetSelectedTile() && settingsListBox.GetNumVisibleItems())
		{
			modsListBox.SaveScrollPosition();
			this->HandleMouseover(kModConfigurationMenu_SettingList, nullptr);
			settingsListBox.RestorePosition();
		}
		return true;
	}
	case kMenu_LeftArrow:
	case kMenu_RightArrow:
	{
//		if (this->HandleActiveSliderArrows( code == kMenu_RightArrow))
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

//		return this->HandleActiveSliderArrows(isRightArrow, 0.2);
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
		this->HandleClick(kModConfigurationMenu_ToggleShowActive, nullptr);
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

void SM_Setting::Choice::Display(Tile* tile)
{
	const auto value = setting.ReadINI();
	const std::string valueString = choice.contains(value) ? choice[value].first : GetStringFromValue(value); // Display name or display value if name not found
	tile->SetString(kTileValue_user0, valueString.c_str());
}

void SM_Setting::Slider::Display(Tile* tile)
{
	const auto value = setting.ReadINI();
	tile->SetFloat(kTileValue_user0, 20 * GetFloatFromValue(value) / GetFloatFromValue(std::get<1>(slider)));
	tile->SetFloat(kTileValue_user3, 20);
}

void SM_Setting::Font::Display(Tile* tile)
{
	const auto value = font.ReadINI();
	SInt32 id = std::get<SInt32>(value);
	std::string valueString;
	if (!id)
	{
		valueString = "--"; // Display name or display value if name not found
		tile->GetChild("lb_toggle_value")->SetFloat(kTileValue_font, 7);
	}
	else if (fontMap.contains(value))
	{
		valueString = "Font " + GetStringFromValue(value);//fontMap[value];
		tile->GetChild("lb_toggle_value")->SetFloat(kTileValue_font, id);
	}
	else
	{
		valueString = "Font " + GetStringFromValue(value);
		tile->GetChild("lb_toggle_value")->SetFloat(kTileValue_font, id);
	}
	tile->SetString(kTileValue_user0, valueString.c_str());
}


void SM_Setting::Control::Display(Tile* tile)
{
	const auto value = keyboard.ReadINI();
	const auto key = GetStringForScancode(std::get<SInt32>(value), 1);
	tile->SetString("_Keyboard", key.c_str());

	const auto mouseValue = mouse.ReadINI();
	const auto mouse = GetStringForScancode(std::get<SInt32>(mouseValue), 2);
	tile->SetString("_Mouse", mouse.c_str());

	const auto controllerValue = controller.ReadINI();

	if (IsViableControllerString(std::get<SInt32>(controllerValue)))
	{
		const auto controller = GetControllerString(std::get<SInt32>(controllerValue));
		tile->SetString("_Controller", controller.c_str());
		tile->SetFloat("_ControllerImage", true);
	}
	else
	{
		const auto controller = GetControllerString(std::get<SInt32>(controllerValue));
		tile->SetString("_Controller", controller.c_str());
		tile->SetFloat("_ControllerImage", false);
	}
}

SM_Value SM_Setting::Choice::GetPrev(const SM_Value& value)
{
	if (choice.empty()) return value;
	if (auto iter = choice.find(value); iter == choice.end() || --iter == choice.end())
		return choice.rbegin()->first;
	else return iter->first;
}

SM_Value SM_Setting::Choice::GetNext(const SM_Value& value)
{
	if (choice.empty()) return value;
	if (auto iter = choice.find(value); iter == choice.end() || ++iter == choice.end())
		return choice.begin()->first;
	else return iter->first;
}

void SM_Setting::Choice::Next(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? GetNext(value) : GetPrev(value);
	Write(newValue);
}

void SM_Setting::Choice::Last(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? choice.rbegin()->first : choice.begin()->first;
	Write(newValue);
}

SM_Value SM_Setting::Slider::GetPrev(const SM_Value& value) const
{
	SM_Value newValue = value - std::get<2>(slider);
	return newValue < std::get<0>(slider) ? value : newValue;
}

SM_Value SM_Setting::Slider::GetNext(const SM_Value& value) const
{
	SM_Value newValue = value + std::get<2>(slider);
	return newValue > std::get<1>(slider) ? value : newValue;
}

void SM_Setting::Slider::Next(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? GetNext(value) : GetPrev(value);
	Write(newValue);
}

void SM_Setting::Slider::Last(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? std::get<1>(slider) : std::get<0>(slider);
	Write(newValue);
}

SM_Value SM_Setting::Font::GetPrev(const SM_Value& value)
{
	if (fontMap.empty()) return value;
	if (auto iter = fontMap.find(value); iter == fontMap.end() || --iter == fontMap.end())
		return fontMap.rbegin()->first;
	else return iter->first;
}

SM_Value SM_Setting::Font::GetNext(const SM_Value& value)
{
	if (fontMap.empty()) return value;
	if (auto iter = fontMap.find(value); iter == fontMap.end() || ++iter == fontMap.end())
		return fontMap.begin()->first;
	else return iter->first;
}

void SM_Setting::Font::Next(const bool forward)
{
	const auto value = font.ReadINI();
	const SM_Value newValue = forward ? GetNext(value) : GetPrev(value);
	Write(newValue);
}

void SM_Setting::Font::Last(const bool forward)
{
	const auto value = font.ReadINI();
	const auto newValue = forward ? 8 : 1;
	Write(newValue);
}

void ModConfigurationMenu::DisplaySettings(std::string tab)
{
	settingsListBox.FreeAllTiles();

	for (const auto& it : g_Settings)
	{
		if (!it->mods.contains(tab)) continue;

		const auto tile = settingsListBox.Insert(it.get());
		
		it->Display(tile);
	}
}

void ModConfigurationMenu::SetActiveMod(SM_Mod* mod)
{
	this->SetInSubsettingInputMode(false);
	this->SetInHotkeyMode(false);
	subSettingInput.tile = nullptr;
	hotkeyInput.tile = nullptr;

	activeTweak = mod;

	DisplaySettings(mod ? mod->id : "");
}

void ModConfigurationMenu::HandleMouseover(UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_ModListItem:
	{

		modsListBox.GetItemForTile(activeTile);

		auto mod = modsListBox.GetItemForTile(activeTile);

		selectionText->SetString(kTileValue_string, FormatString("%s\n- %s", mod->description.c_str(), mod->settingName.c_str()).c_str());

			//		this->SetTweakDescriptionTileString(item);
//		this->SetActiveMod(item);
//		PlayGameSound("UIMenuFocus");

		break;
	}

	case kModConfigurationMenu_SliderDraggableRect:
	case kModConfigurationMenu_SliderLeftArrow:
	case kModConfigurationMenu_SliderRightArrow:
	{
		auto item = settingsListBox.GetItemForTile(activeTile->parent);
		break;
	}

	case kModConfigurationMenu_SettingListItem:
	{
		auto item = settingsListBox.GetItemForTile(activeTile);

		const UInt32 listindex = activeTile->GetFloat(kTileValue_x);

		settingsListBox.parentTile->GetChild("lb_highlight_box")->SetFloat(kTileValue_x, activeTile->GetFloat(kTileValue_x), true);
		settingsListBox.parentTile->GetChild("lb_highlight_box")->SetFloat(kTileValue_width, activeTile->GetFloat(kTileValue_width), true);

		break;
	}

	case kModConfigurationMenu_CategoryItem:
	{
		break;
	}

	case kModConfigurationMenu_ModList:
	{
		break;
	}

	case kModConfigurationMenu_CategoriesBackground:
	{
		break;
	}

	case kModConfigurationMenu_SettingList:
	{
		break;
	}
	case kModConfigurationMenu_ChoiceText:
	{
		settingsListBox.SetSelected(activeTile->parent);
		break;
	}
	}
}

void ModConfigurationMenu::HandleUnmouseover(UInt32 tileID, Tile* tile)
{
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

	touchedSubsettings.RemoveAll();
	touchedTweaks.RemoveAll();

	Menu::Free();

	OSInputGlobals::GetSingleton()->SetDebounceMenuMode(false);
};

void ModConfigurationMenu::Destructor(bool doFree) 
{
	this->Free();
	if (doFree) GameHeapFree(this);
	g_ConfigurationMenu = nullptr;
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

void ModConfigurationMenu::Close()
{
	if (!g_bShownTweaksMenuReloadWarning)
	{
		if (!touchedTweaks.Empty() || !touchedSubsettings.Empty())
		{
			ShowMessageBox("You must restart the game for changes to be applied!", 0, 0, RestartGameWarningCallback, 0, 0x17, 0, 0, (char*)0x103934C, "Don't show this again", NULL); // OK
		}
	}

	Menu::Close();
}

void ModConfigurationMenu::HandleControllerConnectOrDisconnect(bool isControllerConnected)
{

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

		if (activeTileID != kModConfigurationMenu_SearchIcon && activeTileID != kModConfigurationMenu_CancelSearch)
		{
			this->SetInSearchMode(false);
		}

		if (activeTileID != kModConfigurationMenu_CategoriesButton && !isDraggingCategoriesSlider)
		{
			this->SetCategoriesListActive(false);
		}

		if (activeTileID != kModConfigurationMenu_SettingListItem && activeTileID != kModConfigurationMenu_SliderText)
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

//				activeHotkeySubsetting->data.valueInt = pressedKey;
//				ini.SetLongValue(activeHotkeySubsetting->settingCategory.c_str(), activeHotkeySubsetting->id.c_str(), pressedKey);
//				this->RefreshActiveTweakSelectionSquare();
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

//	SetCursorPosTraits();
}

bool ModConfigurationMenu::HasTiles()
{
	for (const auto tile : tiles) if (!tile) return false;
	if (!searchBar.tile) return false;
	return true;
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

	const auto newDepth = StdCall<Float32>(0xA1DFB0);

	if (!hasAddedNewTrait)
	{
		hasAddedNewTrait = true;
		CdeclCall(0x9FF8A0, "&ConfigurationMenu;", MENU_ID);
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

	g_ConfigurationMenu = menu;
	float stackingType = tile->GetFloat(kTileValue_stackingtype);
	if (stackingType == 6006.0 || stackingType == 102.0)
	{
		tile->SetFloat(kTileValue_depth, newDepth, 1);
	}

	Setting* sSettings = (Setting*)0x11D1FE0;
	menu->menuTitle->SetString(kTileValue_string, FormatString("Mod %s", sSettings->data.str).c_str());

	const auto modsListBox = &menu->modsListBox;
	modsListBox->parentTile = menu->tweaksListBackground;
	modsListBox->templateName = "ModTemplate";
	modsListBox->scrollBar = modsListBox->parentTile->GetChild("lb_scrollbar");

	const auto categoriesListBox = &menu->categoriesListBox;
	categoriesListBox->parentTile = menu->categoriesBackground;
	categoriesListBox->templateName = "CategoryTemplate";
	categoriesListBox->scrollBar = categoriesListBox->parentTile->GetChild("lb_scrollbar");

	const auto settingsListBox = &menu->settingsListBox;
	settingsListBox->parentTile = menu->subSettingsListBackground;
	settingsListBox->templateName = "SettingNoneTebmplate";
	settingsListBox->scrollBar = settingsListBox->parentTile->GetChild("lb_scrollbar");

	menu->ReadJSONForPath(GetCurPath() + R"(\Data\menus\)");

	for (const auto& mod : menu->g_Mods)
	{
		const auto tile = menu->modsListBox.Insert(mod.get(), mod->name.c_str());
		tile->SetFloat(kTileValue_id, kModConfigurationMenu_ModListItem);
	}

	modsListBox->Sort(CompareTweaksAlphabetically);

	Tile* listTile = categoriesListBox->Insert(nullptr, "All");
	if (listTile) listTile->SetFloat(kTileValue_id, kModConfigurationMenu_CategoryItem);

//	ModConfigurationMenu::GetSingleton()->SetActiveMod(modsListBox->list.first.data->object);

	menu->HideTitle(false);

	fontMap.emplace(0, "--");

	for (const auto iter : FontManager::GetSingleton()->fontInfos)
	{
		fontMap.emplace(static_cast<SInt32>(iter->id), std::string(iter->fontData->textures[0].fileName));
	};
	for (const auto iter : FontManager::GetSingleton()->extraFonts)
	{
		fontMap.emplace(static_cast<SInt32>(iter->id), std::string(iter->fontData->textures[0].fileName));
	};

}

void HotkeyField::Init()
{
	tile = nullptr;
	isActive = false;
	frameDebounce = true;
	value = 0;
}

int HotkeyField::GetPressedKey()
{
	if (frameDebounce) return 0;

	const auto input = OSInputGlobals::GetSingleton();
	for (int i = Scancodes::_Escape; i < 265; ++i)
	{
		if (input->GetKeyState(i, OSInputGlobals::isPressed))
		{
			return i;
		}
	}

	return 0;
}

int HotkeyField::Update()
{
	const auto key = GetPressedKey();
	if (key > 0) value = key == _Escape ? 0 : key;

	frameDebounce = false;
	return key;
}

void HotkeyField::SetActive(bool active)
{
	frameDebounce = active;
	isActive = active;
}

void HotkeyField::Free()
{

}