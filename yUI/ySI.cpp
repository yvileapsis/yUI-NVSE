#include <ySI.h>
#include <functions.h>
#include <filesystem>
#include <file.h>

extern int g_ySI;
extern int g_ySI_Sort;
extern int g_ySI_Categories;
extern std::unordered_map <TESForm*, std::string> g_SI_Items;
extern std::unordered_map <std::string, JSONEntryTag> g_SI_Tags;
extern std::vector <std::filesystem::path> g_XMLPaths;
extern std::unordered_set <std::string> g_SI_Categories;

extern TileMenu* g_InventoryMenu;

void InjectIconTile(std::string tag, MenuItemEntryList* list, Tile* tile, ContChangesEntry* entry);

void InjectTemplates()
{
	for (auto &iter : g_XMLPaths)
	{
		g_HUDMainMenu->InjectUIXML(iter.generic_string().c_str());
		g_RepairMenu->InjectUIXML(iter.generic_string().c_str());
		g_InventoryMenu->InjectUIXML(iter.generic_string().c_str());
	}
}

extern bool* g_menuVisibility;
std::string stringStewie;

ContChangesEntry* firstEntry = nullptr;

void InjectIconTileLastFix()
{
	if (!firstEntry || !firstEntry->type) return;
	if (RefreshItemsListForm(firstEntry->type))	firstEntry = nullptr;
}

void InjectIconTile(const std::string tag, MenuItemEntryList* list, Tile* tile, ContChangesEntry* entry)
{
	//	if (g_SI_Items.find(entry->type) == g_SI_Items.end()) return;
	if (g_SI_Tags[tag].filename.empty()) return;

	Tile* tilemenu = tile;

	do if IS_TYPE(tilemenu, TileMenu) break;
	while ((tilemenu = tilemenu->parent));

	auto* menu = DYNAMIC_CAST(tilemenu, Tile, TileMenu);

	if (!menu || !menu->menu) return;

	Tile* text = tile->GetChild("ListItemText");

	if (!text) return;
	
	if (!menu->menu->GetTemplateExists(g_SI_Tags[tag].xmltemplate.c_str()))
	{
		if (menu == TileMenu::GetTileMenu(kMenuType_Barter) || menu == TileMenu::GetTileMenu(kMenuType_Container) ||
			menu == TileMenu::GetTileMenu(kMenuType_RepairServices))
			for (auto& iter : g_XMLPaths) menu->InjectUIXML(iter.generic_string().c_str());
		firstEntry = entry;
		if (!menu->menu->GetTemplateExists(g_SI_Tags[tag].xmltemplate.c_str())) return;
	}
	
	Tile* icon = menu->menu->AddTileFromTemplate(text, g_SI_Tags[tag].xmltemplate.c_str(), 0);

	if (!icon) return;

	if (!g_SI_Tags[tag].filename.empty()) icon->SetString(kTileValue_filename, g_SI_Tags[tag].filename.c_str(), false);
	if (!g_SI_Tags[tag].texatlas.empty()) icon->SetString(kTileValue_texatlas, g_SI_Tags[tag].texatlas.c_str(), false);
	if (!g_SI_Tags[tag].systemcolor) {
		icon->SetFloat(kTileValue_systemcolor, menu->GetValueFloat(kTileValue_systemcolor));
	}
	else {
		icon->SetFloat(kTileValue_systemcolor, g_SI_Tags[tag].systemcolor, false);
	}
	//	icon->SetFloat(kTileValue_alpha, 255, propagate);

	float x = text->GetValueFloat(kTileValue_x);

	if (icon->GetValue(kTileValue_user0)) x += icon->GetValueFloat(kTileValue_user0);

	icon->SetFloat(kTileValue_x, x, false);

	x += icon->GetValueFloat(kTileValue_width);

	if (icon->GetValue(kTileValue_user1)) x += icon->GetValueFloat(kTileValue_user1);

	const float wrapwidth = text->GetValueFloat(kTileValue_wrapwidth) - x;

	text->SetFloat(kTileValue_x, x, false);
	text->SetFloat(kTileValue_wrapwidth, wrapwidth, false);

}

void __fastcall SetStringValueInjectTile(Tile* tile, ContChangesEntry* entry, MenuItemEntryList* list, enum TileValues tilevalue, char* src, char propagate)
{
	tile->SetString(tilevalue, src, propagate);
	if (!entry || !entry->type) return;
	InjectIconTile(g_SI_Items[entry->type], list, tile, entry);
}

