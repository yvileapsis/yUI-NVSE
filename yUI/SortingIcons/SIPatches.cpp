#include "SI.h"
#include <SafeWrite.h>

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
		static const auto CompareItems = reinterpret_cast<UInt32>(Sorting::CompareItems);
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
		static const auto CompareItems = reinterpret_cast<UInt32>(Sorting::CompareItems);
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
		static const auto ShouldHide = reinterpret_cast<UInt32>(Keyrings::KeyringHideKeys);
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
		static const auto HasContainerEntry = reinterpret_cast<UInt32>(Keyrings::HasContainerChangesEntry);
		static const auto ShowCategories = reinterpret_cast<UInt32>(Keyrings::KeyringShowCategories);
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
		static const auto HideNonKeysAndGetTile = reinterpret_cast<UInt32>(Keyrings::HideNonKeysGetTile);
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
		static const auto AddCategories = reinterpret_cast<UInt32>(Keyrings::AddSortingCategories);
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
		static const auto GetFilter = reinterpret_cast<UInt32>(Tabs::InventoryMenuHandleClickGetFilter);
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
		static const auto GetFilter = reinterpret_cast<UInt32>(Tabs::InventoryMenuHandleClickGetFilter);
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
		static const auto GetFilter = reinterpret_cast<UInt32>(Tabs::InventoryMenuHandleClickGetFilter);
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
		static const auto SetUpData = reinterpret_cast<UInt32>(Tabs::InventoryMenuSetupData);
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
		static const auto AdvanceTab = reinterpret_cast<UInt32>(Tabs::InventoryMenuChooseTab);
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
		static const auto SaveScroll = reinterpret_cast<UInt32>(Keyrings::InventoryMenuSaveScrollPosition);
		__asm
		{
			call SaveScroll
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuRestoreScrollPosition()
	{
		static const UInt32 retnAddr = retn;
		static const auto RestoreScroll = reinterpret_cast<UInt32>(Keyrings::InventoryMenuRestoreScrollPosition);
		__asm
		{
			call RestoreScroll
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void InventoryMenuShouldHideItem()
	{
		static const UInt32 retnAddr = retn;
		static const auto ShouldHide = reinterpret_cast<UInt32>(Tabs::InventoryMenuShouldHideItem);
		__asm
		{
			mov ecx, [ebp + 0x8]
			call ShouldHide
			jmp retnAddr
		}
	}
}

namespace SortingIcons::Patches
{
	void AlterSorting(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x782516, Hooks::SortingInventoryMenu<0x78251B>);
			WriteRelJump(0x72F0ED, Hooks::SortingBarterContainerMenu);
			WriteRelJump(0x75D138, Hooks::SortingBarterContainerMenu);
		}
		else
		{
			UndoSafeWrite(0x782516);
			UndoSafeWrite(0x72F0ED);
			UndoSafeWrite(0x75D138);
		}
	}

	void AddIcons(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x71A3D5, Hooks::TileSetStringValueIconInject<0x71A3DA>);
		}
		else
		{
			UndoSafeWrite(0x71A3D5);
		}
	}

	void ReplaceHotkeyIcons(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x70189E, Hooks::TileSetStringValueIconHotkeyHUD<0x7018A3>);
			WriteRelJump(0x7814FA, Hooks::TileSetStringValueIconHotkeyPipBoy<0x7814FF>);
		}
		else
		{
			UndoSafeWrite(0x70189E);
			UndoSafeWrite(0x7814FA);
		}
	}

	void AddKeyrings(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x730C81, Hooks::ContainerEntryListBoxFilterPre<0x730C8C>);
			WriteRelJump(0x730C97, Hooks::ContainerEntryListBoxFilterPost<0x730CA9>);

			WriteRelJump(0x7824F6, Hooks::SortingInventoryMenu<0x78251B>);
			WriteRelJump(0x78250B, Hooks::SortingInventoryMenu<0x78251B>);

			WriteRelJump(0x7831C1, Hooks::KeyringAddCategories<0x783213>);

			WriteRelJump(0x7826E4, Hooks::KeyringHideKeys<0x7826EA, 0x7826F1>);
			WriteRelJump(0x78083A, Hooks::KeyringHideNonKeys<0x78083F>);

			WriteRelJump(0x782665, Hooks::KeyringHideKeysShowCategories<0x782679>);

			WriteRelJump(0x780478, Hooks::KeyringEnableEquipDrop<0x78047D>);
			WriteRelJump(0x780934, Hooks::KeyringEnableEquipDrop<0x78093A>);
			WriteRelJump(0x781E6D, Hooks::KeyringEnableEquipDrop<0x781E72>);

			WriteRelCall(0x782F42, Keyrings::KeyringEnableCancelHook);

			WriteRelCall(0x7815A6, Keyrings::KeyringPipBoyIconHook);
		}
		else
		{
			UndoSafeWrite(0x730C81);
			UndoSafeWrite(0x730C97);

			UndoSafeWrite(0x7824F6);
			UndoSafeWrite(0x78250B);

			UndoSafeWrite(0x7831C1);

			UndoSafeWrite(0x7826E4);
			UndoSafeWrite(0x78083A);

			UndoSafeWrite(0x782665);

			UndoSafeWrite(0x780478);
			UndoSafeWrite(0x780934);
			UndoSafeWrite(0x781E6D);

			UndoSafeWrite(0x782F42);

			UndoSafeWrite(0x7815A6);
		}
	}

	void AddPromptIcon()
	{
		WriteRelCall(0x7786CF, Icons::Tile__PropagateIntValue);
	}

	void AddTabs(const bool bEnable)
	{
	//	WriteRelCall(0x523A11, MyFunc2);
	//	WriteRelCall(0x524019, MyFunc2);

	//	WriteRelCall(0x524468, WeaponSpread); // weapon spread
	//	WriteRelCall(0x963056, MyFunc2); // player spread
	//	WriteRelCall(0x96344F, MyFunc2); // scope

	//	WriteRelJump(0x77FCE7, 0x77FCEC);
	//	WriteRelJump(0x7FB064, 0x7FB069);
	//	WriteRelJump(0x7DA376, 0x7DA37B);
		WriteRelJump(0x7801B2, Hooks::InventoryMenuHandleClick1<0x7801B8>);
		WriteRelJump(0x780215, Hooks::InventoryMenuHandleClick2<0x78021B>);
		WriteRelJump(0x78027C, Hooks::InventoryMenuHandleClick3<0x780281>);
		WriteRelJump(0x7802C9, Hooks::InventoryMenuHandleClick4<0x780386>);
		WriteRelJump(0x78232C, Hooks::InventoryMenuHandleSpecialInput<0x782371>);
		WriteRelJump(0x77FFD6, Hooks::InventoryMenuSaveScrollPosition<0x7800BB>);
		WriteRelJump(0x7800C6, Hooks::InventoryMenuRestoreScrollPosition<0x78013B>);
		WriteRelJump(0x782704, Hooks::InventoryMenuShouldHideItem<0x7827F1>);

		WriteRelCall(0x77FDF3, Tabs::SetUpTabline);
	//	WriteRelJump(0x78232C, 0x782362);
	}

}