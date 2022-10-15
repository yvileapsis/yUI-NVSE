#include "SI.h"
#include "SIHooks.h"
#include <SafeWrite.h>

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

			WriteRelCall(0x782F42, Sorting::KeyringEnableCancelHook);

			WriteRelCall(0x7815A6, Sorting::KeyringPipBoyIconHook);
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

		WriteRelCall(0x77FDF3, Sorting::SetUpTabline);
	//	WriteRelJump(0x78232C, 0x782362);
	}

}