signed int __fastcall CompareItemsWithTags(ContChangesEntry* a2, ContChangesEntry* a1, Tile* tile1, Tile* tile2)
{
	TESForm* form1 = nullptr, * form2 = nullptr;

	if (a1 && a1->type) form1 = a1->type;
	if (a2 && a2->type) form2 = a2->type;

	signed int cmp;

	if (g_ySI_Sort && !g_SI_Items.empty())
	{	
		std::string tag1, tag2;

		if (form1) tag1 = g_SI_Items[form1];
		if (form2) tag2 = g_SI_Items[form2];

		if (g_ySI_Categories && !g_SI_Categories.empty())
		{
			if (tag1.empty() && tile1 && tile1->GetValue(kTileValue_user16)) tag1 = tile1->GetValue(kTileValue_user16)->str;
			if (tag2.empty() && tile2 && tile2->GetValue(kTileValue_user16)) tag2 = tile2->GetValue(kTileValue_user16)->str;
		}

		if (tag1.empty())
		{
			if (!tag2.empty()) return 1;
		}
		else if (tag2.empty())
			return -1;
		else
		{
			cmp = tag1.compare(tag2);
			if (cmp > 0) return 1;
			if (cmp < 0) return -1;
		}
	}
	std::string name1, name2;
	
	if (form1) name1 = form1->GetTheName();
	if (form2) name2 = form2->GetTheName();

	if (name1.empty() && tile1->GetValue(kTileValue_string)) name1 = tile1->GetValue(kTileValue_string)->str;
	if (name2.empty() && tile2->GetValue(kTileValue_string)) name2 = tile2->GetValue(kTileValue_string)->str;

	cmp = name1.compare(name2);
	if (cmp > 0) return 1;
	if (cmp < 0) return -1;

	if (!form1) return form2 ? -1 : 0;
	if (!form2) return 1;
	
	const SInt16 mods1 = ContWeaponHasAnyMod(a1);
	const SInt16 mods2 = ContWeaponHasAnyMod(a2);
	if (mods1 != mods2) {
		return mods1 > mods2 ? -1 : 1;
	}

	const float condition1 = ContGetHealthPercent(a1);
	const float condition2 = ContGetHealthPercent(a2);
	if (condition1 != condition2) {
		return condition1 > condition2 ? -1 : 1;
	}

	const bool equipped1 = ContGetEquipped(a1);
	const bool equipped2 = ContGetEquipped(a2);
	if (equipped1 != equipped2) {
		return equipped1 > equipped2 ? -1 : 1;
	}

	const UInt32 refID1 = form1->refID;
	const UInt32 refID2 = form2->refID;
	if (refID1 != refID2) {
		return refID1 > refID2 ? -1 : 1;
	}
	
	return 0;
}

