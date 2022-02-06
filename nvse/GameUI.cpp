#include <Utilities.h>
#include <GameUI.h>
#include <SafeWrite.h>
#include <GameObjects.h>

UInt8* g_menuVisibility = reinterpret_cast<UInt8*>(0x011F308F);
NiTArray <TileMenu*>* g_tileMenuArray = reinterpret_cast<NiTArray<TileMenu*>*>(0x011F3508);

static const UInt32 s_RaceSexMenu__UpdatePlayerHead	= 0x007B25A0;	// End of RaceSexMenu::Func003.case0, call containing QueuedHead::Init (3rd before jmp)
static UInt8*	g_bUpdatePlayerModel		= (UInt8*)0x011C5CB4;	// this is set to true when player confirms change of race in RaceSexMenu -
																	// IF requires change of skeleton - and back to false when model updated
const _TempMenuByType TempMenuByType = (_TempMenuByType)0x00707990;	// Called from called from call RaceSexMenu::Init

bool InterfaceManager::IsMenuVisible(UInt32 menuType)
{
	if((menuType >= kMenuType_Min) && (menuType <= kMenuType_Max))
		return g_menuVisibility[menuType] != 0;

	return false;
}

Menu * InterfaceManager::GetMenuByType(UInt32 menuType)
{
	if((menuType >= kMenuType_Min) && (menuType <= kMenuType_Max))
	{
		TileMenu * tileMenu = g_tileMenuArray->Get(menuType - kMenuType_Min);
		if (tileMenu) return tileMenu->menu;
	}

	return nullptr;
}

TileMenu* TileMenu::GetTileMenu(const UInt32 menuID)
{
	return menuID ? g_tileMenuArray->Get(menuID - kMenuType_Min) : nullptr;
}

Menu * InterfaceManager::TempMenuByType(UInt32 menuType)
{
	if((menuType >= kMenuType_Min) && (menuType <= kMenuType_Max))
	{
		return ::TempMenuByType(menuType);
	}
	return NULL;
}

const char kMenuNames[] =
"MessageMenu\0InventoryMenu\0StatsMenu\0HUDMainMenu\0LoadingMenu\0ContainerMenu\0DialogMenu\0SleepWaitMenu\0StartMenu\0\
LockpickMenu\0QuantityMenu\0MapMenu\0BookMenu\0LevelUpMenu\0RepairMenu\0RaceSexMenu\0CharGenMenu\0TextEditMenu\0BarterMenu\0\
SurgeryMenu\0HackingMenu\0VATSMenu\0ComputersMenu\0RepairServicesMenu\0TutorialMenu\0SpecialBookMenu\0ItemModMenu\0LoveTesterMenu\0\
CompanionWheelMenu\0TraitSelectMenu\0RecipeMenu\0SlotMachineMenu\0BlackjackMenu\0RouletteMenu\0CaravanMenu\0TraitMenu";
const UInt32 kMenuIDs[] =
{
	kMenuType_Message, kMenuType_Inventory, kMenuType_Stats, kMenuType_HUDMain, kMenuType_Loading, kMenuType_Container,
	kMenuType_Dialog, kMenuType_SleepWait, kMenuType_Start, kMenuType_LockPick, kMenuType_Quantity, kMenuType_Map, kMenuType_Book,
	kMenuType_LevelUp, kMenuType_Repair, kMenuType_RaceSex, kMenuType_CharGen, kMenuType_TextEdit, kMenuType_Barter, kMenuType_Surgery,
	kMenuType_Hacking, kMenuType_VATS, kMenuType_Computers, kMenuType_RepairServices, kMenuType_Tutorial, kMenuType_SpecialBook,
	kMenuType_ItemMod, kMenuType_LoveTester, kMenuType_CompanionWheel, kMenuType_TraitSelect, kMenuType_Recipe, kMenuType_SlotMachine,
	kMenuType_Blackjack, kMenuType_Roulette, kMenuType_Caravan, kMenuType_Trait
};

