#include "ySI.h"

#include "functions.h"

void ySIFillMapsAlt()
{
	std::string tag;
	for (DListNode<Tile>* node = g_HUDMainMenu->children.Head(); node; node = node->next) {
		if (node->data) {
			if (!strncmp(node->data->name.m_data, "ySI_Icon", 8) && node->data->GetValue(kTileValue_class)) {
				(node->data->GetValue(kTileValue_string)) ? tag = node->data->GetValue(kTileValue_string)->str : tag = "";
				map_ySI_Icon.emplace(tag, node->data);
				//				map_ySI_Icon[tag] = node->data;
			}
			else if (!strncmp(node->data->name.m_data, "ySI_Tag", 7) && node->data->GetValue(kTileValue_string) && node->data->GetValue(kTileValue_user0)) {
				map_ySI_Tag.emplace(node->data);
			}
			else if (!strncmp(node->data->name.m_data, "ySI_List", 8) && node->data->GetValue(kTileValue_string) && node->data->GetValue(kTileValue_user0)) {
				map_ySI_List.emplace(node->data);
			}

		}
	}

	tag = "";
	Tile* tile;

	for (auto mIter = (*g_allFormsMap)->Begin(); mIter; ++mIter) {
		TESForm* form = mIter.Get();

		for (auto it : map_ySI_List) {
			tile = it;
			if (tile->GetValue(kTileValue_user0)->num != form->typeID) continue;
			TESForm* list = GetRefFromString(tile->GetValue(kTileValue_user1)->str, tile->GetValue(kTileValue_user2)->str);
			UInt8 listtype = 1;
			if (tile->GetValue(kTileValue_user3)) {
				listtype = strtoul(tile->GetValue(kTileValue_user3)->str, NULL, 0);
			}
			if ((listtype & 0x1) && list && IsInListRecursive(form, (BGSListForm*)list)) {
				tag = std::string(tile->GetValue(kTileValue_string)->str);
				break;
			}
			if ((listtype & 0x2) && list && IsInRepairListRecursive(form, (BGSListForm*)list)) {
				tag = std::string(tile->GetValue(kTileValue_string)->str);
				break;
			}
			if ((listtype & 0x4) && list && IsInAmmoListRecursive(form, (BGSListForm*)list)) {
				tag = std::string(tile->GetValue(kTileValue_string)->str);
				break;
			}
		}
		if (!tag.empty()) {
			map_ySI_Item.emplace(form, tag);
			tag = "";
			continue;
		}

		for (auto it : map_ySI_Tag)
		{
			tile = it;

			if (tile->GetValue(kTileValue_user0)->num != form->typeID) continue;

			if (tile->GetValue(kTileValue_user1)) {
				if (g_dataHandler->GetModIndex(tile->GetValue(kTileValue_user1)->str) != form->modIndex) continue;
			}
			if (tile->GetValue(kTileValue_user2)) {
				if (strtoul(tile->GetValue(kTileValue_user2)->str, NULL, 0) != (form->refID & 0xFFFFFF)) continue;
			}
			if (tile->GetValue(kTileValue_user3)) {
				if (tile->GetValue(kTileValue_user3)->num != form->IsQuestItem()) continue;
			}

			switch (form->typeID) {
			case 24: {
				if (tile->GetValue(kTileValue_user4)) {
					UInt32 slotMask = strtoul(tile->GetValue(kTileValue_user4)->str, NULL, 0);
					if ((slotMask & GetArmorValue(form, 6)) != slotMask) continue;
				}
				if (tile->GetValue(kTileValue_user5)) {
					UInt32 slotMask = strtoul(tile->GetValue(kTileValue_user5)->str, NULL, 0);
					if ((slotMask & GetArmorValue(form, 6)) == 0) continue;
				}
				if (tile->GetValue(kTileValue_user6)) {
					if ((tile->GetValue(kTileValue_user6)->num != GetArmorValue(form, 1))) continue;
				}
				if (tile->GetValue(kTileValue_user7)) {
					if ((tile->GetValue(kTileValue_user7)->num != GetArmorValue(form, 2))) continue;
				}
				if (tile->GetValue(kTileValue_user8)) {
					if ((tile->GetValue(kTileValue_user8)->num != GetArmorValue(form, 3))) continue;
				}
				if (tile->GetValue(kTileValue_user9)) {
					if (!HasBaseEffectChangesAV(form, static_cast<int>(tile->GetValue(kTileValue_user9)->num))) continue;
				}
				if (tile->GetValue(kTileValue_user10)) {
					if ((tile->GetValue(kTileValue_user10)->num > GetArmorValue(form, 4))) continue;
				}
				if (tile->GetValue(kTileValue_user11)) {
					if ((tile->GetValue(kTileValue_user11)->num > GetArmorValue(form, 5))) continue;
				}
				break;
			}
			case 31: {
				if (tile->GetValue(kTileValue_user4)) {
					if (IsCraftingComponent(form) != tile->GetValue(kTileValue_user4)->num) continue;
				}
			}
			case 40: {

				if (tile->GetValue(kTileValue_user4) && tile->GetValue(kTileValue_user4)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_Skill)) continue;
				if (tile->GetValue(kTileValue_user5) && tile->GetValue(kTileValue_user5)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_Type)) continue;
				if (tile->GetValue(kTileValue_user6) && tile->GetValue(kTileValue_user6)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_HandGrip)) continue;
				if (tile->GetValue(kTileValue_user7) && tile->GetValue(kTileValue_user7)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_AttackAnim)) continue;
				if (tile->GetValue(kTileValue_user8) && tile->GetValue(kTileValue_user8)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_ReloadAnim)) continue;
				if (tile->GetValue(kTileValue_user9) && tile->GetValue(kTileValue_user9)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_IsAutomatic)) continue;
				if (tile->GetValue(kTileValue_user10) && tile->GetValue(kTileValue_user10)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_HasScope)) continue;
				if (tile->GetValue(kTileValue_user11) && tile->GetValue(kTileValue_user11)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_IgnoresDTDR)) continue;
				if (tile->GetValue(kTileValue_user12)) {
					auto pClipRounds = DYNAMIC_CAST(form, TESForm, BGSClipRoundsForm);
					if (!pClipRounds || (tile->GetValue(kTileValue_user12)->num > (float)pClipRounds->clipRounds)) continue;
				}
				if (tile->GetValue(kTileValue_user13) && tile->GetValue(kTileValue_user13)->num > (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_NumProj)) continue;
				if (tile->GetValue(kTileValue_user14) && tile->GetValue(kTileValue_user14)->num != (DYNAMIC_CAST(form, TESForm, TESObjectWEAP))->GetWeaponValue(TESObjectWEAP::eWeap_SoundLevel)) continue;
				if (tile->GetValue(kTileValue_user15)) {
					auto pAmmoForm = DYNAMIC_CAST(form, TESForm, BGSAmmoForm);
					if (!pAmmoForm || !pAmmoForm->ammo || (g_dataHandler->GetModIndex(tile->GetValue(kTileValue_user15)->str) != pAmmoForm->ammo->modIndex)) continue;
				}
				if (tile->GetValue(kTileValue_user16)) {
					auto pAmmoForm = DYNAMIC_CAST(form, TESForm, BGSAmmoForm);
					if (!pAmmoForm || !pAmmoForm->ammo || (strtoul(tile->GetValue(kTileValue_user16)->str, NULL, 0) != (pAmmoForm->ammo->refID & 0xFFFFFF))) continue;
				}
				break;
			}
			case 47: {
				if (tile->GetValue(kTileValue_user4)) {
					if (!HasBaseEffectRestoresAV(form, static_cast<int>(tile->GetValue(kTileValue_user4)->num))) continue;
				}
				if (tile->GetValue(kTileValue_user5)) {
					if (!HasBaseEffectDamagesAV(form, static_cast<int>(tile->GetValue(kTileValue_user5)->num))) continue;
				}
				if (tile->GetValue(kTileValue_user6)) {
					if (IsAddictive(form) != tile->GetValue(kTileValue_user6)->num) continue;
				}
				if (tile->GetValue(kTileValue_user7)) {
					if (IsFood(form) != tile->GetValue(kTileValue_user7)->num) continue;
				}
				if (tile->GetValue(kTileValue_user8)) {
					if (IsMedicine(form) != tile->GetValue(kTileValue_user8)->num) continue;
				}
				if (tile->GetValue(kTileValue_user9)) {
					if (IsPoisonous(form) != tile->GetValue(kTileValue_user9)->num) continue;
				}
				break;
			}
			}
			tag = std::string(tile->GetValue(kTileValue_string)->str);
			break;
		}
		if (!tag.empty()) {
			map_ySI_Item.emplace(form, tag);
			tag = "";
		}
	}
}

