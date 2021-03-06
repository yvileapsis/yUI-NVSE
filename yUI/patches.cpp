#include <patches.h>
#include <SafeWrite.h>
#include <functions.h>
#include <SortingIcons.h>

void patchSortingHooks(const bool bEnable)
{
	if (bEnable)
	{
		WriteRelJump(0x782516, SIHooks::SortingInventoryMenuHook<0x78251B>);
		WriteRelJump(0x72F0ED, SIHooks::SortingBarterContainerMenuHook);
		WriteRelJump(0x75D138, SIHooks::SortingBarterContainerMenuHook);
	}
	else
	{
		UndoSafeWrite(0x782516);
		UndoSafeWrite(0x72F0ED);
		UndoSafeWrite(0x75D138);
	}
}

void patchAddIcons(const bool bEnable)
{
	if (bEnable)
	{
		WriteRelJump(0x71A3D5, SIHooks::IconInjectTileSetStringValueHook<0x71A3DA>);
	}
	else
	{
		UndoSafeWrite(0x71A3D5);
	}
}

void patchReplaceHotkeyIcons(const bool bEnable)
{
	if (bEnable)
	{
		WriteRelJump(0x70189E, SIHooks::IconHotkeyHUDTileSetStringValueHook<0x7018A3>);
		WriteRelJump(0x7814FA, SIHooks::IconHotkeyPipBoyTileSetStringValueHook<0x7814FF>);
	}
	else
	{
		UndoSafeWrite(0x70189E);
		UndoSafeWrite(0x7814FA);
	}
}

