#include <Utilities.hpp>
#include <Menu.h>
#include <TESObjectREFR.h>
#include <Safewrite.hpp>

static UInt8*	g_bUpdatePlayerModel		= (UInt8*)0x011C5CB4;	// this is set to true when player confirms change of race in RaceSexMenu -

void (*ToggleMenus)(bool toggleON) = reinterpret_cast<void(*)(bool)>(0x703810);
bool(__cdecl* GetIsMenuMode)() = reinterpret_cast<bool(*)()>(0x702360);
void(__cdecl* HUDMainMenu_UpdateVisibilityState)(signed int) = reinterpret_cast<void(*)(int)>(0x771700);
void(__cdecl* MenuButton_Downloads)() = reinterpret_cast<void(*)()>(0x7D0550);

void MenuButton_DownloadsClick() { MenuButton_Downloads(); }

InventoryChanges** g_modMenuTarget = reinterpret_cast<InventoryChanges**>(0x11D9F58);

UInt8* menuVisibility = reinterpret_cast<UInt8*>(0x011F308F);
NiTArray<TileMenu*>* tileMenuArray = reinterpret_cast<NiTArray<TileMenu*>*>(0x011F3508);
// IF requires change of skeleton - and back to false when model updated
bool Menu::IsVisible(const UInt32 menuType) { return menuType >= kMenuType_Min && menuType <= kMenuType_Max ? menuVisibility[menuType] : false; }
bool Menu::IsVisible() const { return IsVisible(id); }
TileMenu* Menu::GetTileMenu(const UInt32 menuType) { return menuType >= kMenuType_Min && menuType <= kMenuType_Max ? tileMenuArray->Get(menuType - kMenuType_Min) : nullptr; }
TileMenu* Menu::GetTileMenu() const { return tileMenuArray->Get(id - kMenuType_Min); }

Menu* Menu::GetMenu(const UInt32 menuType)
{
	const auto tileMenu = GetTileMenu(menuType);
	return tileMenu ? tileMenu->menu : nullptr;
}

std::unordered_map<std::string, UInt32> menuNameToID = {
	{"MessageMenu", kMenuType_Message},			{"InventoryMenu", kMenuType_Inventory},				{"StatsMenu", kMenuType_Stats},
	{"HUDMainMenu", kMenuType_HUDMain},			{"LoadingMenu", kMenuType_Loading},					{"ContainerMenu", kMenuType_Container},
	{"DialogMenu", kMenuType_Dialog},			{"SleepWaitMenu", kMenuType_SleepWait},				{"StartMenu", kMenuType_Start},
	{"LockpickMenu", kMenuType_LockPick},		{"QuantityMenu", kMenuType_Quantity},				{"MapMenu", kMenuType_Map},
	{"BookMenu", kMenuType_Book},				{"LevelUpMenu", kMenuType_LevelUp},					{"RepairMenu", kMenuType_Repair},
	{"RaceSexMenu", kMenuType_RaceSex},			{"CharGenMenu", kMenuType_CharGen},					{"TextEditMenu", kMenuType_TextEdit},
	{"BarterMenu", kMenuType_Barter},			{"SurgeryMenu", kMenuType_Surgery},					{"HackingMenu", kMenuType_Hacking},
	{"VATSMenu", kMenuType_VATS},				{"ComputersMenu", kMenuType_Computers},				{"RepairServicesMenu", kMenuType_RepairServices},
	{"TutorialMenu", kMenuType_Tutorial},		{"SpecialBookMenu", kMenuType_SpecialBook},			{"ItemModMenu", kMenuType_ItemMod},
	{"LoveTesterMenu", kMenuType_LoveTester},	{"CompanionWheelMenu", kMenuType_CompanionWheel},	{"TraitSelectMenu", kMenuType_TraitSelect},
	{"RecipeMenu", kMenuType_Recipe},			{"SlotMachineMenu", kMenuType_SlotMachine},			{"BlackjackMenu", kMenuType_Blackjack},
	{"RouletteMenu", kMenuType_Roulette},		{"CaravanMenu", kMenuType_Caravan},					{"TraitMenu", kMenuType_Trait},
};

TileMenu* Menu::GetTileMenu(const std::string& componentPath) 
{
	return GetTileMenu(menuNameToID[componentPath]);
}

Menu* Menu::GetMenu(const std::string& componentPath) 
{
	const auto tileMenu = GetTileMenu(componentPath);
	return tileMenu ? tileMenu->menu : nullptr;
}

Menu* Menu::TempMenuByType(const UInt32 menuType)
{
	return menuType >= kMenuType_Min && menuType <= kMenuType_Max ? CdeclCall<Menu*>(0x00707990, menuType) : nullptr;
}

bool Menu::GetTemplateExists(const std::string& templateName) const
{
	for (const auto node : menuTemplates) if (templateName == node->templateName) return true;
	return false;
}

bool InventoryMenu::IsKeyringOpen() const
{
	return tile->Get(static_cast<TileValueIDs>(*reinterpret_cast<UInt32*>(0x11D9EB8))); // Trait_KeyringOpen
}

bool DialogMenu::IsNPCTalking() const
{
	return tile->Get(static_cast<TileValueIDs>(*reinterpret_cast<UInt32*>(0x11D9500))); // g_dialogMenu_TraitShowingText
}

bool IsInStartMenu()
{
	const auto menu = StartMenu::GetSingleton();
	return menu && menu->flags & StartMenu::kInStartMenu;
}

/*
StartMenu::Option* StartMenu::Option::Create(const char* str, void (*callback)(), UInt32 flags)
{
	StartMenuOption* option = (StartMenuOption*)GameHeapAlloc(sizeof(StartMenuOption));
	*reinterpret_cast<UInt32*>(option) = kVtbl_StartMenuOption;
	option->displayString = str;
	option->callback = callback;
	option->data = flags;

	return option;
}
*/