void __fastcall SetStringValueInjectTile(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
{
	tile->SetString(tilevalue, src, propagate);

	if (!entry || !entry->type) return;

	//	if (map_ySI_Item.find(entry->type) == map_ySI_Item.end()) return;

	std::string tag = map_ySI_Item[entry->type];

	Tile* menu = tile;

	do {
		if IS_TYPE(menu, TileMenu)
			break;
	} while ((menu = menu->parent));

	if (!map_ySI_Icon[tag]) { return; }

	if (!dynamic_cast<TileMenu*>(menu)->menu->GetTemplateExists(map_ySI_Icon[tag]->GetValue(kTileValue_class)->str)) { return; }

	Tile* text = tile->GetChild("ListItemText");

	if (!text) return;

	Tile* icon = dynamic_cast<TileMenu*>(menu)->menu->AddTileFromTemplate(text, map_ySI_Icon[tag]->GetValue(kTileValue_class)->str, 0);

	if (!icon) return;

	if (map_ySI_Icon[tag]->GetValue(kTileValue_filename)) icon->SetString(kTileValue_filename, map_ySI_Icon[tag]->GetValue(kTileValue_filename)->str, propagate);
	if (map_ySI_Icon[tag]->GetValue(kTileValue_texatlas)) icon->SetString(kTileValue_texatlas, map_ySI_Icon[tag]->GetValue(kTileValue_texatlas)->str, propagate);
	if (map_ySI_Icon[tag]->GetValue(kTileValue_systemcolor)) icon->SetFloat(kTileValue_systemcolor, map_ySI_Icon[tag]->GetValue(kTileValue_systemcolor)->num, propagate);

	float x = text->GetValueFloat(kTileValue_x);

	if (icon->GetValue(kTileValue_user0)) x += icon->GetValueFloat(kTileValue_user0);

	icon->SetFloat(kTileValue_x, x, propagate);

	x += icon->GetValueFloat(kTileValue_width);

	if (icon->GetValue(kTileValue_user1)) x += icon->GetValueFloat(kTileValue_user1);

	float wrapwidth = text->GetValueFloat(kTileValue_wrapwidth) - x;

	text->SetFloat(kTileValue_x, x, propagate);
	text->SetFloat(kTileValue_wrapwidth, wrapwidth, propagate);
}


__declspec(naked) void Tile__SetStringValueInjectIconHook() {
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

	if (map_ySI_Item[form1].empty()) {
		if (!map_ySI_Item[form2].empty()) { return 1; }
	}
	else if (map_ySI_Item[form2].empty()) {
		return -1;
	}
	else {
		cmp = map_ySI_Item[form1].compare(map_ySI_Item[form2]);
		if (cmp > 0) return 1;
		if (cmp < 0) return -1;
	}

	cmp = std::string(form1->GetTheName()).compare(std::string(form2->GetTheName()));
	if (cmp > 0) return 1;
	if (cmp < 0) return -1;

	SInt16 mods1 = -1, mods2 = -1;
	mods1 = ContWeaponHasAnyMod(a1), mods2 = ContWeaponHasAnyMod(a2);
	if (mods1 != mods2) {
		return mods1 > mods2 ? -1 : 1;
	}

	float condition1 = -1, condition2 = -1;
	condition1 = ContGetHealthPercent(a1), condition2 = ContGetHealthPercent(a2);
	if (condition1 != condition2) {
		return condition1 > condition2 ? -1 : 1;
	}

	bool equipped1 = false, equipped2 = false;
	equipped1 = ContGetEquipped(a1), equipped2 = ContGetEquipped(a2);
	if (equipped1 != equipped2) {
		return equipped1 > equipped2 ? -1 : 1;
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

	if (!entry->type || map_ySI_Item.find(entry->type) == map_ySI_Item.end()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	std::string tag = map_ySI_Item[entry->type];

	if (!map_ySI_Icon[tag]) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	tile->SetString(tilevalue, map_ySI_Icon[tag]->GetValue(kTileValue_filename)->str, propagate);
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

	if (!entry->type || map_ySI_Item.find(entry->type) == map_ySI_Item.end()) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	std::string tag = map_ySI_Item[entry->type];

	if (!map_ySI_Icon[tag]) {
		tile->SetString(tilevalue, src, propagate);
		return;
	}

	tile->SetString(tilevalue, map_ySI_Icon[tag]->GetValue(kTileValue_filename)->str, propagate);
}

__declspec(naked) void Tile__SetStringValueHotkeyHook() {
	static const auto SetStringValue = reinterpret_cast<UInt32>(SetStringValueTagImage);
	static const UInt32 retnAddr = 0x7018A3;
	__asm
	{
		mov		edx, [ebp - 0x24]
		call    SetStringValue
		jmp		retnAddr
	}
}

__declspec(naked) void Tile__SetStringValueHotkeyHook2() {
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