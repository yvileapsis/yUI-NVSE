#pragma once
#include <filesystem>
#include <GameTiles.h>
#include <GameData.h>
#include <GameUI.h>
#include <GameExtraData.h>
#include <unordered_set>

#include "file.h"

//alignas(16) char s_strValBuffer[0x10000];

extern TileMenu* g_HUDMainMenu, * g_StartMenu, * g_BarterMenu, * g_ContainerMenu, * g_RepairMenu, * g_RepairServicesMenu, * g_MapMenu, * g_StatsMenu, * g_InventoryMenu;

extern tList<char> queuedConsoleMessages;
extern DataHandler* g_dataHandler;
extern NiTPointerMap<TESForm>** g_allFormsMap;

namespace SI
{
	inline std::unordered_map <TESForm*, std::string>		g_Items;
	inline std::unordered_map <std::string, JSONEntryTag>	g_Tags;
	inline std::unordered_set<std::string>					g_Categories;
	inline std::vector<std::filesystem::path>				g_XMLPaths;

	
	void InjectTemplates();
	void KeyringRefreshPostStewie();
	std::string GetTagForItem(TESForm* form);
	std::string GetTagForItem(ContChangesEntry* entry);
	bool KeyringHideNonKeys(ContChangesEntry*);

	void __fastcall SetTileStringInjectTile(Tile* tile, ContChangesEntry* entry, MenuItemEntryList* list, UInt32 tilevalue, char* tileText, bool propagate);
	bool __fastcall HasContainerChangesEntry(ContChangesEntry* entry);
	bool __fastcall KeyringShowCategories(Tile* tile);
	void __fastcall AddSortingCategories();
	bool __fastcall KeyringHideKeys(ContChangesEntry* entry);
	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile);
	signed int __fastcall CompareItemsWithTags(ContChangesEntry* a2, ContChangesEntry* a1, Tile* tile1, Tile* tile2);
	void __fastcall SetStringValueTagImage(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate);
	void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate);

}

namespace SI_Files
{
	inline std::vector<JSONEntryItem>						g_Items_JSON;
	inline std::vector<JSONEntryTag>						g_Tags_JSON;
	
	bool AssignTagToItem(TESForm* form);
}

namespace SI_Hooks
{

	template <UInt32 retn> __declspec(naked) void IconInjectTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetTileStringInjectTile);
		static const UInt32 retnAddr = retn;
		__asm
		{
//			mov		edx, [ebp - 0x10]	//tile
//			push	edx
			mov		edx, [ebp - 0x2C]	//menu item entry list
			push	edx
			mov		edx, [ebp + 0x8]	//entry
			call    SetStringValue
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void SortingInventoryMenuHook()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(SI::CompareItemsWithTags);
		static const UInt32 ContChangesEntry_GetFullName = 0x4BE2D0;
		static const UInt32 retnAddr = retn;
		_asm
		{
			mov		eax, [ebp + 0xC]	// a2
			mov		ecx, [eax]
			push	ecx					// tile2
			mov		ecx, [eax + 0x4]	// entry2

			mov		eax, [ebp + 0x8]	// a1
			mov		edx, [eax]
			push	edx					// tile1
			mov		edx, [eax + 0x4]	// entry1

			call	CompareItems
			test	eax, eax
			je		got0

			mov		esp, ebp
			pop		ebp
			ret

		got0 :
			mov		edx, [ebp + 0xC]
			mov		ecx, [edx + 0x4]
			call	ContChangesEntry_GetFullName
			jmp		retnAddr
		}
	}
	void SortingBarterContainerMenuHook();

	template<UInt32 retn> __declspec(naked) void IconHotkeyHUDTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetStringValueTagImage);
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		edx, [ebp - 0x24]
			call    SetStringValue
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void IconHotkeyPipBoyTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetStringValueTagRose);
		static const UInt32 retnAddr = retn;
		static const UInt32 g_inventoryMenuSelection = 0x011D9EA8;
		__asm
		{
			mov		edx, dword ptr ds : [0x011D9EA8]
			call    SetStringValue
			jmp		retnAddr
		}
	}


	template<UInt32 retn1, UInt32 retn2> __declspec(naked) void KeyringHideKeysHook()
	{
		static const UInt32 retnAddr1 = retn1;
		static const UInt32 retnAddr2 = retn2;
		static const auto ShouldHide = reinterpret_cast<UInt32>(SI::KeyringHideKeys);
		__asm
		{
			mov		ecx, [ebp + 0x8] // a1
			call	ShouldHide
			test	al, al
			jz		shouldnot
			jmp		retnAddr1
		shouldnot :
			jmp		retnAddr2
		}
	}
	
	template<UInt32 retn> __declspec(naked) void KeyringHideKeysShowCategoriesHook()
	{
		static const UInt32 retnAddr = retn;
		static const auto HasContainerEntry = reinterpret_cast<UInt32>(SI::HasContainerChangesEntry);
		static const auto ShowCategories = reinterpret_cast<UInt32>(SI::KeyringShowCategories);
		__asm
		{
			mov		ecx, [ebp + 0x8] // a1
			call	HasContainerEntry
			test	al, al
			jz		hasnot

			mov		ecx, [ebp + 0xC]
			call	ShowCategories
			mov		esp, ebp
			pop		ebp
			ret
		hasnot :
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void KeyringHideNonKeysHook()
	{
		static const UInt32 retnAddr = retn;
		static const auto HideNonKeysAndGetTile = reinterpret_cast<UInt32>(SI::HideNonKeysGetTile);
		__asm
		{
			mov		edx, [ebp + 0xC]
			call	HideNonKeysAndGetTile
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void KeyringAddCategoriesHook()
	{
		static const UInt32 retnAddr = retn;
		static const auto AddCategories = reinterpret_cast<UInt32>(SI::AddSortingCategories);
		__asm
		{
			call	AddCategories
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void ContainerEntryListBoxFilterHookPre()
	{
		// push additional arg to filter function
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		ecx, [ecx]					// ListBox::ListItem*
			push	dword ptr ds : [ecx]		// ListItem->tile
			push	dword ptr ds : [ecx + 4]	// ListItem->object	
			jmp		retnAddr
//			call	dword ptr ss : [ebp + 8]	// shouldHide
//			pop		ecx							// pop the extra pushed arg (ListItem->tile)
		}
	}

	template<UInt32 retn> __declspec(naked) void ContainerEntryListBoxFilterHookPost()
	{
		// fix stack
		static const UInt32 retnAddr = retn;
		__asm
		{
			add		esp, 4
			test	al, al
			jz		wah
			mov		[ebp - 0x24], 1
			jmp		retnAddr
		wah :
			mov		[ebp - 0x24], 0
			jmp		retnAddr
		}
	}
	template<UInt32 retn> __declspec(naked) void KeyringEnableEquipDropHook()
	{
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		eax, 0
			jmp		retnAddr
		}
	}

	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, signed int a1);
	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* string, int propagate);
}

//void InjectIconTile(Tile* tile, ContChangesEntry* entry);