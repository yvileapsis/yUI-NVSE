#include <GameUI.h>
#include <internal/DecodedUI.h>

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

bool* g_menuVisibility = (bool*)0x11F308F;

extern  TileMenu** g_tileMenuArray;

void MenuButton_DownloadsClick() { MenuButton_Downloads(); }

ContChangesEntry** g_modMenuTarget = reinterpret_cast<ContChangesEntry**>(0x11D9F58);
BSSimpleArray<StartMenuUserOption*>* g_settingsMenuOptions = (BSSimpleArray<StartMenuUserOption*>*)0x11DAB50;

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

__declspec(naked) Tile* Menu::AddTileFromTemplate(Tile* destTile, const char* templateName, UInt32 arg3)
{
	static const UInt32 procAddr = kAddr_TileFromTemplate;
	__asm	jmp		procAddr
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

void(__thiscall* Font__CheckForVariablesInText)(FontManager::FontInfo*, const char* input, FontTextReplaced* a3) = (void(__thiscall*)(FontManager::FontInfo*, const char*, FontTextReplaced*))0xA12FB0;

FontTextReplaced::FontTextReplaced()
{
	str.m_bufLen = 0;
	str.m_data = 0;
	str.m_dataLen = 0;
	wrapWidth = 0;
	unk00C = 0;
	unk010 = 0;
	unk014 = 0;
	unk018 = 0;
	byte1C = 0;
	list20.Init();
};

FontTextReplaced::~FontTextReplaced()
{
	str.Set(NULL);
	list20.RemoveAll();
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