/*
StartMenuUserOption* StartMenuUserOption::Init(char* str, void (*callback)(StartMenuUserOption*), StartMenuOption::WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
{
	return 
}

StartMenuUserOption* StartMenuUserOption::Create(char* str, void(__cdecl* callback)(StartMenuUserOption*), StartMenuOption::WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
{
	StartMenuUserOption* option = (StartMenuUserOption*)GameHeapAlloc(sizeof(StartMenuUserOption));
	return option->Init(str, callback, flags, type, numOptions, buttonWidth, optionNames);
}
*/

/*

static MapMenu* GetSingleton() { return *(MapMenu**)0x11DA368; };
static TList<TESObjectREFR> GetAvailableRadioStations() { return *(TList<TESObjectREFR>*)0x11DD59C; };
static TESObjectREFR* GetCurrentRadioStation() { return *(TESObjectREFR**)0x11DD42C; };

void Zoom(bool direction, float amount) { ThisCall(0x79C5A0, this, direction, amount); };

void Move(float deltaX, float deltaY)
{
	if (!this->tileWorldMap || (this->currentTab != MapMenu::kLocalMap && this->currentTab != MapMenu::kWorldMap))
	{
		return;
	}

	this->tileWorldMap->SetFloat(kTileValue_dragstartx, 1.0F, 1);
	this->tileWorldMap->SetFloat(kTileValue_dragstarty, 1.0F, 1);

	ThisCall(0xA01290, this->tileWorldMap, kTileValue_dragdeltax, deltaX);
	ThisCall(0xA01290, this->tileWorldMap, kTileValue_dragdeltay, deltaY);

	if (this->tile038)
	{
		this->clickStartPos.x = this->tile038->GetFloat(kTileValue_x);
		this->clickStartPos.y = this->tile038->GetFloat(kTileValue_y);
	}
}

void MoveDirection(MenuSpecialKeyboardInputCode direction)
{
	switch (direction)
	{
	case kMenu_UpArrow:
		this->Move(0.0F, 20.0F);
		break;
	case kMenu_DownArrow:
		this->Move(0.0F, -20.0F);
		break;
	case kMenu_LeftArrow:
		this->Move(20.0F, 0.0F);
		break;
	case kMenu_RightArrow:
		this->Move(-20.0F, 0.0F);
		break;
	}
}

void RefreshWorldMap()
{
	ThisCall(0xA04150, this->tile038);
	this->mapMarkerList.RemoveAll();

	// skip code moving the cursor
	WriteRelJump(0x79DF73, 0x79E053);

	// MapMenu::SetupWorldMap
	ThisCall(0x79CDB0, this);

	// restore code moving the cursor
	SafeWriteBuf(0x79DF73, "\x6A\x00\x68\xB1\x0F", 5);
}

void RefreshLocalMap()
{
	ThisCall(0xA04150, this->tile030);

	// skip code moving the cursor
	WriteRelJump(0x79DF73, 0x79E053);

	// MapMenu::SetupLocalMap
	ThisCall(0x79D410, this);

	// restore code moving the cursor
	SafeWriteBuf(0x79DF73, "\x6A\x00\x68\xB1\x0F", 5);
}
};*/

Tile::Value* StringToTilePath(const std::string& componentPath)
{
	const auto firstSlash = componentPath.find_first_of('/');
	const auto lastSlash = componentPath.find_last_of('/');
	const auto tileMenu = Menu::GetTileMenu(componentPath.substr(0, firstSlash));
	const auto tile = tileMenu->GetComponent(componentPath.substr(firstSlash + 2, lastSlash));
	const auto component = Tile::TraitNameToID(componentPath.substr(lastSlash + 2).data());
	return tile->GetValue(component);
}

/*
 * Lifted from Stewie's Tweaks
 * RefreshItemsList calls itemsList->FreeAllTiles() and then allocates the memory for all the tiles again
 * instead skip the calls that free/allocate
 */

bool Menu::RefreshItemsListForm(TESForm* form)
{
	bool refresh = false;
	if (const auto menu = BarterMenu::GetSingleton(); menu->IsVisible())
	{
		menu->Refresh(form);
		refresh = true;
	}
	if (const auto menu = ContainerMenu::GetSingleton(); menu->IsVisible())
	{
		menu->Refresh(form);
		refresh = true;
	}
	if (const auto menu = RepairServicesMenu::GetSingleton(); menu->IsVisible())
	{
		menu->Refresh();
		refresh = true;
	}
	return refresh;
}

void Menu::RefreshItemsListQuick()
{
	SafeWriteBuf(0x78319C, "\xE8\x2F\xCE\xFF\xFF\xEB\x70", 7); // InventoryMenu, call SaveCurrentTabScrollPosition and skip reallocating tiles

	SafeWrite16(0x75C3F5, 0); // ContainerMenu, nop JE by changing dest
	SafeWrite16(0x75C443, 0x60EB); // ContainerMenu

	SafeWrite16(0x72DC9C, 0); // BarterMenu
	SafeWrite32(0x72DCD3, 0x0004B2E9); // BarterMenu, jump over code recalculating the selected items for trading

	RefreshItemsList();

	SafeWriteBuf(0x78319C, "\x6A\x00\x6A\x00\x68\x50\x28", 7); // InventoryMenu

	SafeWrite16(0x75C3F5, 0x0182); // ContainerMenu
	SafeWrite16(0x75C443, 0x4D8B); // ContainerMenu

	SafeWrite16(0x72DC9C, 0x01F6);
	SafeWrite32(0x72DCD3, 0x00D1840F); // jump over code recalculating the selected items for trading
}
