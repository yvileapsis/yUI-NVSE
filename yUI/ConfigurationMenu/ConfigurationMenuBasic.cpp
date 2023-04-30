#include "ConfigurationMenu.h"
#include "ConsoleManager.h"

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
	case kTileID_Back:				tileBackButton = activeTile; return;

	case kTileID_Title:				menuTitle = activeTile; return;
	case kTileID_SelectionText:		description.tile = activeTile; return;

	case kTileID_SettingList:		settingsMain.listBox.parentTile = activeTile; return;
	case kTileID_SettingText:		settingsMain.tagTile = (TileText*)activeTile; return;

	case kTileID_ExtraList:			settingsExtra.listBox.parentTile = activeTile; return;
	case kTileID_ExtraText:			settingsExtra.tagTile = (TileText*)activeTile; return;
	default: break;
	}
}

void ModConfigurationMenu::HandleMousewheel(UInt32 tileID, Tile* activeTile)
{

}

void ModConfigurationMenu::HandleActiveMenuClickHeld(UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kTileID_SettingValue: Drag(activeTile); break;
	case kTileID_SettingPrev:
	case kTileID_SettingNext:
	{
		if (InterfaceManager::GetSingleton()->timeLeftClickHeld > 0.5)
			ClickValue(activeTile, tileID != kTileID_SettingPrev ? CMSetting::kNext : CMSetting::kPrev);
		break;
	}
	default: break;
	}
}

void ModConfigurationMenu::HandleClick(UInt32 tileID, Tile* activeTile)
{
	if (controlHandler.HandleControl()) return;
}

void ModConfigurationMenu::HandleUnclick(UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kTileID_Back:				Back(); break;
	case kTileID_Default:			Default(); break;
	case kTileID_DeviceButton:		Device(); break;
	case kTileID_SaveToJSON:		SaveToJSON(); break;
	case kTileID_LoadFromJSON:		LoadFromJSON(); break;

	case kTileID_SettingRightArrow:
	case kTileID_SettingText:
	case kTileID_SettingLeftArrow:
	{
		tileID != kTileID_SettingLeftArrow ? ++settingsMain : --settingsMain;
		FilterSettings();
		HandleMouseover(kTileID_SettingText, nullptr);
		break;
	}

	case kTileID_ExtraRightArrow:
	case kTileID_ExtraText:
	case kTileID_ExtraLeftArrow:
	{
		tileID != kTileID_ExtraLeftArrow ? ++settingsExtra : --settingsExtra;
		FilterMods();
		HandleMouseover(kTileID_ExtraText, nullptr);
		break;
	}

	case kTileID_ExtraListItem:		ClickExtra(activeTile); break;
	case kTileID_SettingListItem:	ClickItem(activeTile); break;

	case kTileID_SettingValue:		ClickValue(activeTile); break;
	case kTileID_SettingPrev:		ClickValue(activeTile, CMSetting::kPrev); break;
	case kTileID_SettingNext:		ClickValue(activeTile, CMSetting::kNext); break;
	case kTileID_SettingUp:			ClickValue(activeTile, CMSetting::kUp); break;
	case kTileID_SettingDown:		ClickValue(activeTile, CMSetting::kDown); break;
	default: break;
	}
}


void ModConfigurationMenu::HandleRightClick(UInt32 tileID, Tile* activeTile)
{
	if (controlHandler.HandleControl()) return;
}

void ModConfigurationMenu::HandleRightUnclick(UInt32 tileID, Tile* activeTile)
{
	switch (tileID)
	{
	case kTileID_SettingValue:		ClickValue(activeTile, CMSetting::kValueAlt); break;
	case kTileID_SettingPrev:		ClickValue(activeTile, CMSetting::kPrev); break;
	case kTileID_SettingNext:		ClickValue(activeTile, CMSetting::kNext); break;
	case kTileID_SettingUp:			ClickValue(activeTile, CMSetting::kUp); break;
	case kTileID_SettingDown:		ClickValue(activeTile, CMSetting::kDown); break;
	default: break;
	}
}