std::unordered_map<std::string, UInt32> s_menuNameToID;

// Split component path into "top-level menu name" and "everything else".
// Path is of format "MenuType/tile/tile/..." following hierarchy defined in menu's xml.
// Returns pointer to top-level menu or NULL.
// pSlashPos is set to the slash character after the top-level menu name.
TileMenu* InterfaceManager::GetMenuByPath(const char * componentPath, const char ** pSlashPos)
{
	if (s_menuNameToID.empty())
	{
		const char *strPos = kMenuNames;
		UInt32 count = 0;
		do
		{
			s_menuNameToID[strPos] = kMenuIDs[count];
			strPos += StrLen(strPos) + 1;
		}
		while (++count < 36);
	}

	char* slashPos = SlashPos(componentPath);
	if (slashPos) *slashPos = 0;
	*pSlashPos = slashPos;
	const UInt32 menuID = s_menuNameToID[componentPath];
	return menuID ? g_tileMenuArray->Get(menuID - kMenuType_Min) : nullptr;
}

Tile::Value* InterfaceManager::GetMenuComponentValue(const char * componentPath)
{
	// path is of format "MenuType/tile/tile/.../traitName" following hierarchy defined in menu's xml
	const char *slashPos;
	TileMenu *tileMenu = GetMenuByPath(componentPath, &slashPos);
	if (tileMenu && slashPos)
		return tileMenu->GetComponentValue(slashPos + 1);
	return nullptr;
}

Tile* InterfaceManager::GetMenuComponentTile(const char * componentPath)
{
	// path is of format "MenuType/tile/tile/.../tile" following hierarchy defined in menu's xml
	const char *slashPos;
	TileMenu *tileMenu = GetMenuByPath(componentPath, &slashPos);
	if (tileMenu && slashPos)
		return tileMenu->GetComponentTile(slashPos + 1);
	return tileMenu;
}

void Debug_DumpMenus(void)
{
	for(UInt32 i = 0; i < g_tileMenuArray->Length(); i++)
	{
		TileMenu	* tileMenu = g_tileMenuArray->Get(i);

		if(tileMenu)
		{
			PrintLog("menu %d at %x:", i, tileMenu);
			gLog.Indent();

			tileMenu->Dump();

			gLog.Outdent();
		}
	}
}

void RaceSexMenu::UpdatePlayerHead(void)
{
	ThisStdCall(s_RaceSexMenu__UpdatePlayerHead, this);
}

void (*ToggleMenus)(bool toggleON) = reinterpret_cast<void(*)(bool)>(0x703810);
void (*RefreshItemsListBox)(void) = reinterpret_cast<void(*)()>(0x704AF0);
bool(__cdecl* GetIsMenuMode)() = reinterpret_cast<bool(*)()>(0x702360);
int* (__thiscall* Tile_A01000)(Tile*, enum TileValues) = (int* (__thiscall*)(Tile*, enum TileValues))0xA01000;
void(__thiscall* TileValueSetString)(int*, char*, char) = (void(__thiscall*)(int*, char*, char))0xA0A300;
//void(__thiscall* Tile__SetStringValue)(Tile*, enum TileValues, const char*, char) = (void(__thiscall*)(Tile*, enum TileValues, const char*, char))0xA01350;
//void(__thiscall* Tile__SetFloatValue)(Tile*, enum TileValues, float, char) = (void(__thiscall*)(Tile*, enum TileValues, float, char))0xA012D0;
//TileMenu* (__cdecl* Tile__GetMenuTile)(UInt32 id) = reinterpret_cast<TileMenu * (*)(UInt32)>(0xA09030);
Menu* (__thiscall* TileList__GetMenu)(Tile*) = reinterpret_cast<Menu * (__thiscall*)(Tile*)>(0xA03C90);
void(__thiscall* Menu__RegisterTile)(Menu*, Tile*, bool) = reinterpret_cast<void(__thiscall*)(Menu*, Tile*, bool)>(0xA1DC70);
void(__thiscall* Menu__HideTitle)(Menu*, bool) = reinterpret_cast<void(__thiscall*)(Menu*, bool)>(0xA1DC20);
void(__cdecl* HUDMainMenu_UpdateVisibilityState)(signed int) = reinterpret_cast<void(*)(int)>(0x771700);
void(__cdecl* MenuButton_Downloads)() = reinterpret_cast<void(*)()>(0x7D0550);
bool(*MenuVisible)(UINT32) = reinterpret_cast<bool(*)(UINT32)>(0x11F308F);
void(__thiscall* RefreshContainerMenu)(ContainerMenu*, TESForm*) = reinterpret_cast<void(__thiscall*)(ContainerMenu*, TESForm*)>(0x75C280);
void(__thiscall* RefreshBarterMenu)(BarterMenu*, TESForm*) = reinterpret_cast<void(__thiscall*)(BarterMenu*, TESForm*)>(0x72DC30);
void(__thiscall* InitEntriesRepairServices)(RepairServicesMenu*) = reinterpret_cast<void(__thiscall*)(RepairServicesMenu*)>(0x7B8B30);

