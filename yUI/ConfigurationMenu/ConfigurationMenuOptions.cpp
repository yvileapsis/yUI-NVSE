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

CMSettingCategory* CMSettingCategory::Default()
{
	for (const auto iter : ModConfigurationMenu::GetSingleton()->GetSettingsForString(categoryID))
		iter->Default();
	return this;
}

CMSettingCategory* CMSettingCategory::Click(Tile* tile)
{
	ModConfigurationMenu::GetSingleton()->DisplaySettings(categoryID);
	return this;
}

CMSettingChoice* CMSettingChoice::Display(Tile* tile)
{
	const auto value = setting.Read();

	std::string valueString = choice.contains(value) ? choice[value] : "";

	if (valueString.empty()) valueString = static_cast<std::string>(value);

	tile->Set(kTileValue_user0, valueString);

	return this;
}

CMSettingSlider* CMSettingSlider::Display(Tile* tile)
{
	const auto value = setting.Read();
	const Float64 percent = (Float64) (value - min) / (Float64) (max - min);
	tile->Set(kTileValue_user0, 20 * percent);
	tile->Set(kTileValue_user3, 20);

	return this;
}

CMSettingFont* CMSettingFont::Display(Tile* tile)
{
	const auto& fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	const auto value = font.Read();
	const auto valueY = fontY.Read();
	const SInt32 id = value;
	std::string valueString;
	if (!id)
	{
		valueString = "--"; // DisplaySettings name or displayAfterTimer value if name not found
		tile->GetChild("lb_toggle_value")->Set(kTileValue_font, 7);
	}
	else if (fontMap.contains(value))
	{
		valueString = "CMSettingFont " + value.GetAsString();//fontMap[value];
		tile->GetChild("lb_toggle_value")->Set(kTileValue_font, id);
	}
	else
	{
		valueString = "CMSettingFont " + value.GetAsString();
		tile->GetChild("lb_toggle_value")->Set(kTileValue_font, id);
	}
	tile->Set(kTileValue_user0, valueString);
	tile->Set(kTileValue_user1, valueY.GetAsFloat());

	return this;
}

CMSettingControl* CMSettingControl::Display(Tile* tile)
{
	const auto value = keyboard.Read();
	const auto key = GetStringForScancode(value, 1);
	tile->Set("_Keyboard", key);

	const auto mouseValue = mouse.Read();
	const auto mouse = GetStringForScancode((mouseValue), 2);
	tile->Set("_Mouse", mouse);

	const auto controllerValue = controller.Read();

	if (IsViableControllerString(controllerValue))
	{
		const auto controller = GetControllerString(controllerValue);
		tile->Set("_Controller", controller);
		tile->Set("_ControllerImage", true);
	}
	else
	{
		const auto controller = GetControllerString(controllerValue);
		tile->Set("_Controller", controller);
		tile->Set("_ControllerImage", false);
	}
	return this;
}

CMSettingControl* CMSettingControl::ClickValue(Tile* tile, UInt32 option)
{
	const auto menu = ModConfigurationMenu::GetSingleton();
	menu->controlHandler.setting = this;

	return this;
}

CMValue CMSettingChoice::GetPrev(const CMValue& value)
{
	if (choice.empty()) return value;
	if (auto iter = choice.find(value); iter == choice.end() || --iter == choice.end())
		return choice.rbegin()->first;
	else return iter->first;
}

CMValue CMSettingChoice::GetNext(const CMValue& value)
{
	if (choice.empty()) return value;
	if (auto iter = choice.find(value); iter == choice.end() || ++iter == choice.end())
		return choice.begin()->first;
	else return iter->first;
}

CMSettingChoice* CMSettingChoice::ClickValue(Tile* tile, UInt32 option)
{
	switch (option) {
	case kValue:
	case kNext:
	{
		const auto value = setting.Read();

		const auto newValue = !IsShiftHeld() ? GetNext(value) : choice.rbegin()->first;

		setting.Write(newValue);

		break;
	}
	case kValueAlt:
	case kPrev:
	{
		const auto value = setting.Read();

		const auto newValue = !IsShiftHeld() ? GetPrev(value) : choice.begin()->first;

		setting.Write(newValue);

		break;
	}
	default: break;
	}
	return this;
}

CMValue CMSettingSlider::GetPrev(const CMValue& value) const
{
	CMValue newValue = value - delta;
	return newValue < min ? value : newValue;
}

