#include "ConfigurationMenu.h"

#include "functions.h"
#include "InterfaceManager.h"

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

// called by TileMenu::InitMenu
UInt32 ModConfigurationMenu::GetID() { return MENU_ID; }

// not a true unique_ptr since scope is partly managed by the game
std::unique_ptr<ModConfigurationMenu> g_ConfigurationMenu;

ModConfigurationMenu* ModConfigurationMenu::GetSingleton() { return g_ConfigurationMenu.get(); }

ModConfigurationMenu* ModConfigurationMenu::Create()
{
	g_ConfigurationMenu = std::make_unique<ModConfigurationMenu>();
	return g_ConfigurationMenu.get();
}


static bool hasAddedNewTrait = false;

bool ModConfigurationMenu::HasTiles()
{
	//	for (const auto tile : tiles) if (!tile) return false;
	if (!searchBar.tile) return false;
	return true;
}

const char* MenuPath = R"(Data\Menus\ConfigurationMenu.xml)";

ModConfigurationMenu* ModConfigurationMenu::ReloadMenu()
{
	//	if (g_ConfigurationMenu) g_ConfigurationMenu->tile->Destroy(true);
	//	g_ConfigurationMenu = nullptr;

	const auto newDepth = StdCall<Float32>(0xA1DFB0);

	if (!hasAddedNewTrait)
	{
		hasAddedNewTrait = true;
		CdeclCall(0x9FF8A0, "&ConfigurationMenu;", MENU_ID);
	}


	Tile* tile = InterfaceManager::GetSingleton()->menuRoot->InjectUIXML(MenuPath, true); // creates menu by itself

	const auto menu = GetSingleton();

	if (!menu || menu != tile->GetParentMenu())
	{
		Log(g_LogLevel >= Log::kError, Log::kBoth) << "Configuration Menu: Failed Init!";
		return nullptr;
	}

	if (menu->GetID() != MENU_ID)
	{
		Log(g_LogLevel >= Log::kError, Log::kBoth) << FormatString(
			"Configuration Menu: Expected <class> %d </class>, found class '%d!'", MENU_ID, menu->GetID());
		delete menu;
		return nullptr;
	}

	menu->RegisterTile(tile, false);

	if (!menu->HasTiles())
	{
		menu->Close();
		Log(g_LogLevel >= Log::kError, Log::kBoth) << "Configuration Menu: Expected tiles missing!";
		return nullptr;
	}

	const float stackingType = tile->Get(kTileValue_stackingtype);
	if (stackingType == 6006.0 || stackingType == 102.0)
	{
		tile->Set(kTileValue_depth, newDepth, 1);
	}

	return menu;
};


ModConfigurationMenu::ModConfigurationMenu() : Menu()
{
	memset(tiles, 0, sizeof tiles);

	id = MENU_ID;
	modsListBox.Init();
	modsListBox.flags &= ~ListBox<CMMod>::kFlag_RecalculateHeightsOnInsert;

	searchBar.Init();
	subSettingInput.Init();
	hotkeyInput.Init();
	settingsListBox.Init();

//	description.Set("");
//	settingsListBox.flags &= ~ModsListBox::kFlag_RecalculateHeightsOnInsert;

	lastXMLWriteTime.dwLowDateTime = 0;
	lastXMLWriteTime.dwHighDateTime = 0;
}


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

// called by the game
ModConfigurationMenu::~ModConfigurationMenu()
{
	// if we're in the destructor unique_ptr should be invalid
	g_ConfigurationMenu.release();

	modsListBox.Destroy();
	settingsListBox.Destroy();
	searchBar.Free();
	subSettingInput.Free();
	hotkeyInput.Free();

	delete tile;

	//	Menu::Free();

	OSInputGlobals::GetSingleton()->SetDebounceMenuMode(false);

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
	case kModConfigurationMenu_DeviceButton: { Device(); break; }
	case kModConfigurationMenu_Exit: { Back(); break; }
	case kModConfigurationMenu_ModListItem: { SelectMod(clickedTile); break; }
	case kModConfigurationMenu_SettingListItem: { SelectSetting(clickedTile); break; }
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
		settingsListBox.parentTile->GetChild("lb_highlight_box")->Set(kTileValue_x, (Float32) activeTile->Get(kTileValue_x), true);
		settingsListBox.parentTile->GetChild("lb_highlight_box")->Set(kTileValue_width, (Float32)activeTile->Get(kTileValue_width), true);

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
			activeTileID = tile->Get(kTileValue_id);
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
