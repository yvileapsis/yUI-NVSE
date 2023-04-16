#include "ConfigurationMenu.h"

#include "dinput8.h"
#include "functions.h"
#include "Setting.h"
#include "InterfaceManager.h"
#include "SafeWrite.h"


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

void CMSetting::Choice::Display(Tile* tile)
{
	const auto value = setting.Read();
	const std::string valueString = choice.contains(value) ? choice[value] : static_cast<std::string>(value); // Display name or display value if name not found
	tile->Set(kTileValue_user0, valueString);
}

void CMSetting::Slider::Display(Tile* tile)
{
	const auto value = setting.Read();
	tile->Set(kTileValue_user0, 20 * static_cast<Float64>(value) / static_cast<Float64>(max));
	tile->Set(kTileValue_user3, 20);
}

void CMSetting::Font::Display(Tile* tile)
{
	const auto fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	const auto value = font.Read();
	const SInt32 id = value;
	std::string valueString;
	if (!id)
	{
		valueString = "--"; // Display name or display value if name not found
		tile->GetChild("lb_toggle_value")->Set(kTileValue_font, 7);
	}
	else if (fontMap.contains(value))
	{
		valueString = "Font " + value;//fontMap[value];
		tile->GetChild("lb_toggle_value")->Set(kTileValue_font, id);
	}
	else
	{
		valueString = "Font " + value;
		tile->GetChild("lb_toggle_value")->Set(kTileValue_font, id);
	}
	tile->Set(kTileValue_user0, valueString);
}