CMValue CMSettingSlider::GetNext(const CMValue& value) const
{
	CMValue newValue = value + delta;
	return newValue > max ? value : newValue;
}

CMSettingSlider* CMSettingSlider::ClickValue(Tile* tile, UInt32 option)
{
	switch (option)
	{
	case kNext: {
		const auto value = setting.Read();

		const auto newValue = !IsShiftHeld() ? GetNext(value) : max;

		setting.Write(newValue);

		break;
	}
	case kPrev:
	{
		const auto value = setting.Read();

		const auto newValue = !IsShiftHeld() ? GetPrev(value) : min;

		setting.Write(newValue);

		break;
	}
	}
	return this;
}

CMSettingSlider* CMSettingSlider::Drag(Float32 value)
{
	const SInt64 forceTruncating = std::round(value * (max.GetAsFloat() - min.GetAsFloat()) / delta.GetAsFloat());

	CMValue newValue;

	if (delta.IsFloat()) 
		newValue.Set((Float64) min + forceTruncating * delta.GetAsFloat());
	else 
		newValue.Set((SInt64) min + forceTruncating * delta.GetAsInteger());

	setting.Write(newValue);

	return this;
}

CMValue CMSettingFont::GetPrev(const CMValue& value)
{
	const auto& fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	if (fontMap.empty()) return value;
	if (auto iter = fontMap.find(value); iter == fontMap.end() || --iter == fontMap.end())
		return fontMap.rbegin()->first;
	else return iter->first;
}

CMValue CMSettingFont::GetNext(const CMValue& value)
{
	const auto& fontMap = ModConfigurationMenu::GetSingleton()->fontMap;
	if (fontMap.empty()) return value;
	if (auto iter = fontMap.find(value); iter == fontMap.end() || ++iter == fontMap.end())
		return fontMap.begin()->first;
	else return iter->first;
}

CMSettingFont* CMSettingFont::ClickValue(Tile* tile, UInt32 option)
{
	switch (option)
	{
	case kValue:
	case kNext:
	{
		const auto value = font.Read();
		const auto newValue = !IsShiftHeld() ? GetNext(value) : CMValue(static_cast<SInt32>(8));
		font.Write(newValue);

		break;
	}
	case kValueAlt:
	case kPrev:
	{
		const auto value = font.Read();
		const auto newValue = !IsShiftHeld() ? GetPrev(value) : CMValue(static_cast<SInt32>(1));
		font.Write(newValue);

		break;
	}
	case kUp:
	{
		const auto value = fontY.Read();
		const auto newValue = value.GetAsFloat() + 0.5;
		fontY.Write(newValue);

		break;
	}
	case kDown:
	{
		const auto value = fontY.Read();
		const auto newValue = value.GetAsFloat() - 0.5;
		fontY.Write(newValue);

		break;
	}
	}
	return this;
}

void ModConfigurationMenu::SettingList::UpdateSettingsList()
{
	const auto menu = GetSingleton();

	listBox.parentTile->Set("_doublestacked", doublestacked);

	listBox.FreeAllTiles();

	*this << "";

	tags.clear();

	// TODO: add order of displayed tags;

	if (allTag) *this <<= "!All";

	for (const auto& setting : menu->GetSettingsForString(categoryActive))
	{
		if (!main && !setting->IsCategory()) continue;

		const auto item = listBox.InsertAlt(setting, main && !doublestacked ? setting->GetName().c_str() : setting->GetShortName().c_str(), main ? setting->GetTemplate() : nullptr);
		listBox.SortAlt(item);
		for (const auto& tag : setting->tags) *this <<= tag;
	}

	++* this;

	auto compareAlphabetically = [](ListBoxItem<CMSetting>* a, ListBoxItem<CMSetting>* b)
	{
		auto itemA = a->object;
		auto itemB = b->object;

		if (itemA->priority == itemB->priority)
		{
			return static_cast<SInt32>((itemA->GetName() <=> itemB->GetName())._Value);
		}

		return static_cast<SInt32>(itemA->priority > itemB->priority ? -1 : 1);
	};

	// TODO:: Figure out why it needs Sort, probably something to do with recalculate on insert flag
	listBox.Sort(compareAlphabetically);

}

