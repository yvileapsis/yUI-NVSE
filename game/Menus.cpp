#include <Utilities.h>
#include <Menus.h>
#include <Objects.h>

static UInt8*	g_bUpdatePlayerModel		= (UInt8*)0x011C5CB4;	// this is set to true when player confirms change of race in RaceSexMenu -

void (*ToggleMenus)(bool toggleON) = reinterpret_cast<void(*)(bool)>(0x703810);
bool(__cdecl* GetIsMenuMode)() = reinterpret_cast<bool(*)()>(0x702360);
void(__cdecl* HUDMainMenu_UpdateVisibilityState)(signed int) = reinterpret_cast<void(*)(int)>(0x771700);
void(__cdecl* MenuButton_Downloads)() = reinterpret_cast<void(*)()>(0x7D0550);

void MenuButton_DownloadsClick() { MenuButton_Downloads(); }

InventoryChanges** g_modMenuTarget = reinterpret_cast<InventoryChanges**>(0x11D9F58);


bool Menu::GetTemplateExists(const char* templateName)
{
	for (const auto node : menuTemplates)
		if (node && !_strcmpi(node->templateName, templateName)) return true;
	return false;
}

bool InventoryMenu::IsKeyringOpen()
{
	return GetSingleton()->tile->GetFloat(*(UInt32*)0x11D9EB8); // Trait_KeyringOpen
}


bool DialogMenu::IsNPCTalking()
{
	return tile->GetFloat(*(UInt32*)0x11D9500); // g_dialogMenu_TraitShowingText
}


bool IsInStartMenu()
{
	auto menu = StartMenu::GetSingleton();
	return menu && menu->flags & StartMenu::kInStartMenu;
}

StartMenuOption* StartMenuOption::Create(const char* str, void (*callback)(void), UInt32 flags)
{
	StartMenuOption* option = (StartMenuOption*)GameHeapAlloc(sizeof(StartMenuOption));
	option->vtable = (UInt32*)kVtbl_StartMenuOption;
	option->displayString = str;
	option->callback = callback;
	option->data = flags;

	return option;
}

StartMenuUserOption* StartMenuUserOption::Init(char* str, void (*callback)(StartMenuUserOption*), StartMenuOption::WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
{
	return CdeclCall<StartMenuUserOption*>(0x7D6350, this, str, callback, flags, type, numOptions, buttonWidth, optionNames);
}

StartMenuUserOption* StartMenuUserOption::Create(char* str, void(__cdecl* callback)(StartMenuUserOption*), StartMenuOption::WhichMenus flags, int type, int numOptions, int buttonWidth, char** optionNames)
{
	StartMenuUserOption* option = (StartMenuUserOption*)GameHeapAlloc(sizeof(StartMenuUserOption));
	return option->Init(str, callback, flags, type, numOptions, buttonWidth, optionNames);
}


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