void MenuButton_DownloadsClick() { MenuButton_Downloads(); }

ContChangesEntry** g_modMenuTarget = reinterpret_cast<ContChangesEntry**>(0x11D9F58);

void InterfaceManager::VATSHighlightData::AddRef(TESObjectREFR* ref)
{
	if (ref->GetNiNode())
	{
		ThisCall(0x800D50, this, ref);
		this->mode = 1;
	}
};

void InterfaceManager::VATSHighlightData::AddRefAndSetFlashing(TESObjectREFR* ref)
{
	if (ref->GetNiNode())
	{
		ThisCall(0x800E50, this, ref);
		this->mode = 1;
	}
};

Tile* Menu::AddTileFromTemplate(Tile* destTile, const char* templateName, UInt32 arg3 = 0)
{
	return ThisCall<Tile*>(0xA1DDB0, this, destTile, templateName, 0);
}

DIHookControl* g_DIHook = nullptr;

__declspec(naked) bool IsShiftHeld()
{
	_asm
	{
		mov ecx, g_DIHook
		cmp byte ptr[ecx + 0x12A], 0    // check left shift (DirectX scancode * 7 + 4)
		jne done
		cmp byte ptr[ecx + 0x17E], 0    // check right shift (DirectX scancode * 7 + 4)

		done:
		setne al
			ret
	}
}

_declspec(naked) bool IsAltHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp byte ptr[edx + 0x18C], 0    // check left alt (DirectX scancode * 7 + 4)
		setne al
		ret
	}
}

_declspec(naked) bool IsTabHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp byte ptr[edx + 0x6D], 0    // check tab (DirectX scancode * 7 + 4)
		setne al
		ret
	}
}

_declspec(naked) bool IsControlHeld()
{
	_asm
	{
		mov edx, g_DIHook
		cmp	byte ptr[edx + 0xCF], 0
		jne	done
		cmp	byte ptr[edx + 0x44F], 0
		done:
		setne al
			ret
	}
}

bool InventoryMenu::IsKeyringOpen() const
{
	return this->tile->GetValueFloat(*(UInt32*)0x11D9EB8); // Trait_KeyringOpen
}


bool DialogMenu::IsNPCTalking()
{
	return tile->GetValueFloat(*(UInt32*)0x11D9500); // g_dialogMenu_TraitShowingText
}