void ModConfigurationMenu::SettingList::DisplaySettings()
{
	if (main) for (const auto& setting : listBox.list)
		setting->object->Display(setting->tile);
}

void ModConfigurationMenu::SettingList::Update()
{
	if (!startTime) return;
	auto percentageCompleted = static_cast<Float32>(GetTickCount() - startTime) / duration;

	if (percentageCompleted > 1.0) percentageCompleted = 1;

	listBox.parentTile->Set("_alpha", (alphaTarget - alphaStart) * percentageCompleted + alphaStart, true);
	tagTile->Set("_alpha", (alphaTarget - alphaStart) * percentageCompleted + alphaStart, true);

	if (percentageCompleted < 1.0) return;

	if (!displayAfterTimer)
		startTime = 0;
	else
	{
		alphaStart = 0.0;
		alphaTarget = 255.0;
		startTime = GetTickCount();
		duration = 0.25 * 1000;

		UpdateSettingsList();
		DisplaySettings();

		displayAfterTimer = false;
	}
}

void ModConfigurationMenu::SettingList::Display(const std::string& newcategory, bool newmain, bool newallTag,
                                                       bool mewdoublestacked)
{
	if (categoryActive == newcategory)
	{
		DisplaySettings();
		return;
	}
	categoryActive = newcategory;

	main = newmain;
	allTag = newallTag;
	doublestacked = mewdoublestacked;

	startTime = GetTickCount();
	duration = 0.25 * 1000;

	if (listBox.parentTile->Get("_alpha") > 0)
	{
		displayAfterTimer = true;

		alphaStart = 255.0;
		alphaTarget = 0.0;
	}
	else
	{
		UpdateSettingsList();
		DisplaySettings();

		displayAfterTimer = false;

		alphaStart = 0.0;
		alphaTarget = 255.0;
	}

}

void ModConfigurationMenu::SettingList::Hide()
{
	if (listBox.parentTile->Get("_alpha") > 0)
	{
		displayAfterTimer = false;
		categoryActive = "!Empty";

		startTime = GetTickCount();
		duration = 0.25 * 1000;

		alphaStart = 255.0;
		alphaTarget = 0.0;
	}
}

void ModConfigurationMenu::FilterSettings()
{
	const auto filter = [](CMSetting* setting)
	{
		const auto menu = GetSingleton();
		if (menu->settingsMain.tagActive == menu->tagDefault->GetID()) return false;
		if (setting->tags.contains(menu->settingsMain.tagActive)) return false;
		return true;
	};
	settingsMain.listBox.Filter(filter);
}

void ModConfigurationMenu::ClickItem(Tile* activeTile)
{
	settingsMain.listBox.GetItemForTile(activeTile)->Click(activeTile);
}

void ModConfigurationMenu::ClickExtra(Tile* activeTile)
{
	categoryHistory.pop_back();
	settingsExtra.listBox.GetItemForTile(activeTile)->Click(activeTile);
}

void ModConfigurationMenu::ClickValue(Tile* activeTile, CMSetting::ClickType type)
{
	const auto option = activeTile->parent;
	settingsMain.listBox.GetItemForTile(option)->ClickValue(option, type)->Display(option);
}

void ModConfigurationMenu::Drag(Tile* activeTile)
{
	const auto option = activeTile->parent;
	settingsMain.listBox.GetItemForTile(option)->Drag(activeTile->Get(kTileValue_user1))->Display(option);
}

void ModConfigurationMenu::FilterMods() 
{
	const auto filter = [](CMSetting* mod)
	{
		const auto menu = GetSingleton();
		if (menu->settingsExtra.tagActive == menu->tagDefault->id) return false;
		if (mod->tags.contains(menu->settingsExtra.tagActive)) return false;
		return true;
	};
	settingsExtra.listBox.Filter(filter);
}

int reloadTweaksMenuFrameDelay;
void __fastcall ReloadTweaksMenuInOneFrameHook(void* startMenu)
{
	ThisCall(0x7D5830, startMenu);
	if (!--reloadTweaksMenuFrameDelay)
	{
		WriteRelCall(0x7CFA8C, 0x7D5830);
		if (const auto menu = ModConfigurationMenu::ReloadMenu()) menu->ShowMenuFirstTime();
	}
}

