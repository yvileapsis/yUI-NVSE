#include <InterfaceManager.h>
#include <InterfaceMenus.h>

#include "GameObjects.h"
#include "SafeWrite.h"

UInt8* g_menuVisibility = reinterpret_cast<UInt8*>(0x011F308F);
NiTArray<TileMenu*>* g_tileMenuArray = reinterpret_cast<NiTArray<TileMenu*>*>(0x011F3508);

const _QueueUIMessage QueueUIMessage = reinterpret_cast<_QueueUIMessage>(0x007052F0);	// Called from Cmd_AddSpell_Execute

const _ShowMessageBox ShowMessageBox = reinterpret_cast<_ShowMessageBox>(0x00703E80);
const _ShowMessageBox_Callback ShowMessageBox_Callback = reinterpret_cast<_ShowMessageBox_Callback>(0x005B4A70);
const _ShowMessageBox_pScriptRefID ShowMessageBox_pScriptRefID = reinterpret_cast<_ShowMessageBox_pScriptRefID>(0x011CAC64);
const _ShowMessageBox_button ShowMessageBox_button = reinterpret_cast<_ShowMessageBox_button>(0x0118C684);

int* (__thiscall* Tile_A01000)(Tile*, enum TileValues) = (int* (__thiscall*)(Tile*, enum TileValues))0xA01000;
void(__thiscall* TileValueSetString)(int*, char*, char) = (void(__thiscall*)(int*, char*, char))0xA0A300;
void FontTextReplaced::GetVariableEscapedText(const char* input) { Font__CheckForVariablesInText(FontManager::GetSingleton()->fontInfos[0], input, this); }

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

// IF requires change of skeleton - and back to false when model updated
bool InterfaceManager::IsMenuVisible(const UInt32 menuType)
{
	return menuType >= kMenuType_Min && menuType <= kMenuType_Max ? g_menuVisibility[menuType] != 0 : false;
}

TileMenu* TileMenu::GetTileMenu(const UInt32 menuType)
{
	return menuType >= kMenuType_Min && menuType <= kMenuType_Max ? g_tileMenuArray->Get(menuType - kMenuType_Min) : nullptr;
}

Menu* InterfaceManager::GetMenuByType(const UInt32 menuType)
{
	if (const auto tileMenu = TileMenu::GetTileMenu(menuType)) return tileMenu->menu;
	return nullptr;
}

Menu* InterfaceManager::TempMenuByType(UInt32 menuType)
{
	return menuType >= kMenuType_Min && menuType <= kMenuType_Max ? CdeclCall<Menu*>(0x00707990, menuType) : nullptr;
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
TileMenu* InterfaceManager::GetMenuByPath(const char* componentPath, const char** pSlashPos)
{
	if (s_menuNameToID.empty())
	{
		const char* strPos = kMenuNames;
		UInt32 count = 0;
		do
		{
			s_menuNameToID[strPos] = kMenuIDs[count];
			strPos += StrLen(strPos) + 1;
		} while (++count < 36);
	}

	char* slashPos = SlashPos(componentPath);
	if (slashPos) *slashPos = 0;
	*pSlashPos = slashPos;
	const UInt32 menuID = s_menuNameToID[componentPath];
	return menuID ? g_tileMenuArray->Get(menuID - kMenuType_Min) : nullptr;
}

Tile::Value* InterfaceManager::GetMenuComponentValue(const char* componentPath)
{
	// path is of format "MenuType/tile/tile/.../traitName" following hierarchy defined in menu's xml
	const char* slashPos;
	TileMenu* tileMenu = GetMenuByPath(componentPath, &slashPos);
	if (tileMenu && slashPos)
		return tileMenu->GetComponentValue(slashPos + 1);
	return nullptr;
}

Tile* InterfaceManager::GetMenuComponentTile(const char* componentPath)
{
	// path is of format "MenuType/tile/tile/.../tile" following hierarchy defined in menu's xml
	const char* slashPos;
	TileMenu* tileMenu = GetMenuByPath(componentPath, &slashPos);
	if (tileMenu && slashPos)
		return tileMenu->GetComponentTile(slashPos + 1);
	return tileMenu;
}

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

/*
 * Lifted from Stewie's Tweaks
 * RefreshItemsList calls itemsList->FreeAllTiles() and then allocates the memory for all the tiles again
 * instead skip the calls that free/allocate
 */
 /*
 void RefreshItemsList()
 {
	 SafeWriteBuf(0x75C588, "\xA1\x3C\xEA\x1D\x01", 5);
	 SafeWriteBuf(0x75C5AC, "\x8B\x8D\x44\xFF\xFF\xFF\x8B\x51\x74", 9);
	 InterfaceManager::RefreshItemsList();
 }
 */
bool InterfaceManager::RefreshItemsListForm(TESForm* form)
{
	bool refresh = false;
	if (IsMenuVisible(kMenuType_Barter))
	{
		BarterMenu::GetSingleton()->Refresh(form);
		refresh = true;
	}
	if (IsMenuVisible(kMenuType_Container))
	{
		ContainerMenu::GetSingleton()->Refresh(form);
		refresh = true;
	}
	if (IsMenuVisible(kMenuType_RepairServices))
	{
		RepairServicesMenu::GetSingleton()->Refresh();
		refresh = true;
	}
	return refresh;
}

void InterfaceManager::RefreshItemsListQuick()
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

void Debug_DumpMenus(void)
{
	for (UInt32 i = 0; i < g_tileMenuArray->Length(); i++)
	{
		TileMenu* tileMenu = g_tileMenuArray->Get(i);

		if (tileMenu)
		{
			PrintLog("menu %d at %x:", i, tileMenu);
			gLog.Indent();

			tileMenu->Dump();

			gLog.Outdent();
		}
	}
}