bool MapMenuScrollNotes(MenuSpecialKeyboardInputCode direction)
{
	MapMenu* mapMenu = MapMenu::GetSingleton();
	Tile* tile = NULL;

	if (mapMenu->currentTab == MapMenu::MapMenuTabs::kQuests)
	{
		tile = mapMenu->objectiveList.scrollBar;
	}
	else if (mapMenu->currentTab == MapMenu::MapMenuTabs::kMisc)
	{
		tile = ThisCall<Tile*>(0xA03DA0, mapMenu->tile05C, "MM_TextScrollbar");
	}

	if (!tile) return false;

	int deltaScroll = direction == kMenu_UpArrow ? -1 : 1;
	if (IsShiftHeld()) deltaScroll *= 4;

	if (tile->GetValueFloat(kTileValue_visible))
	{
		int currentValueTrait = tile->TraitNameToID("_current_value");
		float currentScroll = tile->GetValueFloat(currentValueTrait);

		currentScroll += deltaScroll;
		tile->SetFloat(currentValueTrait, currentScroll, 0);

		if (currentScroll != tile->GetValueFloat(currentValueTrait))
		{
			int setInCodeTrait = tile->TraitNameToID("_SetInCode");
			tile->SetFloat(setInCodeTrait, 1.0, 1);
			tile->SetFloat(setInCodeTrait, 0.0, 1);
		}
	}
	return true;
}

bool StatsMenuNotesScroll(StatsMenu* statsMenu, MenuSpecialKeyboardInputCode direction)
{
	Tile* tile = NULL;

	tile = statsMenu->statusEffListBox.scrollBar;
	if (!tile) return false;

	Tile* lastChild = statsMenu->tile0F8->children.Tail()->data;

	if (ThisCall<bool>(0xA040A0, tile) || lastChild->GetValueFloat(kTileValue_visible))
	{
		int deltaScroll = direction == kMenu_UpArrow ? -1 : 1;
		if (IsShiftHeld()) deltaScroll *= 4;

		if (statsMenu->tile->GetValue(kTileValue_user0))
		{
			tile = lastChild;
		}

		int currentValueTrait = tile->TraitNameToID("_current_value");
		float currentScroll = tile->GetValueFloat(currentValueTrait);

		currentScroll += deltaScroll;
		tile->SetFloat(currentValueTrait, currentScroll, 0);

		if (currentScroll != tile->GetValueFloat(currentValueTrait))
		{
			int setInCodeTrait = tile->TraitNameToID("_SetInCode");
			tile->SetFloat(setInCodeTrait, 1.0, 1);
			tile->SetFloat(setInCodeTrait, 0.0, 1);
		}
	}
	return true;
}


bool IsInStartMenu()
{
	auto menu = StartMenu::GetSingleton();
	return menu && menu->flags & StartMenu::kInStartMenu;
}


/*
 * Lifted from Stewie's Tweaks
 * RefreshItemsList calls itemsList->FreeAllTiles() and then allocates the memory for all the tiles again
 * instead skip the calls that free/allocate
 */
void RefreshItemsList()
{
//	SafeWrite16(0x75C3F5, 0); // ContainerMenu, nop JE by changing dest
//	SafeWrite16(0x75C443, 0x60EB); // ContainerMenu

	SafeWriteBuf(0x75C588, "\xA1\x3C\xEA\x1D\x01", 5);
	SafeWriteBuf(0x75C5AC, "\x8B\x8D\x44\xFF\xFF\xFF\x8B\x51\x74", 9);
	RefreshItemsListBox();
	
//	SafeWrite16(0x75C3F5, 0x0182); // ContainerMenu
//	SafeWrite16(0x75C443, 0x4D8B); // ContainerMenu
}

bool RefreshItemsListForm(TESForm* form)
{
	if (g_menuVisibility[kMenuType_Barter])
	{
		RefreshBarterMenu(BarterMenu::GetSingleton(), form);
		return true;
	}
	else if (g_menuVisibility[kMenuType_Container])
	{
		RefreshContainerMenu(ContainerMenu::GetSingleton(), form);
		return true;
	}
	else if (g_menuVisibility[kMenuType_RepairServices])
	{
		InitEntriesRepairServices(RepairServicesMenu::GetSingleton());
		return true;
	}
	else
	{
		return false;
	}
}