// reloads the entire menu by closing it, and writing a hook in StartMenu::UpdateTagString which waits a couple frames and then reopens the menu
void ModConfigurationMenu::ReloadMenuXML()
{
	Close();
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


void ModConfigurationMenu::ShowMenuFirstTime()
{

//	const auto sSettings = reinterpret_cast<Setting*>(0x11D1FE0);
//	menuTitle->Set(kTileValue_string, FormatString("Mod %s", sSettings->data.str));

	settingsExtra.listBox.templateName = "ModTemplate";
	settingsExtra.listBox.scrollBar = settingsExtra.listBox.parentTile->GetChild("lb_scrollbar");

	settingsMain.listBox.templateName = CMSetting::GetTemplateAlt();
	settingsMain.listBox.scrollBar = settingsMain.listBox.parentTile->GetChild("lb_scrollbar");

	if (!tagDefault)
	{
		CMTag tag;
		tag.id = "!All";
		tag.description = "Settings can be filtered by tags.";
		mapTags.emplace("!All", std::make_unique<CMTag>(tag));
		tagDefault = mapTags["!All"].get();
	}

	ReadJSONForPath(GetCurPath() / R"(Data/menus/ConfigurationMenu/)");
	ReadMCM();

	DisplaySettings("");

	fontMap.emplace(CMValue(static_cast<SInt32>(0)), "--");

	for (const auto font : FontManager::GetSingleton()->fontInfos)
		fontMap.emplace(static_cast<SInt32>(font->id), std::string(font->fontData->textures[0].fileName));
	for (const auto font : FontManager::GetSingleton()->extraFonts)
		fontMap.emplace(static_cast<SInt32>(font->id), std::string(font->fontData->textures[0].fileName));;

	Open();
}

void ModConfigurationMenu::SaveToJSON()
{
	SaveModJSON(*categoryHistory.rbegin());
	for (const auto& iter : settingsMain.listBox.list)
	{
		iter->object->Display(iter->tile);
	}
}

void ModConfigurationMenu::LoadFromJSON()
{
	LoadModJSON(*categoryHistory.rbegin());
	for (const auto& iter : settingsMain.listBox.list)
	{
		iter->object->Display(iter->tile);
	}
}

void ModConfigurationMenu::Back()
{
	if (categoryHistory.size() > 1)
	{
		categoryHistory.pop_back();
		DisplaySettings(*categoryHistory.rbegin());
	}
	else
	{
		Close();
		StartMenu::GetSingleton()->HandleUnclick(0, nullptr);
	}
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
	for (const auto& iter : settingsMain.listBox.list)
	{
		iter->object->Default();
		iter->object->Display(iter->tile);
	}
}

std::vector<CMSetting*> ModConfigurationMenu::GetSettingsForString(std::string str)
{
	std::vector<CMSetting*> ret;
	for (const auto& setting : setSettings)
	{
		if (str.empty() && setting->mods.empty()) {}
		else if (!setting->mods.contains(str)) continue;
		ret.push_back(setting.get());
	}
	return ret;
}

void ModConfigurationMenu::SettingList::UpdateTagString()
{
	const auto menu = GetSingleton();
	const auto tag = menu->mapTags[tagActive].get();

	std::string str;

	if (tag == menu->tagDefault)
	{
		const auto category = menu->mapCategories.contains(categoryActive) ? menu->mapCategories[categoryActive].get() : nullptr;
		str = category ? main ? category->GetName() : category->GetShortName() : "All";
	}
	else
	{
		str = tag ? tag->GetName() : tagActive;
	}
	tagTile->Set(kTileValue_string, str);
}

void ModConfigurationMenu::SettingList::operator<<(const std::string& tag)
{
	tagActive = tag;
	UpdateTagString();
}

void ModConfigurationMenu::SettingList::operator<<=(const std::string& tag)
{
	tags.emplace(tag);
}

void ModConfigurationMenu::SettingList::operator++()
{
	if (tags.empty()) return;
	auto iter = tags.find(tagActive);
	tagActive = iter == tags.end() || ++iter == tags.end() ? *tags.begin() : *iter;
	UpdateTagString();
}

void ModConfigurationMenu::SettingList::operator--()
{
	if (tags.empty()) return;
	auto iter = tags.find(tagActive);
	tagActive = iter == tags.end() || iter-- == tags.begin() ? *tags.rbegin() : *iter;
	UpdateTagString();
}

/*
void ModConfigurationMenu::SetActiveSubsettingValueFromInput()
{
	
	auto id = activeInputSubsetting->settingCategory;
	auto name = activeInputSubsetting->id;

	auto dataType = activeInputSubsetting->GetDataType();
	switch (dataType)
	{
	case SubSettingData::kSettingDataType_String:
	{
		activeInputSubsetting->data.valueStr = subSettingInput.GetText();
		ini.SetValue(id.c_str(), name.c_str(), activeInputSubsetting->data.valueStr.c_str());
		break;
	}
	case SubSettingData::kSettingDataType_Integer:
	{
		activeInputSubsetting->data.valueInt = std::stol(subSettingInput.GetText());
		ini.SetLongValue(id.c_str(), name.c_str(), activeInputSubsetting->data.valueInt);
		break;
	}
	case SubSettingData::kSettingDataType_Float:
	{
		activeInputSubsetting->data.valueFloat = std::stof(subSettingInput.GetText());
		ini.SetDoubleValue(id.c_str(), name.c_str(), activeInputSubsetting->data.valueFloat);
		break;
	}
	}
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
			if (auto boxBackground = tile->parent->GetChildByID(kTileID_SubsettingInputFieldText_BoxBG))
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

	
			}
			//			else
			{
				if (tile->parent)
				{
					// set the input string back to the current value of the setting
					//activeInputSubsetting->DisplaySettings(tile->parent);

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
*/
/* CLICK SETTING 
if (setting->IsInputField())
{
	auto inputFieldTextTile = clickedTile->GetChildByID(kTileID_SubsettingInputFieldText);
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
		Log(true, Log::kConsole) << FormatString("ModConfigurationMenu: Failed to find input text tile (ID: %d)", kTileID_SubsettingInputFieldText);
	}
}
}*/


bool __cdecl HideItemsNotMatchingFilterString(CMSetting* item)
{
	const auto searchStr = ModConfigurationMenu::GetSingleton()->searchBar.GetText();
	if (item->name.find(searchStr) != std::string::npos) return false;
	//	if (item->settingName.find(searchStr) != std::string::npos) return false;
	if (item->description.find(searchStr) != std::string::npos) return false;
	return true;
}

void ModConfigurationMenu::RefreshFilter()
{
	//	settingsExtra.Filter(TweakFilter);

	auto textColor = settingsExtra.listBox.GetNumVisibleItems() ? 1 : 2;
	searchBar.tile->Set(kTileValue_systemcolor, textColor);

//	if (auto selectedTile = settingsExtra.GetTileFromItem(&activeMod))
	{
		// if the selected tweak is filtered out
//		if (selectedTile->GetFloat(kTileValue_listindex) < 0)
		{
//			this->ClickExtra(nullptr);
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

void ModConfigurationMenu::DisplaySettings(std::string id)
{
	if (const auto iter = std::ranges::find(categoryHistory, id); iter == categoryHistory.end())
		categoryHistory.push_back(id);

	auto iter = std::ranges::find(categoryHistory, id);

	if (iter == categoryHistory.end()) return;

	const auto category = mapCategories.contains(id) ? mapCategories[id].get() : nullptr;

	settingsMain.Display(id, true, category ? category->allTag : true, category ? category->doublestacked : false);

	if (iter != categoryHistory.begin())
	{
		const auto& id = *--iter; ++iter;

		const auto category = mapCategories.contains(id) ? mapCategories[id].get() : nullptr;

		settingsExtra.Display(id, false, category ? category->allTag : true, false);
	}
	else
	{
		settingsExtra.Hide();
	}

	categoryHistory.erase(++iter, categoryHistory.end());
}

void ModConfigurationMenu::UpdateEscape()
{
	if (OSInputGlobals::GetSingleton()->GetControlState(OSInputGlobals::Escape_, OSInputGlobals::isPressed))
	{
		Back();
	}
}


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
	std::string inputString;
	inputString = GetText();
	if (isActive)
	{
		inputString.insert(caretIndex, isCaretShown ? "|" : " ");
		tile->Set("_CaretIndex", caretIndex);
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
				const auto whatever = GetCharsSinceSpace(input.c_str(), caretIndex);
				input.erase(caretIndex, whatever);
				caretIndex = whatever;
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
				const auto whatever = GetCharsTillSpace(input.c_str(), caretIndex) + 1;
				input.erase(caretIndex, whatever);
				caretIndex = whatever;
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