void __fastcall SetStringValueTagImage(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
{
	if (!tile) return;
	
	if (Tile* icon = tile->GetChild("HK_Icon"); icon) {
		icon->SetFloat(kTileValue_width, tile->GetValueFloat(kTileValue_width) - 16, propagate);
		icon->SetFloat(kTileValue_height, tile->GetValueFloat(kTileValue_height) - 16, propagate);
		icon->SetFloat(kTileValue_x, 8, propagate);
		icon->SetFloat(kTileValue_y, 8, propagate);
	}

	if (!entry->type || g_SI_Items.find(entry->type) == g_SI_Items.end()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	const std::string tag = g_SI_Items[entry->type];

	if (g_SI_Tags[tag].filename.empty()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	tile->SetString(tilevalue, g_SI_Tags[tag].filename.c_str(), propagate);
}

float compassRoseX = 0, compassRoseY = 0;

void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
{
	if (!tile) return;
	
	if (compassRoseX == 0) compassRoseX = tile->GetValueFloat(kTileValue_x);
	if (compassRoseY == 0) compassRoseY = tile->GetValueFloat(kTileValue_y);

	tile->SetFloat(kTileValue_width, 44, propagate);
	tile->SetFloat(kTileValue_height, 44, propagate);
	tile->SetFloat(kTileValue_x, compassRoseX + 3, propagate);
	tile->SetFloat(kTileValue_y, compassRoseY + 3, propagate);

	if (!entry->type || g_SI_Items.find(entry->type) == g_SI_Items.end()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	const std::string tag = g_SI_Items[entry->type];

	if (g_SI_Tags[tag].filename.empty()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	tile->SetString(tilevalue, g_SI_Tags[tag].filename.c_str(), propagate);
}

std::string openCategory;

bool __fastcall HasContainerChangesEntry(ContChangesEntry* entry)
{
	if (entry && entry->type) return false;
	return true;
}

bool __fastcall KeyringShowCategories(Tile* tile)
{
	std::string tag;
	if (tile->GetValue(kTileValue_user16)) tag = tile->GetValue(kTileValue_user16)->str;
	if (g_SI_Tags[tag].tab == InventoryMenu::GetSingleton()->filter) return false;
	return true;
}

bool __fastcall KeyringHideKeys(ContChangesEntry* entry)
{
	if (!entry || !entry->type) return false;
	if (!g_SI_Tags[g_SI_Items[entry->type]].category.empty()) return true;
	return false;
}

bool __cdecl KeyringHideNonKeys(ContChangesEntry* entry)
{
	if (!entry || !entry->type) return true;
	if (g_SI_Tags[g_SI_Items[entry->type]].category._Equal(openCategory)) {
		if (stringStewie.empty() || stringStewie._Equal("_")) return false;
		return !stristr(entry->type->GetTheName(), stringStewie.c_str());
	}
	return true;
}

void __fastcall AddSortingCategories()
{
	const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
	for (auto& entry : g_SI_Categories)
	{
		UInt32 keys = 0;
		if (!entryDataList) return;
		for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			if (iter->data && iter->data->type && g_SI_Tags[g_SI_Items[iter->data->type]].category._Equal(entry)) keys++;
		if (keys) {
			//	std::string keyringname = StrFromINI(reinterpret_cast<DWORD*>(0x011D3B20));
			std::string keyringname = g_SI_Tags[entry].name;
			if (keyringname.find("&-") == 0)
			{
				keyringname = keyringname.substr(2, keyringname.length() - 3);
				keyringname = GetStringFromGameSettingFromString(keyringname);
			}
			; //g_SI_Tags[entry].name;
			if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";
			auto tile = InventoryMenu::GetSingleton()->itemsList.Insert(nullptr, keyringname.c_str(), nullptr, nullptr);
			tile->SetFloat(kTileValue_id, 30);
			tile->SetString(kTileValue_user16, entry.c_str());
			InjectIconTile(entry, &InventoryMenu::GetSingleton()->itemsList, tile, nullptr);
		}
	}
}

void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile)
{
	if (tile && tile->GetValue(kTileValue_user16)) openCategory = tile->GetValue(kTileValue_user16)->str;
	invmenu->itemsList.Filter(KeyringHideNonKeys);
	invmenu->itemsList.ForEach((void(__cdecl*)(Tile*, ContChangesEntry*))0x780C00);
	invmenu->ResetInventorySelectionAndHideDataTile();
}

void KeyringRefreshPostStewie()
{
	if (CdeclCall<bool>(0x702360) && g_menuVisibility[kMenuType_Inventory] && InventoryMenu::GetSingleton()->IsKeyringOpen())
	{
		if (Tile* stew = InventoryMenu::GetSingleton()->tile->GetChild("IM_SearchBar"); stew)
		{
			if (const auto string = stew->GetValue(kTileValue_string)->str; !stringStewie._Equal(string))
			{
				stringStewie = string;
				InventoryMenu::GetSingleton()->itemsList.Filter(KeyringHideNonKeys);
				InventoryMenu::GetSingleton()->itemsList.ForEach((void(__cdecl*)(Tile*, ContChangesEntry*))0x780C00);
			}
		}
		else stringStewie.clear();
	}
}

__declspec(naked) void IconInjectTileSetStringValueHook() {
	static const auto SetStringValue = reinterpret_cast<UInt32>(SetStringValueInjectTile);
	static const UInt32 retnAddr = 0x71A3DA;
	__asm
	{
		mov     edx, [ebp - 0x2C]
		push	edx
		mov		edx, [ebp + 0x8]
		call    SetStringValue
		jmp		retnAddr
	}
}
__declspec(naked) void IconHotkeyHUDTileSetStringValueHook() {
	static const auto SetStringValue = reinterpret_cast<UInt32>(SetStringValueTagImage);
	static const UInt32 retnAddr = 0x7018A3;
	__asm
	{
		mov		edx, [ebp - 0x24]
		call    SetStringValue
		jmp		retnAddr
	}
}

__declspec(naked) void IconHotkeyPipBoyTileSetStringValueHook() {
	static const auto SetStringValue = reinterpret_cast<UInt32>(SetStringValueTagRose);
	static const UInt32 retnAddr = 0x7814FF;
	static const UInt32 g_inventoryMenuSelection = 0x011D9EA8;
	__asm
	{
		mov edx, dword ptr ds : [0x011D9EA8]
		call    SetStringValue
		jmp		retnAddr
	}
}

__declspec(naked) void SortingInventoryMenuHook()
{
	static const UInt32 CompareItems = reinterpret_cast<UInt32>(CompareItemsWithTags);
	static const UInt32 ContChangesEntry_GetFullName = 0x4BE2D0;
	static const UInt32 retnAddr = 0x78251B;

	_asm
	{
		mov eax, [ebp + 0xC]	// a2
		mov ecx, [eax]
		push ecx				// tile2
		mov ecx, [eax + 0x4]	// entry2
		
		mov eax, [ebp + 0x8]	// a1
		mov edx, [eax]
		push edx				// tile1
		mov edx, [eax + 0x4]	// entry1

		call CompareItems
		test eax, eax
		je got0
		
		mov esp, ebp
		pop ebp
		ret

	got0:
		mov edx, [ebp + 0xC]
		mov ecx, [edx + 0x4]
		call ContChangesEntry_GetFullName
		jmp retnAddr
	}
}

__declspec(naked) void SortingBarterContainerMenuHook()
{
	static const auto CompareItems = reinterpret_cast<UInt32>(CompareItemsWithTags);
	_asm
	{
		pop eax
		mov eax, [ebp + 0xC]	// a2
		mov ecx, [eax]
		push ecx				// tile2
		mov ecx, [eax + 0x4]	// entry2
		
		mov eax, [ebp + 0x8]	// a1
		mov edx, [eax]
		push edx				// tile1
		mov edx, [eax + 0x4]	// entry1

		call CompareItems
		mov esp, ebp
		pop ebp
		ret
	}
}


__declspec(naked) void KeyringHideKeysHook()
{
	static const UInt32 retnAddr1 = 0x7826EA;
	static const UInt32 retnAddr2 = 0x7826F1;
	static const UInt32 ShouldHide = reinterpret_cast<UInt32>(KeyringHideKeys);
	__asm
	{
		mov ecx, [ebp + 0x8] // a1
		call ShouldHide
		test al, al
		jz shouldnot
		jmp retnAddr1
	shouldnot :
		jmp retnAddr2
	}
}

__declspec(naked) void KeyringHideKeysShowCategoriesHook()
{
	static const UInt32 retnAddr = 0x782679;
	static const UInt32 HasContainerEntry = reinterpret_cast<UInt32>(HasContainerChangesEntry);
	static const UInt32 ShowCategories = reinterpret_cast<UInt32>(KeyringShowCategories);
	__asm
	{
		mov ecx, [ebp + 0x8] // a1
		call HasContainerEntry
		test al, al
		jz hasnot

		mov ecx, [ebp + 0xC]
		call ShowCategories
		mov esp, ebp
		pop ebp
		ret
	hasnot :
		jmp retnAddr
	}
}

__declspec(naked) void KeyringAddCategoriesHook()
{
	static const UInt32 retnAddr = 0x783213;
	static const UInt32 AddCategories = reinterpret_cast<UInt32>(AddSortingCategories);
	__asm
	{
		call AddCategories
		jmp retnAddr
	}
}

__declspec(naked) void KeyringHideNonKeysHook()
{
	static const UInt32 retnAddr = 0x78083F;
	static const UInt32 HideNonKeysAndGetTile = reinterpret_cast<UInt32>(HideNonKeysGetTile);
	__asm
	{
		mov edx, [ebp + 0xC]
		call HideNonKeysAndGetTile
		jmp retnAddr
	}
}

__declspec(naked) void ContainerEntryListBoxFilterHook()
{
	// STEWIE STEWIE STEWIE
	static const UInt32 retnAddr = 0x730C8F;
	__asm
	{
		mov ecx, [ecx] // ListBox::ListItem*

		push dword ptr ds : [ecx] // ListItem->tile
		push dword ptr ds : [ecx + 4] // ListItem->object
		call dword ptr ss : [ebp + 8] // shouldHide
		pop ecx // pop the extra pushed arg (ListItem->tile)

		jmp retnAddr
	}
}

__declspec(naked) void KeyringEnableEquipHook()
{
	static const UInt32 retnAddr = 0x78047D;
//	static const UInt32 HideNonKeysAndGetTile = reinterpret_cast<UInt32>(HideNonKeysGetTile);
	__asm
	{
		mov eax, 0
		jmp retnAddr
	}
}

__declspec(naked) void KeyringEnableDropHook()
{
	static const UInt32 retnAddr = 0x78093A;
	//	static const UInt32 HideNonKeysAndGetTile = reinterpret_cast<UInt32>(HideNonKeysGetTile);
	__asm
	{
		mov ecx, 0
		jmp retnAddr
	}
}

void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, signed int a1)
{
	tile->SetFloat(tilevalue, InventoryMenu::GetSingleton()->IsKeyringOpen(), true);
}

void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char * string, int propagate)
{
	std::string stringnew = string;
	if (auto clickedtile = InventoryMenu::GetSingleton()->itemsList.selected; clickedtile->GetValue(kTileValue_user16)
		&& !g_SI_Tags[clickedtile->GetValue(kTileValue_user16)->str].icon.empty()) 
		stringnew = g_SI_Tags[clickedtile->GetValue(kTileValue_user16)->str].icon;
	tile->SetString(tilevalue, stringnew.c_str(), true);
}