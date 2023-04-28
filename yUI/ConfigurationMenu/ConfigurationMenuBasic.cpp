#include "ConfigurationMenu.h"

#include "functions.h"
#include "InterfaceManager.h"

bool IsControllerConnected() { return *(UInt8*)0x11D8C50 || *(UInt8*)0x11D8A84 == 0; }

// called by TileMenu::InitMenu
UInt32 ModConfigurationMenu::GetID() { return MENU_ID; }

// not a true unique_ptr since scope is partly managed by the game, 
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

//	settingsExtra.listBox.flags &= ~ListBox<CMSetting>::kFlag_RecalculateHeightsOnInsert;
//	settingsMain.flags &= ~ModsListBox::kFlag_RecalculateHeightsOnInsert;

	searchBar.Init();
	subSettingInput.Init();

	lastXMLWriteTime.dwLowDateTime = 0;
	lastXMLWriteTime.dwHighDateTime = 0;

	// prevent Escape closing the whole start menu if Configuration Menu is open
	*(UInt8*)0x119F348 = 0;

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

	delete tile;

	OSInputGlobals::GetSingleton()->SetDebounceMenuMode(false);

}

void ModConfigurationMenu::SetTile(const UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_Title:				menuTitle = activeTile; return;
	case kModConfigurationMenu_ModList:				modsListBackground = activeTile; return;
	case kModConfigurationMenu_SelectionText:		description.tile = activeTile; return;
	case kModConfigurationMenu_Exit:				doneTile = activeTile; return;
	case kModConfigurationMenu_SearchBar:			searchBar.tile = activeTile; return;
	case kModConfigurationMenu_SettingCategoryText:	settingsMain.tagTile = (TileText*)activeTile; return;
	case kModConfigurationMenu_SettingList:			settingsListBackground = activeTile; return;
	case kModConfigurationMenu_CategoryText:		settingsExtra.tagTile = (TileText*)activeTile; return;
	}
}

void ModConfigurationMenu::HandleMousewheel(UInt32 tileID, Tile* activeTile)
{

}

void ModConfigurationMenu::HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile)
{
	if (tileID == kModConfigurationMenu_SliderDraggableRect)
	{
		const auto option = activeTile->parent;
		settingsMain.listBox.GetItemForTile(option)->Drag(activeTile->Get(kTileValue_user1))->Display(option);
	}

	return;
	if (tileID == kModConfigurationMenu_SliderLeftArrow || tileID == kModConfigurationMenu_SliderRightArrow)
	{
		if (InterfaceManager::GetSingleton()->timeLeftClickHeld > 0.3)
		{
			const auto option = activeTile->parent;
			if (tileID != kModConfigurationMenu_SliderLeftArrow)
			{
				settingsMain.listBox.GetItemForTile(option)->ClickValue(option)->Display(option);
			}
			else
			{
				settingsMain.listBox.GetItemForTile(option)->ClickValueAlt(option)->Display(option);
			}
		}
	}
}

void ModConfigurationMenu::HandleClick(UInt32 tileID, Tile* clickedTile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_ChoiceText:
	case kModConfigurationMenu_SliderLeftArrow:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->ClickValueAlt(option)->Display(option);
		break;
	}
	case 98:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->ClickValue(option);

	}
	case kModConfigurationMenu_SliderRightArrow:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->ClickValue(option)->Display(option);
		break;
	}
	}
}


void ModConfigurationMenu::HandleUnclick(UInt32 tileID, Tile* clickedTile)
{
	switch (tileID)
	{
	case kModConfigurationMenu_CategoryRightArrow:
	case kModConfigurationMenu_CategoryText:
	case kModConfigurationMenu_CategoryLeftArrow:
	{
		tileID != kModConfigurationMenu_CategoryLeftArrow ? ++settingsExtra : --settingsExtra;
		FilterMods();
		HandleMouseover(kModConfigurationMenu_CategoryText, nullptr);
		break;
	}

	case kModConfigurationMenu_SettingCategoryRightArrow:
	case kModConfigurationMenu_SettingCategoryText:
	case kModConfigurationMenu_SettingCategoryLeftArrow:
	{
		tileID != kModConfigurationMenu_SettingCategoryLeftArrow ? ++settingsMain : --settingsMain;
		FilterSettings();
		HandleMouseover(kModConfigurationMenu_SettingCategoryText, nullptr);
		break;
	}
	case kModConfigurationMenu_SaveToJSON:		SaveToJSON(); return;
	case kModConfigurationMenu_LoadFromJSON:	LoadFromJSON(); return;
	case kModConfigurationMenu_Default:			Default(); return;
	case kModConfigurationMenu_DeviceButton:	Device(); return;
	case kModConfigurationMenu_Exit:			Back(); return;
	case kModConfigurationMenu_ModListItem:		ClickExtra(clickedTile); return;
	case kModConfigurationMenu_SettingListItem:	ClickMain(clickedTile); return;
	case kModConfigurationMenu_SearchIcon:
	{
		//		this->SetInSearchMode(true);
		break;
	}
	case kModConfigurationMenu_SliderLeftArrow:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->ClickValueAlt(option)->Display(option);
		break;
	}
	case 98:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->ClickValue(option);

	}
	case kModConfigurationMenu_ChoiceText:
	case kModConfigurationMenu_SliderRightArrow:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->ClickValue(option)->Display(option);
		break;
	}
	case kModConfigurationMenu_SubsettingInputFieldText_BoxBG:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->Up(false)->Display(option);
		break;
	}
	case kModConfigurationMenu_SliderText:
	{
		const auto option = clickedTile->parent;
		settingsMain.listBox.GetItemForTile(option)->Up()->Display(option);
		break;
	}
	}
}

