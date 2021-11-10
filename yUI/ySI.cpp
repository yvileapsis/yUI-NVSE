#include "ySI.h"

#include "functions.h"

extern int g_ySI;
extern int g_ySI_Sort;

extern std::unordered_map <TESForm*, std::string> g_SI_Items;
extern std::unordered_map <std::string, JSONEntryTag> g_SI_Tags;

extern TileMenu* g_InventoryMenu;

void InjectTemplates()
{
	g_HUDMainMenu->InjectUIXML("data/menus/ySI/ySI.xml");
	g_StartMenu->InjectUIXML("data/menus/ySI/ySI.xml");
	g_RepairMenu->InjectUIXML("data/menus/ySI/ySI.xml");
	g_InventoryMenu->InjectUIXML("data/menus/ySI/ySI.xml");
}

void __fastcall SetStringValueInjectTile(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
{
	tile->SetString(tilevalue, src, propagate);

	if (!entry || !entry->type) return;

	//	if (g_SI_Items.find(entry->type) == g_SI_Items.end()) return;

	const std::string tag = g_SI_Items[entry->type];

	Tile* tilemenu = tile;

	do { if IS_TYPE(tilemenu, TileMenu) break;
	} while ((tilemenu = tilemenu->parent));

	if (!&g_SI_Tags[tag]) { return; }

	auto* menu = DYNAMIC_CAST(tilemenu, Tile, TileMenu);

	if (!menu || !menu->menu) return;
	
	if (!menu->menu->GetTemplateExists(g_SI_Tags[tag].xmltemplate.c_str()))
	{
		if (menu == TileMenu::GetTileMenu(kMenuType_Barter)) menu->InjectUIXML("data/menus/ySI/ySI.xml");
		if (menu == TileMenu::GetTileMenu(kMenuType_Container)) menu->InjectUIXML("data/menus/ySI/ySI.xml");
		if (menu == TileMenu::GetTileMenu(kMenuType_RepairServices)) menu->InjectUIXML("data/menus/ySI/ySI.xml");
	}

	if (!menu->menu->GetTemplateExists(g_SI_Tags[tag].xmltemplate.c_str())) return;

	Tile* text = tile->GetChild("ListItemText");

	if (!text) return;

	Tile* icon = menu->menu->AddTileFromTemplate(text, g_SI_Tags[tag].xmltemplate.c_str(), 0);

	if (!icon) return;

	if (!g_SI_Tags[tag].filename.empty()) icon->SetString(kTileValue_filename, g_SI_Tags[tag].filename.c_str(), propagate);
	if (!g_SI_Tags[tag].texatlas.empty()) icon->SetString(kTileValue_texatlas, g_SI_Tags[tag].texatlas.c_str(), propagate);
	if (!g_SI_Tags[tag].systemcolor) {
		icon->SetFloat(kTileValue_systemcolor, menu->GetValueFloat(kTileValue_systemcolor));
	} else {
		icon->SetFloat(kTileValue_systemcolor, g_SI_Tags[tag].systemcolor, propagate);
	}
	
	float x = text->GetValueFloat(kTileValue_x);

	if (icon->GetValue(kTileValue_user0)) x += icon->GetValueFloat(kTileValue_user0);

	icon->SetFloat(kTileValue_x, x, propagate);

	x += icon->GetValueFloat(kTileValue_width);

	if (icon->GetValue(kTileValue_user1)) x += icon->GetValueFloat(kTileValue_user1);

	const float wrapwidth = text->GetValueFloat(kTileValue_wrapwidth) - x;

	text->SetFloat(kTileValue_x, x, propagate);
	text->SetFloat(kTileValue_wrapwidth, wrapwidth, propagate);
}


__declspec(naked) void TileSetStringValueInjectIconHook() {
	static const auto SetStringValue = reinterpret_cast<UInt32>(SetStringValueInjectTile);
	static const UInt32 retnAddr = 0x71A3DA;
	__asm
	{
		mov		edx, [ebp + 0x8]
		call    SetStringValue
		jmp		retnAddr
	}
}

signed int __fastcall CompareItemsWithTags(ContChangesEntry* a2, ContChangesEntry* a1)
{
	TESForm* form1 = a1->type, * form2 = a2->type;

	if (!form1) { if (!form2) return 0; else return -1; }
	if (!form2) { return 1; }

	signed int cmp;

	if (g_ySI_Sort) {
		if (g_SI_Items[form1].empty()) {
			if (!g_SI_Items[form2].empty()) { return 1; }
		}
		else if (g_SI_Items[form2].empty()) {
			return -1;
		}
		else {
			cmp = g_SI_Items[form1].compare(g_SI_Items[form2]);
			if (cmp > 0) return 1;
			if (cmp < 0) return -1;
		}
	}
	
	cmp = std::string(form1->GetTheName()).compare(std::string(form2->GetTheName()));
	if (cmp > 0) return 1;
	if (cmp < 0) return -1;

	SInt16 mods1 = -1, mods2 = -1;
	mods1 = ContWeaponHasAnyMod(a1); mods2 = ContWeaponHasAnyMod(a2);
	if (mods1 != mods2) {
		return mods1 > mods2 ? -1 : 1;
	}

	float condition1 = -1, condition2 = -1;
	condition1 = ContGetHealthPercent(a1); condition2 = ContGetHealthPercent(a2);
	if (condition1 != condition2) {
		return condition1 > condition2 ? -1 : 1;
	}

	bool equipped1 = false, equipped2 = false;
	equipped1 = ContGetEquipped(a1); equipped2 = ContGetEquipped(a2);
	if (equipped1 != equipped2) {
		return equipped1 > equipped2 ? -1 : 1;
	}

	UInt32 refID1 = 0, refID2 = 0;
	refID1 = form1->refID; refID2 = form2->refID;
	if (refID1 != refID2) {
		return refID1 > refID2 ? -1 : 1;
	}
	
	return 0;
}

void __fastcall SetStringValueTagImage(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
{
	Tile* icon = tile->GetChild("HK_Icon");

	if (icon) {
		icon->SetFloat(kTileValue_width, tile->GetValueFloat(kTileValue_width) - 12, propagate);
		icon->SetFloat(kTileValue_height, tile->GetValueFloat(kTileValue_height) - 12, propagate);
		icon->SetFloat(kTileValue_x, 6, propagate);
		icon->SetFloat(kTileValue_y, 6, propagate);
	}

	if (!entry->type || g_SI_Items.find(entry->type) == g_SI_Items.end()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	std::string tag = g_SI_Items[entry->type];

	if (!&g_SI_Tags[tag]) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	tile->SetString(tilevalue, g_SI_Tags[tag].filename.c_str(), propagate);
}

float compassRoseX, compassRoseY;

void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
{
	if (compassRoseX == 0) compassRoseX = tile->GetValueFloat(kTileValue_x);
	if (compassRoseY == 0) compassRoseY = tile->GetValueFloat(kTileValue_y);

	tile->SetFloat(kTileValue_width, 48, propagate);
	tile->SetFloat(kTileValue_height, 48, propagate);
	tile->SetFloat(kTileValue_x, compassRoseX + 1, propagate);
	tile->SetFloat(kTileValue_y, compassRoseY + 1, propagate);

	if (!entry->type || g_SI_Items.find(entry->type) == g_SI_Items.end()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	std::string tag = g_SI_Items[entry->type];

	if (!&g_SI_Tags[tag]) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	tile->SetString(tilevalue, g_SI_Tags[tag].filename.c_str(), propagate);
}

__declspec(naked) void TileSetStringValueHotkeyHook() {
	static const auto SetStringValue = reinterpret_cast<UInt32>(SetStringValueTagImage);
	static const UInt32 retnAddr = 0x7018A3;
	__asm
	{
		mov		edx, [ebp - 0x24]
		call    SetStringValue
		jmp		retnAddr
	}
}

__declspec(naked) void TileSetStringValueHotkeyHook2() {
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

__declspec(naked) void InventoryMenuSortingHook()
{
	static const auto CompareItems = reinterpret_cast<UInt32>(CompareItemsWithTags);
	_asm
	{
		mov eax, [ebp + 0x8] // a1
		mov edx, [eax + 4]
		call CompareItems
		mov esp, ebp
		pop ebp
		ret
	}
}

__declspec(naked) void BarterContainerMenuSortingHook()
{
	static const auto CompareItems = reinterpret_cast<UInt32>(CompareItemsWithTags);
	_asm
	{
		pop eax
		mov eax, [ebp + 0x8] // a1
		mov edx, [eax + 4]
		mov eax, [ebp + 0xC] // a2
		mov ecx, [eax + 4]
		call CompareItems
		mov esp, ebp
		pop ebp
		ret
	}
}

/*
__declspec(naked) void PostJazzHookInventory()
{
	static const UInt32 SetStringValue = UInt32(Tile__SetStringValueRemoveTag);
	static const UInt32 GetValueAsText = 0xA011F0;
	static const UInt32 GetLast = 0x7B5370;

	static const UInt32 retnAddr = 0x782902;

	__asm
	{

		push    1
		push    0x0FC4
		mov     ecx, [ebp + 0x8]
		call    GetValueAsText
		push    eax
		push    0x0FC4
		mov     ecx, [ebp + 0x8]
		add     ecx, 4
		call    GetLast
		mov     ecx, [eax]
		call    SetStringValue

		push	0x1D
		push	0x0FAA
		mov     ecx, [ebp + 8]
		jmp		retnAddr
	}
}


__declspec(naked) void PostJazzHookBarter()
{
	static const UInt32 SetStringValue = UInt32(Tile__SetStringValueRemoveTag);
	static const UInt32 GetValueAsText = 0xA011F0;
	static const UInt32 GetLast = 0x7B5370;

	static const UInt32 retnAddr = 0x72F384;

	__asm
	{
		push    1
		push    0x0FC4
		mov     ecx, [ebp + 0x8]
		call    GetValueAsText
		push    eax
		push    0x0FC4
		mov     ecx, [ebp + 0x8]
		add     ecx, 4
		call    GetLast
		mov     ecx, [eax]
		call    SetStringValue

		push	0
		mov		ecx, [ebp + 0xC]
		jmp		retnAddr
	}
}*/
/*
void __fastcall Tile__SetStringValueNop(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* src, char propagate)
{
	ConsoleQueueOrPrint(src, strlen(src));
}*/
/*
void __fastcall PostJazzHookBarterHandle(TileMenu* tile)
{
	char* string = ThisStdCall<char*>(0xA011F0, tile, kTileValue_string);
	Tile* child = ThisStdCall<Tile*>(0x7B5370, tile + 0x4);
	Tile__SetStringValueAlt(child, 0, kTileValue_string, string, 1);
}


__declspec(naked) void PostJazzHookBarterAlt()
{
	static const UInt32 SetStringValue = UInt32(PostJazzHookBarterHandle);

	static const UInt32 retnAddr = 0x72F384;

	__asm
	{
		mov     ecx, [ebp + 0x8]
		call    PostJazzHookBarterHandle

		push	0
		mov		ecx, [ebp + 0xC]
		jmp		retnAddr
	}
}


void __fastcall Tile__SetStringValueRemoveTag(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* src, char propagate)
{
	for (auto& it = map_ySI_Icon.rbegin(); it != map_ySI_Icon.rend(); it++) {
		if (std::string(src).rfind(it->first, 0) == 0) {
			if (it->first.empty()) { break; }
			tile->SetString(tilevalue, src + 1 + it->first.length(), propagate);
			return;
		}
	};
	tile->SetString(tilevalue, src, propagate);
}

int __cdecl strcpy_s_Alt(char* Dst, rsize_t SizeInBytes, char* src)
{
	for (auto& it = map_ySI_Icon.rbegin(); it != map_ySI_Icon.rend(); it++) {
		if (std::string(src).rfind(it->first, 0) == 0) {
			if (it->first.empty()) { break; }
			return strcpy_s_(Dst, SizeInBytes, src + 1 + it->first.length());
		}
	};
	return strcpy_s_(Dst, SizeInBytes, src);
}

*/