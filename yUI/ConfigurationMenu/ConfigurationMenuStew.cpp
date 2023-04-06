#include "ConfigurationMenu.h"

#include "dinput8.h"
#include "functions.h"
#include "GameSettings.h"
#include "InterfaceManager.h"
#include "SafeWrite.h"

const char* MenuPath = R"(Data\Menus\ConfigurationMenu.xml)";

ModConfigurationMenu* g_ConfigurationMenu;

ModConfigurationMenu* ModConfigurationMenu::GetSingleton() { return g_ConfigurationMenu; }

UInt32 ModConfigurationMenu::GetID() { return id; }

void ModConfigurationMenu::Destructor(bool doFree)
{
	this->~ModConfigurationMenu();

	if (doFree) this->operator delete(this);

	g_ConfigurationMenu = nullptr;
}

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

void ModConfigurationMenu::SetTile(const UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_Title:				menuTitle = activeTile; break;
	case kModConfigurationMenu_ModList:				modsListBackground = activeTile; break;
	case kModConfigurationMenu_SelectionText:		description.tile = activeTile; break;
	case kModConfigurationMenu_Exit:				doneTile = activeTile; break;
	case kModConfigurationMenu_SearchBar:			searchBar.tile = activeTile; break;
	case kModConfigurationMenu_SettingCategoryText:	settingsListBox.tagTile = (TileText*)activeTile; break;
	case kModConfigurationMenu_SettingList:			settingsListBackground = activeTile; break;
	case kModConfigurationMenu_CategoryText:		modsListBox.tagTile = (TileText*)activeTile; break;
	}
}

void ModConfigurationMenu::HandleMousewheel(UInt32 tileID, Tile* activeTile)
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

void ModConfigurationMenu::HandleLeftClick(UInt32 tileID, Tile* clickedTile)
{
//	if (clickedTile == categoriesListBox.scrollBar || clickedTile->parent == categoriesListBox.scrollBar)
	{
//		this->isDraggingCategoriesSlider = true;
	}
}


