#include <prefix.h>
#include "SI.h"

namespace SortingIcons::Hooks
{
	template <UInt32 retn> __declspec(naked) void TileSetStringValueIconInject() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(Icons::SetTileStringInjectTile);
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

	template<UInt32 retn> __declspec(naked) void TileSetStringValueIconHotkeyHUD() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(Icons::SetStringValueTagImage);
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		edx, [ebp - 0x24]
			call    SetStringValue
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void TileSetStringValueIconHotkeyPipBoy() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(Icons::SetStringValueTagRose);
		static const UInt32 retnAddr = retn;
		static const UInt32 g_inventoryMenuSelection = 0x011D9EA8;
		__asm
		{
			mov		edx, dword ptr ds : [0x011D9EA8]
			call    SetStringValue
			jmp		retnAddr
		}
	}

	inline __declspec(naked) void SortingBarterContainerMenu()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(Sorting::CompareItemsWithTags);
		_asm
		{
			pop		eax
			mov		ecx, [ebp + 0x8]	// a1
			mov		edx, [ebp + 0xC]	// a2

			call	CompareItems
			mov		esp, ebp
			pop		ebp
			ret
		}
	}
	template<UInt32 retn> __declspec(naked) void SortingInventoryMenu()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(Sorting::CompareItemsWithTags);
		static const UInt32 InventoryChanges_GetFullName = 0x4BE2D0;
		static const UInt32 retnAddr = retn;
		_asm
		{
			mov		ecx, [ebp + 0x8]	// a1
			mov		edx, [ebp + 0xC]	// a2

			call	CompareItems
			test	eax, eax
			je		got0

			mov		esp, ebp
			pop		ebp
			ret

		got0 :
			mov		edx, [ebp + 0xC]
			mov		ecx, [edx + 0x4]
			call	InventoryChanges_GetFullName
			jmp		retnAddr
		}
	}


	template<UInt32 retn1, UInt32 retn2> __declspec(naked) void KeyringHideKeys()
	{
		static const UInt32 retnAddr1 = retn1;
		static const UInt32 retnAddr2 = retn2;
		static const auto ShouldHide = reinterpret_cast<UInt32>(Sorting::KeyringHideKeys);
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

	template<UInt32 retn> __declspec(naked) void KeyringHideKeysShowCategories()
	{
		static const UInt32 retnAddr = retn;
		static const auto HasContainerEntry = reinterpret_cast<UInt32>(Sorting::HasContainerChangesEntry);
		static const auto ShowCategories = reinterpret_cast<UInt32>(Sorting::KeyringShowCategories);
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

	template<UInt32 retn> __declspec(naked) void KeyringHideNonKeys()
	{
		static const UInt32 retnAddr = retn;
		static const auto HideNonKeysAndGetTile = reinterpret_cast<UInt32>(Sorting::HideNonKeysGetTile);
		__asm
		{
			mov		edx, [ebp + 0xC]
			call	HideNonKeysAndGetTile
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void KeyringAddCategories()
	{
		static const UInt32 retnAddr = retn;
		static const auto AddCategories = reinterpret_cast<UInt32>(Sorting::AddSortingCategories);
		__asm
		{
			call	AddCategories
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void KeyringEnableEquipDrop()
	{
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		eax, 0
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void ContainerEntryListBoxFilterPre()
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

	template<UInt32 retn> __declspec(naked) void ContainerEntryListBoxFilterPost()
	{
		// fix stack
		static const UInt32 retnAddr = retn;
		__asm
		{
			add		esp, 4
			test	al, al
			jz		wah
			mov[ebp - 0x24], 1
			jmp		retnAddr
		wah :
			mov[ebp - 0x24], 0
			jmp		retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClick1()
	{
		static const UInt32 retnAddr = retn;
		static const auto GetFilter = reinterpret_cast<UInt32>(Sorting::InventoryMenuHandleClickGetFilter);
		__asm
		{
			mov ecx, [ebp + 0xC]
			push ecx
			mov ecx, [ebp - 0x64]
			mov edx, [ebp + 0x8]
			call GetFilter
			mov ecx, eax
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClick2()
	{
		static const UInt32 retnAddr = retn;
		static const auto GetFilter = reinterpret_cast<UInt32>(Sorting::InventoryMenuHandleClickGetFilter);
		__asm
		{
			mov ecx, [ebp + 0xC]
			push ecx
			mov ecx, [ebp - 0x64]
			mov edx, [ebp + 0x8]
			call GetFilter
			mov edx, eax
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClick3()
	{
		static const UInt32 retnAddr = retn;
		static const auto GetFilter = reinterpret_cast<UInt32>(Sorting::InventoryMenuHandleClickGetFilter);
		__asm
		{
			mov ecx, [ebp + 0xC]
			push ecx
			mov ecx, [ebp - 0x64]
			mov edx, [ebp + 0x8]
			call GetFilter
			push eax
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClick4()
	{
		static const UInt32 retnAddr = retn;
		static const auto SetUpData = reinterpret_cast<UInt32>(Sorting::InventoryMenuSetupData);
		__asm
		{
			mov ecx, [ebp + 0xC]
			push ecx
			mov ecx, [ebp - 0x64]
			mov edx, [ebp + 0x8]
			call SetUpData
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuHandleSpecialInput()
	{
		static const UInt32 retnAddr = retn;
		static const auto AdvanceTab = reinterpret_cast<UInt32>(Sorting::InventoryMenuChooseTab);
		__asm
		{
			mov ecx, [ebp + 0x8]
			mov edx, [ebp - 0x8]
			call AdvanceTab
			push eax
			push 0x18
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuSaveScrollPosition()
	{
		static const UInt32 retnAddr = retn;
		static const auto SaveScroll = reinterpret_cast<UInt32>(Sorting::InventoryMenuSaveScrollPosition);
		__asm
		{
			call SaveScroll
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuRestoreScrollPosition()
	{
		static const UInt32 retnAddr = retn;
		static const auto RestoreScroll = reinterpret_cast<UInt32>(Sorting::InventoryMenuRestoreScrollPosition);
		__asm
		{
			call RestoreScroll
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuShouldHideItem()
	{
		static const UInt32 retnAddr = retn;
		static const auto ShouldHide = reinterpret_cast<UInt32>(Sorting::InventoryMenuShouldHideItem);
		__asm
		{
			mov ecx, [ebp + 0x8]
			call ShouldHide
			jmp retnAddr
		}
	}
}
