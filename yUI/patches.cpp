#include <patches.h>
#include <SafeWrite.h>
#include <ySI.h>
#include <functions.h>

void patchSortingHooks()
{
	WriteRelJump(0x782516, SI_Hooks::SortingInventoryMenuHook<0x78251B>);
	WriteRelJump(0x72F0ED, SI_Hooks::SortingBarterContainerMenuHook);
	WriteRelJump(0x75D138, SI_Hooks::SortingBarterContainerMenuHook);
}

void patchAddIcons()
{
	WriteRelJump(0x71A3D5, SI_Hooks::IconInjectTileSetStringValueHook<0x71A3DA>);
}

void patchReplaceHotkeyIcons()
{
	WriteRelJump(0x70189E, SI_Hooks::IconHotkeyHUDTileSetStringValueHook<0x7018A3>);
	WriteRelJump(0x7814FA, SI_Hooks::IconHotkeyPipBoyTileSetStringValueHook<0x7814FF>);
}

void patchSortingCategories()
{
	WriteRelJump(0x730C81, SI_Hooks::ContainerEntryListBoxFilterHookPre<0x730C8C>);
	WriteRelJump(0x730C97, SI_Hooks::ContainerEntryListBoxFilterHookPost<0x730CA9>);

	WriteRelJump(0x7824F6, SI_Hooks::SortingInventoryMenuHook<0x78251B>);
	WriteRelJump(0x78250B, SI_Hooks::SortingInventoryMenuHook<0x78251B>);

	WriteRelJump(0x7831C1, SI_Hooks::KeyringAddCategoriesHook<0x783213>);
	
	WriteRelJump(0x7826E4, SI_Hooks::KeyringHideKeysHook<0x7826EA, 0x7826F1>);
	WriteRelJump(0x78083A, SI_Hooks::KeyringHideNonKeysHook<0x78083F>);

	WriteRelJump(0x782665, SI_Hooks::KeyringHideKeysShowCategoriesHook<0x782679>);

	WriteRelJump(0x780478, SI_Hooks::KeyringEnableEquipDropHook<0x78047D>);
	WriteRelJump(0x780934, SI_Hooks::KeyringEnableEquipDropHook<0x78093A>);

	WriteRelCall(0x782F42, SI_Hooks::KeyringEnableCancelHook);

	WriteRelCall(0x7815A6, SI_Hooks::KeyringPipBoyIconHook);
}

//bool __fastcall FFFNOP()
//{
//	return false;
//}
///	WriteRelCall(0x780488, reinterpret_cast<UInt32>(FFFNOP));	   
void patchFixDroppedItems()
{
	WriteRelCall(0x75C793, FixGetDroppedWeaponPre);
	WriteRelJump(0x75C9AB, FixGetDroppedWeaponPost<0x75C798>);
}

void patchMatchedCursor()
{
	WriteRelCall(0x70B33A, CursorTileSetStringValue);
	WriteRelCall(0x70C727, CursorTileSetIntValue);
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