void ModConfigurationMenu::HandleClick(UInt32 tileID, Tile* clickedTile) 
{
	switch (tileID)
	{
	case kModConfigurationMenu_CategoryRightArrow:
	case kModConfigurationMenu_CategoryText:
	case kModConfigurationMenu_CategoryLeftArrow:
	{
		tileID != kModConfigurationMenu_CategoryLeftArrow ? ++modsListBox : --modsListBox;
		FilterMods();
		HandleMouseover(kModConfigurationMenu_CategoryText, nullptr);
		break;
	}

	case kModConfigurationMenu_SettingCategoryRightArrow:
	case kModConfigurationMenu_SettingCategoryText:
	case kModConfigurationMenu_SettingCategoryLeftArrow:
	{
		tileID != kModConfigurationMenu_SettingCategoryLeftArrow ? ++settingsListBox : --settingsListBox;
		FilterSettings();
		HandleMouseover(kModConfigurationMenu_SettingCategoryText, nullptr);
		break;
	}
	case kModConfigurationMenu_DeviceButton:	{ Device(); break; }
	case kModConfigurationMenu_Exit:			{ Back(); break; }
	case kModConfigurationMenu_ModListItem:		{ SelectMod(clickedTile); break; }
	case kModConfigurationMenu_SettingListItem:	{ SelectSetting(clickedTile); break; }
	case kModConfigurationMenu_SearchIcon:
	{
//		this->SetInSearchMode(true);
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
//		this->SetInSubsettingInputMode(false);

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

bool ModConfigurationMenu::HandleKeyboardInput(UInt32 key) 
{
	if (IsControlHeld())
	{
		if ((key | 0x20) == 'f')
		{
//			this->SetInSearchMode(!this->GetInSearchMode());
			return true;
		}
		else if ((key | 0x20) == 'r')
		{
//			if (this->GetInSubsettingInputMode())
			{
				subSettingInput.Set("");
			}
//			else if (this->GetInSearchMode() || this->IsSearchSuspended())
			{
//				this->ClearAndCloseSearch();
			}
			return true;
		}
	}

//	if (this->GetInSearchMode())
	{
		if (searchBar.HandleKey(key))
		{
			this->RefreshFilter();
			return true;
		}
	}
//	else// if (this->GetInSubsettingInputMode())
	{
		if (key == kInputCode_Enter)
		{
			// handle it here instead of in the InputField since we want to update the setting string
//			this->SetInSubsettingInputMode(false);
			return true;
		}
		else if (key != '`' && key != '\t')
		{
			if (subSettingInput.HandleKey(key))
			{
//				subSettingInput.tile->SetFloat(kTileValue_systemcolor, this->IsSubsettingInputValid() ? 1 : 2);
			}
			else
			{
				PlayGameSound("UIMenuCancel");
			}
			return true;
		}
	}
//	else if (this->GetInHotkeyMode())
	{
		// ignore all keyboard input while in hotkey mode, hotkeys are checked in ModConfigurationMenu::UpdateTagString to allow checking function keys etc.
//		return true;
	}
//	else
	if ((key | 0x20) == 'r')
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
		Back();
		return true;
	}
	case kMenu_ShiftLeft:
	{
		if (modsListBox.GetSelectedTile())
		{
			modsListBox.SaveScrollPosition();
//			this->HandleMouseover(kModConfigurationMenu_CategoriesBackground, nullptr);
//			categoriesListBox.RestorePosition();
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
		if (modsListBox.GetNumVisibleItems())
		{
//			categoriesListBox.SaveScrollPosition();
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

void ModConfigurationMenu::HandleMouseover(UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{

	case kModConfigurationMenu_CategoryLeftArrow:
	case kModConfigurationMenu_CategoryText:
	case kModConfigurationMenu_CategoryRightArrow:
	{
		description <<= modsListBox;
		break;
	}

	case kModConfigurationMenu_SettingCategoryLeftArrow:
	case kModConfigurationMenu_SettingCategoryText:
	case kModConfigurationMenu_SettingCategoryRightArrow:
	{
		description <<= settingsListBox;
		break;
	}
	case kModConfigurationMenu_ModListItem:
	{
		description <<= modsListBox.GetItemForTile(activeTile);
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
		settingsListBox.parentTile->GetChild("lb_highlight_box")->SetFloat(kTileValue_x, activeTile->GetFloat(kTileValue_x), true);
		settingsListBox.parentTile->GetChild("lb_highlight_box")->SetFloat(kTileValue_width, activeTile->GetFloat(kTileValue_width), true);
	
		description <<= settingsListBox.GetItemForTile(activeTile);

		break;
	}
	default:
	{
		break;
	}
	}
}

void ModConfigurationMenu::HandleUnmouseover(UInt32 tileID, Tile* tile)
{
	description <<= "";
}


/*
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
*/

void ModConfigurationMenu::Close()
{
//	if (!g_bShownTweaksMenuReloadWarning)
	{
//		if (!touchedTweaks.Empty() || !touchedSubsettings.Empty())
		{
//			ShowMessageBox("You must restart the game for changes to be applied!", 0, 0, RestartGameWarningCallback, 0, 0x17, 0, 0, (char*)0x103934C, "Don't show this again", NULL); // OK
		}
	}

	Menu::Close();
}

bool ModConfigurationMenu::HandleControllerInput(int code, Tile* tile)
{
//	this->SetInSearchMode(false);
//	if (this->GetInSubsettingInputMode() && code != OSInputGlobals::kXboxCtrl_BUTTON_A)
	{
//		this->SetInSubsettingInputMode(false);
	}

	if (code == OSInputGlobals::kXboxCtrl_L3 || code == OSInputGlobals::kXboxCtrl_R3)
	{
//		this->HandleClick(kModConfigurationMenu_ToggleShowActive, nullptr);
		return true;
	}

	return false;
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
		Back();
		return;
	}

	// close the search and categoriesMods list when clicking elsewhere
	if (input->GetMouseState(OSInputGlobals::kLeftClick, OSInputGlobals::isDepressed))
	{
		auto activeTileID = -1;
		if (auto tile = InterfaceManager::GetSingleton()->GetActiveTile())
		{
			activeTileID = tile->GetFloat(kTileValue_id);
		}

//		if (activeTileID != kModConfigurationMenu_SearchIcon && activeTileID != kModConfigurationMenu_CancelSearch)
		{
//			this->SetInSearchMode(false);
		}

//		if (activeTileID != kModConfigurationMenu_CategoriesButton && !isDraggingCategoriesSlider)
		{
//			this->SetCategoriesListActive(false);
		}

		if (activeTileID != kModConfigurationMenu_SettingListItem && activeTileID != kModConfigurationMenu_SliderText)
		{
//			if (this->GetInSubsettingInputMode())
			{
//				this->SetInSubsettingInputMode(false);
			}
		}

	}

	if (this->GetInHotkeyMode())
	{
		auto pressedKey = hotkeyInput.Update();
		if (pressedKey)
		{
			if (pressedKey != Tilde)
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
//			this->SetInHotkeyMode(false);
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


void CMSetting::Choice::Display(Tile* tile)
{
	const auto value = setting.ReadINI();
	const std::string valueString = choice.contains(value) ? choice[value].first : static_cast<std::string>(value); // Display name or display value if name not found
	tile->SetString(kTileValue_user0, valueString.c_str());
}

void CMSetting::Slider::Display(Tile* tile)
{
	const auto value = setting.ReadINI();
	tile->SetFloat(kTileValue_user0, 20 * static_cast<Float64>(value) / static_cast<Float64>(std::get<1>(slider)));
	tile->SetFloat(kTileValue_user3, 20);
}

void CMSetting::Font::Display(Tile* tile)
{
	const auto fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	const auto value = font.ReadINI();
	const SInt32 id = value;
	std::string valueString;
	if (!id)
	{
		valueString = "--"; // Display name or display value if name not found
		tile->GetChild("lb_toggle_value")->SetFloat(kTileValue_font, 7);
	}
	else if (fontMap.contains(value))
	{
		valueString = "Font " + value;//fontMap[value];
		tile->GetChild("lb_toggle_value")->SetFloat(kTileValue_font, id);
	}
	else
	{
		valueString = "Font " + value;
		tile->GetChild("lb_toggle_value")->SetFloat(kTileValue_font, id);
	}
	tile->SetString(kTileValue_user0, valueString.c_str());
}


void CMSetting::Control::Display(Tile* tile)
{
	const auto value = keyboard.ReadINI();
	const auto key = GetStringForScancode(value, 1);
	tile->SetString("_Keyboard", key.c_str());

	const auto mouseValue = mouse.ReadINI();
	const auto mouse = GetStringForScancode((mouseValue), 2);
	tile->SetString("_Mouse", mouse.c_str());

	const auto controllerValue = controller.ReadINI();

	if (IsViableControllerString((controllerValue)))
	{
		const auto controller = GetControllerString((controllerValue));
		tile->SetString("_Controller", controller.c_str());
		tile->SetFloat("_ControllerImage", true);
	}
	else
	{
		const auto controller = GetControllerString((controllerValue));
		tile->SetString("_Controller", controller.c_str());
		tile->SetFloat("_ControllerImage", false);
	}
}

SM_Value CMSetting::Choice::GetPrev(const SM_Value& value)
{
	if (choice.empty()) return value;
	if (auto iter = choice.find(value); iter == choice.end() || --iter == choice.end())
		return choice.rbegin()->first;
	else return iter->first;
}

SM_Value CMSetting::Choice::GetNext(const SM_Value& value)
{
	if (choice.empty()) return value;
	if (auto iter = choice.find(value); iter == choice.end() || ++iter == choice.end())
		return choice.begin()->first;
	else return iter->first;
}

void CMSetting::Choice::Next(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? GetNext(value) : GetPrev(value);
	Write(newValue);
}

void CMSetting::Choice::Last(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? choice.rbegin()->first : choice.begin()->first;
	Write(newValue);
}

SM_Value CMSetting::Slider::GetPrev(const SM_Value& value) const
{
	SM_Value newValue = value - std::get<2>(slider);
	return newValue < std::get<0>(slider) ? value : newValue;
}

SM_Value CMSetting::Slider::GetNext(const SM_Value& value) const
{
	SM_Value newValue = value + std::get<2>(slider);
	return newValue > std::get<1>(slider) ? value : newValue;
}

void CMSetting::Slider::Next(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? GetNext(value) : GetPrev(value);
	Write(newValue);
}

void CMSetting::Slider::Last(const bool forward)
{
	const auto value = setting.ReadINI();
	const auto newValue = forward ? std::get<1>(slider) : std::get<0>(slider);
	Write(newValue);
}

SM_Value CMSetting::Font::GetPrev(const SM_Value& value)
{
	const auto fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	if (fontMap.empty()) return value;
	if (auto iter = fontMap.find(value); iter == fontMap.end() || --iter == fontMap.end())
		return fontMap.rbegin()->first;
	else return iter->first;
}

SM_Value CMSetting::Font::GetNext(const SM_Value& value)
{
	const auto fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	if (fontMap.empty()) return value;
	if (auto iter = fontMap.find(value); iter == fontMap.end() || ++iter == fontMap.end())
		return fontMap.begin()->first;
	else return iter->first;
}

void CMSetting::Font::Next(const bool forward)
{
	const auto value = font.ReadINI();
	const SM_Value newValue = forward ? GetNext(value) : GetPrev(value);
	Write(newValue);
}

void CMSetting::Font::Last(const bool forward)
{
	const auto value = font.ReadINI();
	const auto newValue = SM_Value(static_cast<SInt32>(forward ? 8 : 1));
	Write(newValue);
}

void ModConfigurationMenu::DisplayMods()
{
	const auto compareAlphabetically = [](ListBoxItem<CMMod>* a, ListBoxItem<CMMod>* b)
	{
		auto itemA = a->object;
		auto itemB = b->object;

		if (itemA->priority == itemB->priority)
		{
			return static_cast<SInt32>((itemA->GetName() <=> itemB->GetName())._Value);
		}

		return static_cast<SInt32>( itemA->priority > itemB->priority ? -1 : 1);
	};

	modsListBox.FreeAllTiles();
	modsListBox.ClearTags();
	modsListBox << "";

	modsListBox <<= "!Mods";
	for (const auto& mod : g_Mods) {

		modsListBox.SortAlt(modsListBox <<= mod.get());

		for (const auto& tag : mod->tags) modsListBox <<= tag;
	}

	// TODO:: Figure out why it needs Sort, probably something to do with recalculate on insert flag
	modsListBox.Sort(compareAlphabetically);

	++modsListBox;
}

void ModConfigurationMenu::DisplaySettings(std::string tab)
{
	settingsListBox.FreeAllTiles();
	settingsListBox.ClearTags();

	// TODO: add order of displayed tags;
	settingsListBox << "";

	for (const auto& setting : g_Settings)
	{
		if (!setting->mods.contains(tab)) continue;

		const auto item = settingsListBox <<= setting.get();
		settingsListBox.SortAlt(item);

		for (const auto& tag : setting->tags) settingsListBox <<= tag;
	}

	++settingsListBox;
}

void ModConfigurationMenu::SelectMod(Tile* clickedTile)
{
	const auto mod = modsListBox.GetItemForTile(clickedTile);

	modsListBox.ForEach([](Tile* tile, CMMod* tweak) { tile->SetFloat("_selected", 0); });
	clickedTile->parent->SetFloat("_selected", 1);
	clickedTile->SetFloat("_selected", 1);

	DisplaySettings(mod ? mod->GetID() : "");
	FilterSettings();
}

void ModConfigurationMenu::FilterMods() 
{
	const auto filter = [](CMMod* mod)
	{
		const auto menu = GetSingleton();
		if (menu->modsListBox.tagActive == menu->tagDefault->id) return false;
		if (mod->tags.contains(menu->modsListBox.tagActive)) return false;
		return true;
	};
	modsListBox.Filter(filter);
}

void ModConfigurationMenu::SelectSetting(Tile* clickedTile)
{
	const auto setting = settingsListBox.GetItemForTile(clickedTile);

	// TODO: subsetting virtual function
	if (setting->type == setting->kSettingType_Subsetting)
	{
		DisplaySettings(setting ? setting->GetID() : "");
	}
}

void ModConfigurationMenu::FilterSettings()
{
	const auto filter = [](CMSetting* setting)
	{
		const auto menu = GetSingleton();
		if (menu->settingsListBox.tagActive == menu->tagDefault->GetID()) return false;
		if (setting->tags.contains(menu->settingsListBox.tagActive)) return false;
		return true;
	};
	settingsListBox.Filter(filter);
}


bool ModConfigurationMenu::HasTiles()
{
//	for (const auto tile : tiles) if (!tile) return false;
	if (!searchBar.tile) return false;
	return true;
}

int reloadTweaksMenuFrameDelay;
void __fastcall ReloadTweaksMenuInOneFrameHook(void* startMenu)
{
	ThisCall(0x7D5830, startMenu);
	if (!--reloadTweaksMenuFrameDelay)
	{
		WriteRelCall(0x7CFA8C, 0x7D5830);
		if (const auto menu = ModConfigurationMenu::ReloadMenu()) menu->ShowTweaksMenu();
	}
}

// reloads the entire menu by closing it, and writing a hook in StartMenu::UpdateTagString which waits a couple frames and then reopens the menu
void ModConfigurationMenu::ReloadMenuXML()
{
	this->Close();
	WriteRelCall(0x7CFA8C, ReloadTweaksMenuInOneFrameHook);
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


static bool hasAddedNewTrait = false;

ModConfigurationMenu* ModConfigurationMenu::ReloadMenu()
{
	if (g_ConfigurationMenu) g_ConfigurationMenu->tile->Destroy(true);
	if (g_ConfigurationMenu) g_ConfigurationMenu->Destructor(true);

	const auto newDepth = StdCall<Float32>(0xA1DFB0);

	if (!hasAddedNewTrait)
	{
		hasAddedNewTrait = true;
		CdeclCall(0x9FF8A0, "&ConfigurationMenu;", MENU_ID);
	}

	// allow hot-reloading the menu even if UIO is installed
	UInt32 previousResolveXMLFile = DetourRelCall(0xA01B87, 0xA01E20);

	Create();
	Tile* tile = InterfaceManager::GetSingleton()->menuRoot->ReadXML(MenuPath);
	WriteRelCall(0xA01B87, previousResolveXMLFile);

	const auto menu = static_cast<ModConfigurationMenu*>(tile->GetParentMenu());

	if (!menu)
	{
		Log(true, Log::kConsole) << FormatString("MENUS: Stew Menu Creation Failed...");
		return nullptr;
	}

	if (menu->GetID() != MENU_ID)
	{
		Log(true, Log::kConsole) << FormatString(
			"MENUS: Expected Stew Menu to have <class> %d </class> but it had class %d...", MENU_ID, menu->GetID());
		menu->Destructor(true);
		return nullptr;
	}

	menu->RegisterTile(tile, false);

	if (!menu->HasTiles())
	{
		menu->Close();
		Log(true, Log::kConsole) << FormatString("MENUS: Stew Menu is missing some expected tiles...");
		return nullptr;
	}

	g_ConfigurationMenu = menu;

	const float stackingType = tile->GetFloat(kTileValue_stackingtype);
	if (stackingType == 6006.0 || stackingType == 102.0)
	{
		tile->SetFloat(kTileValue_depth, newDepth, 1);
	}

	return g_ConfigurationMenu;
};

void ModConfigurationMenu::ShowTweaksMenu()
{
	const auto sSettings = reinterpret_cast<Setting*>(0x11D1FE0);
	menuTitle->SetString(kTileValue_string, FormatString("Mod %s", sSettings->data.str).c_str());

	modsListBox.parentTile = modsListBackground;
	modsListBox.templateName = CMMod::GetTemplate();
	modsListBox.scrollBar = modsListBox.parentTile->GetChild("lb_scrollbar");

	settingsListBox.parentTile = settingsListBackground;
	settingsListBox.templateName = CMSetting::None::GetTemplateAlt();
	settingsListBox.scrollBar = settingsListBox.parentTile->GetChild("lb_scrollbar");

	if (!tagDefault)
	{
		g_Tags.emplace("!Mods", std::make_unique<CMTag>("!Mods", "Mods", "Mods can be filtered by modTags."));
		tagDefault = g_Tags.begin()->second.get();
	}

	ReadJSONForPath(GetCurPath() + R"(\Data\menus\)");

	DisplayMods();

	HideTitle(false);

	fontMap.emplace(SM_Value(static_cast<SInt32>(0)), "--");

	for (const auto font : FontManager::GetSingleton()->fontInfos)
		fontMap.emplace(static_cast<SInt32>(font->id), std::string(font->fontData->textures[0].fileName));
	for (const auto font : FontManager::GetSingleton()->extraFonts)
		fontMap.emplace(static_cast<SInt32>(font->id), std::string(font->fontData->textures[0].fileName));;

}

void ModConfigurationMenu::Back()
{
	Close();
	StartMenu::GetSingleton()->HandleClick(0, nullptr);
}

void ModConfigurationMenu::Device()
{
	if (controlDevice == OSInputGlobals::kControlType_Keyboard) controlDevice = OSInputGlobals::kControlType_Mouse;
	else if (controlDevice == OSInputGlobals::kControlType_Mouse) controlDevice = OSInputGlobals::kControlType_Joystick;
	else if (controlDevice == OSInputGlobals::kControlType_Joystick) controlDevice = OSInputGlobals::kControlType_Keyboard;
	tile->SetFloat("_controlDevice", controlDevice);
}

template <typename Item> void ModConfigurationMenu::ListBoxWithFilter<Item>::UpdateTagString()
{
	const auto menu = GetSingleton();
	const auto tag = menu->g_Tags[tagActive].get();
	tagTile->SetString(kTileValue_string, tag ? tag->GetName().c_str() : tagActive.c_str());
}

template <typename Item>
void ModConfigurationMenu::ListBoxWithFilter<Item>::ClearTags()
{
	tags.clear();
}

template <typename Item> void ModConfigurationMenu::ListBoxWithFilter<Item>::operator++()
{
	if (tags.empty()) return;
	auto iter = tags.find(tagActive);
	tagActive = iter == tags.end() || ++iter == tags.end() ? *tags.begin() : *iter;
	UpdateTagString();
}

template <typename Item> void ModConfigurationMenu::ListBoxWithFilter<Item>::operator--()
{
	if (tags.empty()) return;
	auto iter = tags.find(tagActive);
	tagActive = iter == tags.end() || iter-- == tags.begin() ? *tags.rbegin() : *iter;
	UpdateTagString();
}

template <typename Item> void ModConfigurationMenu::ListBoxWithFilter<Item>::operator<<(const std::string& tag)
{
	tagActive = tag;
	UpdateTagString();
}

template <typename Item>
void ModConfigurationMenu::ListBoxWithFilter<Item>::operator<<=(const std::string& tag)
{
	tags.emplace(tag);
}

template <typename Item>
ListBoxItem<Item>* ModConfigurationMenu::ListBoxWithFilter<Item>::operator<<=(Item* tag)
{
	const auto item = this->InsertAlt(tag, tag->GetName().c_str(), tag->GetTemplate());
	tag->Display(item->tile);
	return item;
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
/*
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
	}
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
		//		subSettingInput.inputType = activeInputSubsetting->type;
	}
	else
	{
		//		if (activeInputSubsetting)
		{
			if (this->IsSubsettingInputValid())
			{
				this->SetActiveSubsettingValueFromInput();

				
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
				}
			}
			//			else
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

		//	activeInputSubsetting = nullptr;
	}

	subSettingInput.SetActive(isActive);
}

bool ModConfigurationMenu::GetInSubsettingInputMode()
{
	return subSettingInput.isActive;
}
*//*
void ModConfigurationMenu::SetInHotkeyMode(bool isActive)
{
		auto tile = hotkeyInput.tile;
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

		hotkeyInput.SetActive(isActive);
}
*/
/* CLICK SETTING 
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

bool ModConfigurationMenu::GetInHotkeyMode()
{
	return hotkeyInput.isActive;
}

bool __cdecl HideItemsNotMatchingFilterString(CMMod* item)
{
	const auto searchStr = ModConfigurationMenu::GetSingleton()->searchBar.GetText();
	if (item->name.find(searchStr) != std::string::npos) return false;
	//	if (item->settingName.find(searchStr) != std::string::npos) return false;
	if (item->description.find(searchStr) != std::string::npos) return false;
	return true;
}

void ModConfigurationMenu::RefreshFilter()
{
	//	modsListBox.Filter(TweakFilter);

	auto textColor = modsListBox.GetNumVisibleItems() ? 1 : 2;
	searchBar.tile->SetFloat(kTileValue_systemcolor, textColor);

//	if (auto selectedTile = modsListBox.GetTileFromItem(&activeMod))
	{
		// if the selected tweak is filtered out
//		if (selectedTile->GetFloat(kTileValue_listindex) < 0)
		{
//			this->SelectMod(nullptr);
		}
	}
}

/*
void ModConfigurationMenu::ClearAndCloseSearch()
{
	searchBar.Set("");
	this->SetInSearchMode(false);
	this->RefreshFilter();
}

*/

void InputField::Init()
{
	this->tile = nullptr;
	input.clear();
	isActive = false;
	isCaretShown = false;
	caretIndex = 0;
	lastCaretUpdateTime = 0;
	inputType = kInputType_String;
}

void InputField::Set(std::string str)
{
	input = str;
	if (str.empty())
	{
		tile->SetString(kTileValue_string, "");
		caretIndex = 0;
	}
	else
	{
		caretIndex = str.length();
	}
}

std::string InputField::GetText()
{
	return input;
}

UInt16 InputField::GetLen()
{
	return input.length();
}

void InputField::UpdateCaretDisplay()
{
	static const UInt32 _CaretIndex = Tile::TraitNameToID("_CaretIndex");

	std::string inputString;
	inputString = GetText();
	if (isActive)
	{
		inputString.insert(caretIndex, isCaretShown ? "|" : " ");
		tile->SetFloat(_CaretIndex, caretIndex);
	}
	tile->SetString(kTileValue_string, inputString.c_str());
}

void InputField::SetActive(bool active)
{
	isActive = active;
	lastCaretUpdateTime = 0;
	isCaretShown = false;

	if (tile)
	{
		static auto textAlphaTrait = Tile::TraitNameToID("_IsActive");
		tile->SetFloat(textAlphaTrait, active);
		if (!active)
		{
			UpdateCaretDisplay();
		}
	}
}

bool InputField::IsKeyValid(UInt32 key)
{
	if (key >= kInputCode_Backspace) return false;

	if (inputType != kInputType_String)
	{
		if (key == '-')
		{
			return caretIndex == 0 && GetText()[0] != '-';
		}

		if (inputType == kInputType_Float)
		{
			// only allow one . character
			if (key == '.') return strchr(GetText().c_str(), '.') == nullptr;
		}
		return key >= '0' && key <= '9';
	}

	return key != '\t' && key != '`' && key != '\r' && key != '\n';
}



bool InputField::HandleSpecialInputKey(UInt32 key)
{
	switch (key)
	{
	case kInputCode_Backspace:
	case kInputCode_Delete:
	{
		if (key == kInputCode_Backspace)
		{
			if (IsControlHeld())
			{
				//				caretIndex = (input.c_str()).EraseWord(caretIndex);
			}
			else
			{
				input.erase(--caretIndex);
				caretIndex = max(0, caretIndex);
			}
		}
		else
		{
			if (IsControlHeld())
			{
				//				input.EraseNextWord(caretIndex);
			}
			else
			{
				input.erase(caretIndex);
				caretIndex = min(input.length(), caretIndex);
			}
		}

		if (input.length())
		{
			tile->SetString(kTileValue_string, this->GetText().c_str());
		}
		else
		{
			tile->SetString(kTileValue_string, "");
		}

		return true;
	}
	case kInputCode_ArrowLeft:
	{
		if (IsControlHeld())
		{
			caretIndex -= GetCharsSinceSpace(input.c_str(), caretIndex);
		}
		else
		{
			--caretIndex;
		}
		caretIndex = max(0, caretIndex);
		return true;
	}
	case kInputCode_ArrowRight:
	{
		if (IsControlHeld())
		{
			caretIndex += GetCharsTillSpace(input.c_str(), caretIndex) + 1;
		}
		else
		{
			++caretIndex;
		}
		caretIndex = min(input.length(), caretIndex);
		return true;
	}
	case kInputCode_Enter:
	{
		SetActive(false);
		return true;
	}
	case kInputCode_Home:
	{
		caretIndex = 0;
		return true;
	}
	case kInputCode_End:
	{
		caretIndex = input.length();
		return true;
	}
	}

	return false;
}

void InputField::HandleKeyboardShortcuts(UInt32 key)
{
	if ((key | 0x20) == 'v')
	{
		// handle pasting from clipboard
		auto clipboardText = GetClipboardText();
		auto len = min(200, clipboardText.length()); // cap paste to 200 characters
		for (int i = 0; i < len; ++i)
		{
			if (char c = clipboardText[i]; IsKeyValid(c))
			{
				input.insert(caretIndex++, 1, c);
			}
		}
	}
}

bool InputField::HandleKey(UInt32 key)
{
	lastCaretUpdateTime = 0;
	isCaretShown = false;

	if (IsKeyValid(key))
	{
		if (IsControlHeld())
		{
			HandleKeyboardShortcuts(key);
		}
		else
		{
			input.insert(caretIndex++, 1, key);
		}

		tile->SetString(kTileValue_string, this->GetText().c_str());
		return true;
	}

	return HandleSpecialInputKey(key);
}

void InputField::Update()
{
	if (isActive)
	{
		auto currentTime = StdCall<UInt32>(0x457FE0);
		if (currentTime - lastCaretUpdateTime > 530)
		{
			isCaretShown = !isCaretShown;
			lastCaretUpdateTime = currentTime;
			UpdateCaretDisplay();
		}
	}
}

void InputField::Free()
{
}