void QuickRefreshItemsList()
{
	SafeWriteBuf(0x78319C, "\xE8\x2F\xCE\xFF\xFF\xEB\x70", 7); // InventoryMenu, call SaveCurrentTabScrollPosition and skip reallocating tiles
	SafeWrite16(0x75C3F5, 0); // ContainerMenu, nop JE by changing dest
	SafeWrite16(0x75C443, 0x60EB); // ContainerMenu

	SafeWrite16(0x72DC9C, 0); // BarterMenu
	SafeWrite32(0x72DCD3, 0x0004B2E9); // BarterMenu, jump over code recalculating the selected items for trading

	RefreshItemsListBox();

	SafeWriteBuf(0x78319C, "\x6A\x00\x6A\x00\x68\x50\x28", 7); // InventoryMenu
	SafeWrite16(0x75C3F5, 0x0182); // ContainerMenu
	SafeWrite16(0x75C443, 0x4D8B); // ContainerMenu

	SafeWrite16(0x72DC9C, 0x01F6);
	SafeWrite32(0x72DCD3, 0x00D1840F); // jump over code recalculating the selected items for trading
}

void InterfaceManager::VATSHighlightData::ResetRefs()
{
	ThisCall(0x800ED0, this);
}

__declspec(naked) void Tile::SetFloat(UInt32 id, float fltVal, bool bPropagate)
{
	static const UInt32 procAddr = 0xA012D0;
	__asm	jmp		procAddr
}

__declspec(naked) void Tile::SetString(UInt32 id, const char* strVal, bool bPropagate)
{
	static const UInt32 procAddr = 0xA01350;
	__asm	jmp		procAddr
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

void FontTextReplaced::GetVariableEscapedText(const char* input) { Font__CheckForVariablesInText(FontManager::GetSingleton()->fontInfos[0], input, this); }

/*
 * Lifted from JIP LN
 */
__declspec(naked) UInt32 InterfaceManager::GetTopVisibleMenuID()
{
	__asm
	{
		cmp		byte ptr[ecx + 0xC], 2
		jb		retn0
		mov		eax, [ecx + 0xD0]
		add		ecx, 0x114
		test	eax, eax
		jz		stackIter
		mov		eax, [eax + 0x20]
		retn
		ALIGN 16
	stackIter:
		add		ecx, 4
		cmp[ecx], 0
		jnz		stackIter
		mov		eax, [ecx - 4]
		cmp		eax, 1
		jnz		done
		mov		ecx, 0x11F3479
		mov		eax, 0x3EA
		cmp[ecx], 0
		jnz		done
		mov		al, 0xFF
		cmp[ecx + 0x15], 0
		jnz		done
		mov		al, 0xEB
		cmp[ecx + 1], 0
		jnz		done
		mov		eax, 0x40B
		cmp[ecx + 0x21], 0
		jnz		done
		mov		al, 0x25
		cmp[ecx + 0x3B], 0
		jnz		done
	retn0 :
		xor eax, eax
	done :
		retn
	}
}

/*

static MapMenu* GetSingleton() { return *(MapMenu**)0x11DA368; };
static tList<TESObjectREFR> GetAvailableRadioStations() { return *(tList<TESObjectREFR>*)0x11DD59C; };
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
		this->clickStartPos.x = this->tile038->GetValueFloat(kTileValue_x);
		this->clickStartPos.y = this->tile038->GetValueFloat(kTileValue_y);
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


FontInfo* FontInfo::Load(const char* path, UInt32 ID)
{
	FontInfo* info = (FontInfo*)FormHeap_Allocate(sizeof(FontInfo));
	return (FontInfo*)ThisStdCall(0x00A12020, info, ID, path, 1);
}

bool FontInfo::GetName(char* out)
{
	UInt32 len = strlen(path);
	len -= 4;					// '.fnt'
	UInt32 start = len;
	while (path[start - 1] != '\\') {
		start--;
	}

	len -= start;

	memcpy(out, path + start, len);
	out[len] = 0;

	return true;
}

void Debug_DumpFontNames(void)
{
	FontInfo** fonts = FontManager::GetSingleton()->fontInfos;

	for (UInt32 i = 0; i < FontArraySize; i++)
		PrintLog("Font %d is named %s", i + 1, fonts[i]->path);

}