bool ModConfigurationMenu::HandleKeyboardInput(UInt32 key)
{
	if (controlHandler.HandleControl()) return true;

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
			//			this->HandleMouseover(kTileID_CategoriesBackground, nullptr);
			//			categoriesListBox.RestorePosition();
		}
		else if (settingsMain.listBox.GetSelectedTile() && settingsExtra.listBox.GetNumVisibleItems())
		{
			settingsMain.listBox.SaveScrollPosition();
			this->HandleMouseover(kTileID_ExtraList, nullptr);
			settingsExtra.listBox.RestorePosition();
		}
		return true;
	}
	case kMenu_ShiftRight:
	{
		if (settingsExtra.listBox.GetNumVisibleItems())
		{
			//			categoriesListBox.SaveScrollPosition();
			this->HandleMouseover(kTileID_ExtraList, nullptr);
			settingsExtra.listBox.RestorePosition();
		}
		else if (settingsExtra.listBox.GetSelectedTile() && settingsMain.listBox.GetNumVisibleItems())
		{
			settingsExtra.listBox.SaveScrollPosition();
			this->HandleMouseover(kTileID_SettingList, nullptr);
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

	case kTileID_ExtraLeftArrow:
	case kTileID_ExtraText:
	case kTileID_ExtraRightArrow:
	{
//		description <<= settingsExtra;
		break;
	}

	case kTileID_SettingLeftArrow:
	case kTileID_SettingText:
	case kTileID_SettingRightArrow:
	{
//		description <<= settingsMain;
		break;
	}
	case kTileID_ExtraListItem:
	{
//		description <<= settingsExtra.listBox.GetItemForTile(activeTile);
		break;
	}
	case kTileID_SettingListItem:
	{
		settingsMain.listBox.parentTile->GetChild("lb_highlight_box")->Set(kTileValue_x, (Float32) activeTile->Get(kTileValue_x), true);
		settingsMain.listBox.parentTile->GetChild("lb_highlight_box")->Set(kTileValue_width, (Float32)activeTile->Get(kTileValue_width), true);

//		description <<= settingsMain.listBox.GetItemForTile(activeTile);

		break;
	}
	case kTileID_SettingValue:
	case kTileID_SettingPrev:
	case kTileID_SettingNext:
	case kTileID_SettingUp:
	case kTileID_SettingDown:
	{
		settingsMain.listBox.SetSelectedTile(activeTile->parent);
		break;
	}
	default:
	{
		break;
	}
	}
}

void ModConfigurationMenu::HandleUnmouseover(UInt32 tileID, Tile* activeTile)
{
	description <<= "";
}


bool ModConfigurationMenu::HandleControllerInput(int code, Tile* activeTile)
{
	if (controlHandler.HandleControl()) return true;
	//	this->SetInSearchMode(false);
	//	if (this->GetInSubsettingInputMode() && code != OSInputGlobals::kXboxCtrl_BUTTON_A)
	{
		//		this->SetInSubsettingInputMode(false);
	}

	if (code == OSInputGlobals::kXboxCtrl_L3 || code == OSInputGlobals::kXboxCtrl_R3)
	{
		//		this->HandleUnclick(kTileID_ToggleShowActive, nullptr);
		return true;
	}

	return false;
}

void ModConfigurationMenu::HandleControllerConnectOrDisconnect(bool isControllerConnected)
{
}

void ModConfigurationMenu::UpdateRightClick()
{
	const auto manager = InterfaceManager::GetSingleton();
	auto isActivateDepressed = 0;
	if (const auto console = ConsoleManager::GetSingleton(); !console || console->isConsoleOpen <= 0)
		isActivateDepressed = OSInputGlobals::GetSingleton()->GetControlState(OSInputGlobals::Aim, OSInputGlobals::isDepressed);
	//GetMouseState(OSInputGlobals::kRightClick, OSInputGlobals::isDepressed);
	//GetControlState(OSInputGlobals::Aim, OSInputGlobals::isDepressed);

	if (isActivateDepressed)
	{
		const auto activeTile = manager->activeTile;
		if (manager->activeMenu == this && visibilityState == 1 && activeTile && activeTile->GetValue(kTileValue_id))
		{
			activeTile->PlayTileSound(kTileValue_clicksound);
			activeTile->Set(kTileValue_clicked, 1);
			activeTile->Set(kTileValue_clicked, 0);
			HandleRightUnclick(activeTile->Get(kTileValue_id), activeTile);
		}
	}
}

void ModConfigurationMenu::Update()
{
	UpdateRightClick();

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

		//		if (activeTileID != kTileID_SearchIcon && activeTileID != kTileID_CancelSearch)
		{
			//			this->SetInSearchMode(false);
		}

		//		if (activeTileID != kTileID_CategoriesButton && !isDraggingCategoriesSlider)
		{
			//			this->SetCategoriesListActive(false);
		}

//		if (activeTileID != kTileID_SettingListItem && activeTileID != kTileID_SliderText)
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