bool ModConfigurationMenu::HandleKeyboardInput(UInt32 key)
{
	if (key == OSInputGlobals::Escape_)
	{
		Back();
		return true;
	}

	if (controlHandler.IsActive())
	{
		controlHandler.SetKeyboard(key);
		return true;
	}

	return false;
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
				return true;
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
	case kMenu_Tab: Back(); return true;
	case kMenu_ShiftLeft:
	{
		if (settingsExtra.listBox.GetSelectedTile())
		{
			settingsExtra.listBox.SaveScrollPosition();
			//			this->HandleMouseover(kModConfigurationMenu_CategoriesBackground, nullptr);
			//			categoriesListBox.RestorePosition();
		}
		else if (settingsMain.listBox.GetSelectedTile() && settingsExtra.listBox.GetNumVisibleItems())
		{
			settingsMain.listBox.SaveScrollPosition();
			this->HandleMouseover(kModConfigurationMenu_ModList, nullptr);
			settingsExtra.listBox.RestorePosition();
		}
		return true;
	}
	case kMenu_ShiftRight:
	{
		if (settingsExtra.listBox.GetNumVisibleItems())
		{
			//			categoriesListBox.SaveScrollPosition();
			this->HandleMouseover(kModConfigurationMenu_ModList, nullptr);
			settingsExtra.listBox.RestorePosition();
		}
		else if (settingsExtra.listBox.GetSelectedTile() && settingsMain.listBox.GetNumVisibleItems())
		{
			settingsExtra.listBox.SaveScrollPosition();
			this->HandleMouseover(kModConfigurationMenu_SettingList, nullptr);
			settingsMain.listBox.RestorePosition();
		}
		return true;
	}
	case kMenu_LeftArrow:
	case kMenu_RightArrow: return true;
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
		description <<= settingsExtra;
		break;
	}

	case kModConfigurationMenu_SettingCategoryLeftArrow:
	case kModConfigurationMenu_SettingCategoryText:
	case kModConfigurationMenu_SettingCategoryRightArrow:
	{
		description <<= settingsMain;
		break;
	}
	case kModConfigurationMenu_ModListItem:
	{
		description <<= settingsExtra.listBox.GetItemForTile(activeTile);
		break;
	}
	case kModConfigurationMenu_SliderDraggableRect:
	case kModConfigurationMenu_SliderLeftArrow:
	case kModConfigurationMenu_SliderRightArrow:
	{
		auto item = settingsMain.listBox.GetItemForTile(activeTile->parent);
		break;
	}
	case kModConfigurationMenu_SettingListItem:
	{
		settingsMain.listBox.parentTile->GetChild("lb_highlight_box")->Set(kTileValue_x, (Float32) activeTile->Get(kTileValue_x), true);
		settingsMain.listBox.parentTile->GetChild("lb_highlight_box")->Set(kTileValue_width, (Float32)activeTile->Get(kTileValue_width), true);

		description <<= settingsMain.listBox.GetItemForTile(activeTile);

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
		//		this->HandleUnclick(kModConfigurationMenu_ToggleShowActive, nullptr);
		return true;
	}

	return false;
}

void ModConfigurationMenu::HandleControllerConnectOrDisconnect(bool isControllerConnected)
{

}


void ModConfigurationMenu::Update()
{
	settingsMain.Update();
	settingsExtra.Update();

	searchBar.Update();
	subSettingInput.Update();

	UpdateEscape();

	return;
	
	// close the search and categoriesMods list when clicking elsewhere
	//if (input->GetMouseState(OSInputGlobals::kLeftClick, OSInputGlobals::isDepressed))
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
	return;

	/*
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
	*/
	if (IsControllerConnected())
	{
		auto rightStickX = XINPUT_GAMEPAD_EX::GetCurrent()->sThumbRX;
		if (rightStickX)
		{
			int deadzoneRS = *(UInt32*)0x9455F5;
			if (abs(rightStickX) > deadzoneRS)
			{
				if (auto activeSubsetting = settingsMain.listBox.GetSelected())
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