void patchSortingCategories(const bool bEnable)
{
	if (bEnable)
	{
		WriteRelJump(0x730C81, SIHooks::ContainerEntryListBoxFilterHookPre<0x730C8C>);
		WriteRelJump(0x730C97, SIHooks::ContainerEntryListBoxFilterHookPost<0x730CA9>);

		WriteRelJump(0x7824F6, SIHooks::SortingInventoryMenuHook<0x78251B>);
		WriteRelJump(0x78250B, SIHooks::SortingInventoryMenuHook<0x78251B>);

		WriteRelJump(0x7831C1, SIHooks::KeyringAddCategoriesHook<0x783213>);

		WriteRelJump(0x7826E4, SIHooks::KeyringHideKeysHook<0x7826EA, 0x7826F1>);
		WriteRelJump(0x78083A, SIHooks::KeyringHideNonKeysHook<0x78083F>);

		WriteRelJump(0x782665, SIHooks::KeyringHideKeysShowCategoriesHook<0x782679>);

		WriteRelJump(0x780478, SIHooks::KeyringEnableEquipDropHook<0x78047D>);
		WriteRelJump(0x780934, SIHooks::KeyringEnableEquipDropHook<0x78093A>);
		WriteRelJump(0x781E6D, SIHooks::KeyringEnableEquipDropHook<0x781E72>);

		WriteRelCall(0x782F42, SIHooks::KeyringEnableCancelHook);

		WriteRelCall(0x7815A6, SIHooks::KeyringPipBoyIconHook);
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

template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClickHook1()
{
	static const UInt32 retnAddr = retn;
	static const auto GetFilter = reinterpret_cast<UInt32>(SI::InventoryMenuHandleClickGetFilter);
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

template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClickHook2()
{
	static const UInt32 retnAddr = retn;
	static const auto GetFilter = reinterpret_cast<UInt32>(SI::InventoryMenuHandleClickGetFilter);
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

template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClickHook3()
{
	static const UInt32 retnAddr = retn;
	static const auto GetFilter = reinterpret_cast<UInt32>(SI::InventoryMenuHandleClickGetFilter);
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

template <UInt32 retn> __declspec(naked) void InventoryMenuHandleClickHook4()
{
	static const UInt32 retnAddr = retn;
	static const auto SetUpData = reinterpret_cast<UInt32>(SI::InventoryMenuSetupData);
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

template <UInt32 retn> __declspec(naked) void InventoryMenuHandleSpecialInputHook()
{
	static const UInt32 retnAddr = retn;
	static const auto AdvanceTab = reinterpret_cast<UInt32>(SI::InventoryMenuChooseTab);
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

template <UInt32 retn> __declspec(naked) void InventoryMenuSaveScrollPositionHook()
{
	static const UInt32 retnAddr = retn;
	static const auto SaveScroll = reinterpret_cast<UInt32>(SI::InventoryMenuSaveScrollPosition);
	__asm
	{
		call SaveScroll
		jmp retnAddr
	}
}

template <UInt32 retn> __declspec(naked) void InventoryMenuRestoreScrollPositionHook()
{
	static const UInt32 retnAddr = retn;
	static const auto RestoreScroll = reinterpret_cast<UInt32>(SI::InventoryMenuRestoreScrollPosition);
	__asm
	{
		call RestoreScroll
		jmp retnAddr
	}
}

template <UInt32 retn> __declspec(naked) void InventoryMenuShouldHideItemHook()
{
	static const UInt32 retnAddr = retn;
	static const auto ShouldHide = reinterpret_cast<UInt32>(SI::InventoryMenuShouldHideItem);
	__asm
	{
		mov ecx, [ebp + 0x8]
		call ShouldHide
		jmp retnAddr
	}
}

float __cdecl WeaponSpread(float a1, float a2)
{
	return a2;
};

float __fastcall MyFunc2(Actor* actor, void* edx, int i)
{
	return 0;
}

void patchSortingTabs(const bool bEnable)
{
//	WriteRelCall(0x523A11, MyFunc2);
//	WriteRelCall(0x524019, MyFunc2);

	WriteRelCall(0x524468, WeaponSpread); // weapon spread
//	WriteRelCall(0x963056, MyFunc2); // player spread
//	WriteRelCall(0x96344F, MyFunc2); // scope

	Log("wa");
	//	WriteRelJump(0x77FCE7, 0x77FCEC);
//	WriteRelJump(0x7FB064, 0x7FB069);
//	WriteRelJump(0x7DA376, 0x7DA37B);
	WriteRelJump(0x7801B2, InventoryMenuHandleClickHook1<0x7801B8>);
	WriteRelJump(0x780215, InventoryMenuHandleClickHook2<0x78021B>);
	WriteRelJump(0x78027C, InventoryMenuHandleClickHook3<0x780281>);
	WriteRelJump(0x7802C9, InventoryMenuHandleClickHook4<0x780386>);
	WriteRelJump(0x78232C, InventoryMenuHandleSpecialInputHook<0x782371>);
	WriteRelJump(0x77FFD6, InventoryMenuSaveScrollPositionHook<0x7800BB>);
	WriteRelJump(0x7800C6, InventoryMenuRestoreScrollPositionHook<0x78013B>);
	WriteRelJump(0x782704, InventoryMenuShouldHideItemHook<0x7827F1>);

	WriteRelCall(0x77FDF3, SI::SetUpTabline);
	//	WriteRelJump(0x78232C, 0x782362);
}


void patchFixDroppedItems(const bool bEnable)
{
	if (bEnable)
	{
		WriteRelCall(0x75C793, FixGetDroppedWeaponPre);
		WriteRelJump(0x75C9AB, FixGetDroppedWeaponPost<0x75C798>);

//		WriteRelJump(0x75C9DB, 0x75C9F1);
//		WriteRelJump(0x75CA30, 0x75CA46);
	}
	else
	{
		UndoSafeWrite(0x75C793);
		UndoSafeWrite(0x75C9AB);
	}
}

void patchMatchedCursor(const bool bEnable)
{
	if (bEnable)
	{
		WriteRelCall(0x70B33A, CursorTileSetStringValue);
		WriteRelCall(0x70C727, CursorTileSetIntValue);
	}
	else
	{
		UndoSafeWrite(0x70B33A);
		UndoSafeWrite(0x70C727);
	}
}

void patch1080pUI()
{

	//	WriteRelCall(0x70B9CE, UInt32(Tile_SetStringValueCursor));
//	WriteRelCall(0x70B320, UInt32(Tile_SetStringValueCursor));
//	WriteRelCall(0x70CC1F, UInt32(Tile_SetStringValueCursor));

	WriteRelJump(0x715D54, UInt32(UIWidth));
	WriteRelCall(0x715DA6, 0x4DC200);
	WriteRelJump(0x715DB4, UInt32(UIHeight));

	//	SafeWriteBuf(0x106EC38, "\x00\x00\xF0\x44", 4); //1280
	//	SafeWriteBuf(0x106F2DC, "\x00\x00\x87\x44", 4); //960
	//	SafeWriteBuf(0x106E960, "\x00\x00\x00\x00\x00\x00\x9E\x40", 8); //1280
	//	SafeWriteBuf(0x106E7F8, "\x00\x00\x00\x00\x00\xe0\x90\x40", 8); //960
	//	SafeWriteBuf(0x1021790, "\x1D\xC7\x71\x1C\xC7\x71\xFC\x3F", 8); //1.3333
	SafeWriteBuf(0x1078128, "\x1D\xC7\x71\x1C\xC7\x71\xFC\x3F", 8); //1.3333
//	SafeWriteBuf(0x713040, "\xDD\x05\xF8\xE7\x06\x01", 6);

	WriteRelJump(0x713037, UInt32(UIWidth2));
	WriteRelJump(0x713040, UInt32(UIHeight2));
	WriteRelJump(0x7FBBDC, UInt32(UIWidth3));
	WriteRelJump(0x7FBBE5, UInt32(UIHeight3));
	//	`DC 0D 60 E9 06 01`
	//	`D9 05 DC F2 06 01`
	//	SafeWriteBuf(0x11A0190, "\x60\x05\x00\x00", 4); //960


	//	SafeWriteBuf(0x106F070, "\x00\x00\x00\x00\x00\x60\x78\x40", 8);
		//	SafeWriteBuf(0x1021790, "\x00\x00\x00\x00\x00\x00\xF8\x3F", 8);

		//01189488 0118948C 01189490 01189494 01189498

	//	SafeWriteBuf(0x71302B, "\x66\x0F\x1F\x44\x00\x00", 6);
	//	SafeWriteBuf(0x713031, "\x66\x0F\x1F\x44\x00\x00", 6);
	//	SafeWriteBuf(0x713037, "\xD9\x05\x38\xEC\x06\x01", 6);
		// sub_E68A80 Tile_A04640
}

void patchAddyCMToSettingsMenu()
{

	WriteRelCall(0x7CC9D4, UInt32(AddyCMToSettingsMenu));
	WriteRelJump(0x7CCA43, 0x7CCAAD);

	//	SafeWriteBuf(0x7CBF77

	//	SafeWriteBuf(0x7CBF8C, "\x66\x0F\x1F\x44\x00\x00", 6);//
	//	SafeWriteBuf(0x7CB674, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB686, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB698, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6AA, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6BC, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6CE, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CB6E0, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CBE50, "\x81\x7A\x08\x50\x05\x7D\x00", 7);

	/*	SafeWriteBuf(0x7CC044, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC392, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC23E, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC0C3, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC044, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC416, "\x0F\x1F\x44\x00\x00", 5);
		SafeWriteBuf(0x7CC582, "\x0F\x1F\x44\x00\x00", 5);*/
	//	SafeWriteBuf(0x7CBF59, "\x0F\x1F\x44\x00\x00", 5);
	//	SafeWriteBuf(0x7CC01F, "\x0F\x1F\x44\x00\x00", 5);

}