void CMSetting::Control::Display(Tile* tile)
{
	const auto value = keyboard.Read();
	const auto key = GetStringForScancode(value, 1);
	tile->Set("_Keyboard", key);

	const auto mouseValue = mouse.Read();
	const auto mouse = GetStringForScancode((mouseValue), 2);
	tile->Set("_Mouse", mouse);

	const auto controllerValue = controller.Read();

	if (IsViableControllerString((controllerValue)))
	{
		const auto controller = GetControllerString((controllerValue));
		tile->Set("_Controller", controller);
		tile->Set("_ControllerImage", true);
	}
	else
	{
		const auto controller = GetControllerString((controllerValue));
		tile->Set("_Controller", controller);
		tile->Set("_ControllerImage", false);
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
	const auto value = setting.Read();
	const auto newValue = forward ? GetNext(value) : GetPrev(value);
	setting.Write(newValue);
}

void CMSetting::Choice::Last(const bool forward)
{
	const auto value = setting.Read();
	const auto newValue = forward ? choice.rbegin()->first : choice.begin()->first;
	setting.Write(newValue);
}

SM_Value CMSetting::Slider::GetPrev(const SM_Value& value) const
{
	SM_Value newValue = value - delta;
	return newValue < max ? value : newValue;
}

SM_Value CMSetting::Slider::GetNext(const SM_Value& value) const
{
	SM_Value newValue = value + delta;
	return newValue > min ? value : newValue;
}

void CMSetting::Slider::Next(const bool forward)
{
	const auto value = setting.Read();
	const auto newValue = forward ? GetNext(value) : GetPrev(value);
	setting.Write(newValue);
}

void CMSetting::Slider::Last(const bool forward)
{
	const auto value = setting.Read();
	const auto newValue = forward ? max : min;
	setting.Write(newValue);
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
	const auto value = font.Read();
	const SM_Value newValue = forward ? GetNext(value) : GetPrev(value);
	font.Write(newValue);
}

void CMSetting::Font::Last(const bool forward)
{
	const auto value = font.Read();
	const auto newValue = SM_Value(static_cast<SInt32>(forward ? 8 : 1));
	font.Write(newValue);
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

		modsListBox <<= mod.get();

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

	for (const auto& setting : GetSettingsForString(tab))
	{
		settingsListBox <<= setting;
		for (const auto& tag : setting->tags) settingsListBox <<= tag;
	}

	++settingsListBox;
}

void ModConfigurationMenu::SelectMod(CMMod* mod)
{
	activeMod = mod;

	modsListBox.ForEach([](Tile* tile, CMMod* mod)
	{
		const auto menu = GetSingleton();
		tile->Set("_selected", menu->activeMod == mod ? 1 : 0);
	});
	modsListBox.parentTile->Set("_selected", 1);

	DisplaySettings(activeMod ? activeMod->GetID() : "");
	FilterSettings();
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

void ModConfigurationMenu::ClickMod(Tile* clickedTile) { SelectMod(modsListBox.GetItemForTile(clickedTile)); }

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


void ModConfigurationMenu::SelectSetting(CMSetting* setting)
{
	activeSetting = setting;
	// TODO: subsetting virtual function
	if (setting->type == setting->kSettingType_Subsetting)
	{
		DisplaySettings(setting ? setting->GetID() : "");
	}
}

void ModConfigurationMenu::ClickSetting(Tile* clickedTile) { SelectSetting(settingsListBox.GetItemForTile(clickedTile)); }

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


void ModConfigurationMenu::ShowTweaksMenu()
{
	// prevent Escape closing the whole start menu if StewMenu is open
	*(UInt8*)0x119F348 = 0;

	const auto sSettings = reinterpret_cast<Setting*>(0x11D1FE0);
	menuTitle->Set(kTileValue_string, FormatString("Mod %s", sSettings->data.str));

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

	ReadJSONForPath(GetCurPath() + R"(\Data\menus\ConfigurationMenu\)");

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
	*(UInt8*)0x119F348 = 1;
}

void ModConfigurationMenu::Device()
{
	if (controlDevice == OSInputGlobals::kControlType_Keyboard) controlDevice = OSInputGlobals::kControlType_Mouse;
	else if (controlDevice == OSInputGlobals::kControlType_Mouse) controlDevice = OSInputGlobals::kControlType_Joystick;
	else if (controlDevice == OSInputGlobals::kControlType_Joystick) controlDevice = OSInputGlobals::kControlType_Keyboard;
	tile->Set("_controlDevice", controlDevice);
}

void ModConfigurationMenu::Default()
{
//	SaveModToJSON(activeMod);
}

std::vector<CMSetting*> ModConfigurationMenu::GetSettingsForString(std::string str)
{
	std::vector<CMSetting*> ret;
	for (const auto& setting : g_Settings)
	{
		if (!setting->mods.contains(str)) continue;
		ret.push_back(setting.get());
	}
	return ret;
}

template <typename Item> void ModConfigurationMenu::ListBoxWithFilter<Item>::UpdateTagString()
{
	const auto menu = GetSingleton();
	const auto tag = menu->g_Tags[tagActive].get();
	tagTile->Set(kTileValue_string, tag ? tag->GetName() : tagActive);
}

template <typename Item>
void ModConfigurationMenu::ListBoxWithFilter<Item>::ClearTags()
{
	tags.clear();
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
	this->SortAlt(item);
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
	searchBar.tile->Set(kTileValue_systemcolor, textColor);

//	if (auto selectedTile = modsListBox.GetTileFromItem(&activeMod))
	{
		// if the selected tweak is filtered out
//		if (selectedTile->GetFloat(kTileValue_listindex) < 0)
		{
//			this->ClickMod(nullptr);
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
		tile->Set(kTileValue_string, "");
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
		tile->Set(_CaretIndex, caretIndex);
	}
	tile->Set(kTileValue_string, inputString);
}

void InputField::SetActive(bool active)
{
	isActive = active;
	lastCaretUpdateTime = 0;
	isCaretShown = false;

	if (tile)
	{
		static auto textAlphaTrait = Tile::TraitNameToID("_IsActive");
		tile->Set(textAlphaTrait, active);
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
			tile->Set(kTileValue_string, GetText());
		}
		else
		{
			tile->Set(kTileValue_string, "");
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

		tile->Set(kTileValue_string, GetText());
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