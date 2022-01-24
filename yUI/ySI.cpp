#include <ySI.h>
#include <functions.h>
#include <filesystem>
#include <file.h>

extern int g_ySI;
extern int g_ySI_Sort;
extern int g_ySI_Categories;

extern bool* g_menuVisibility;


namespace SI
{
	extern std::unordered_map <TESForm*, std::string> g_Items;
	extern std::unordered_map <std::string, JSONEntryTag> g_Tags;
	extern std::vector <std::filesystem::path> g_XMLPaths;
	extern std::unordered_set <std::string> g_Categories;

	std::string stringStewie;

	std::string openCategory;

	float compassRoseX = 0, compassRoseY = 0;

	TESForm* firstEntry = nullptr;
}

namespace SI_Files
{
	extern std::vector<JSONEntryItem> g_Items_JSON;
	extern std::vector<JSONEntryTag> g_Tags_JSON;
	
	bool AssignTagToItem(TESForm* form)
	{
		for (const auto& entry : g_Items_JSON) {
			if (entry.form && entry.form->refID != form->refID) continue;
			if (entry.formType && entry.formType != form->typeID) continue;

			if (entry.questItem && entry.questItem != static_cast<UInt8>(form->IsQuestItem2())) continue;

			if (entry.formType == 40) {
				const auto weapon = DYNAMIC_CAST(form, TESForm, TESObjectWEAP);
				if (!weapon) continue;
				if (entry.formWeapon.weaponSkill && entry.formWeapon.weaponSkill != weapon->weaponSkill) continue;
				if (entry.formWeapon.weaponType && entry.formWeapon.weaponType != weapon->eWeaponType) continue;
				if (entry.formWeapon.weaponHandgrip && entry.formWeapon.weaponHandgrip != weapon->HandGrip()) continue;
				if (entry.formWeapon.weaponAttackAnim && entry.formWeapon.weaponAttackAnim != weapon->AttackAnimation()) continue;
				if (entry.formWeapon.weaponReloadAnim && entry.formWeapon.weaponReloadAnim != weapon->reloadAnim) continue;
				if (entry.formWeapon.weaponType && entry.formWeapon.weaponType != weapon->eWeaponType) continue;
				if (entry.formWeapon.weaponIsAutomatic && entry.formWeapon.weaponIsAutomatic != static_cast<UInt32>(weapon->IsAutomatic())) continue;
				if (entry.formWeapon.weaponHasScope && entry.formWeapon.weaponHasScope != static_cast<UInt32>(weapon->HasScopeAlt())) continue;
				if (entry.formWeapon.weaponIgnoresDTDR && entry.formWeapon.weaponIgnoresDTDR != static_cast<UInt32>(weapon->IgnoresDTDR())) continue;
				if (entry.formWeapon.weaponClipRounds && entry.formWeapon.weaponClipRounds > static_cast<UInt32>(weapon->GetClipRounds(false))) continue;
				if (entry.formWeapon.weaponNumProjectiles && entry.formWeapon.weaponNumProjectiles > weapon->numProjectiles) continue;
				if (entry.formWeapon.weaponSoundLevel && entry.formWeapon.weaponSoundLevel != weapon->soundLevel) continue;
				if (entry.formWeapon.ammo && !IsInListRecursive(entry.formWeapon.ammo, weapon->ammo.ammo)) continue;
			}
			else if (entry.formType == 24) {
				const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
				if (!armor) continue;
				if (entry.formArmor.armorSlotsMaskWL && (entry.formArmor.armorSlotsMaskWL & armor->GetArmorValue(6)) != entry.formArmor.armorSlotsMaskWL) continue;
				if (entry.formArmor.armorSlotsMaskWL && (entry.formArmor.armorSlotsMaskBL & armor->GetArmorValue(6)) != 0) continue;
				if (entry.formArmor.armorClass && entry.formArmor.armorClass != armor->GetArmorValue(1)) continue;
				if (entry.formArmor.armorPower && entry.formArmor.armorPower != armor->GetArmorValue(2)) continue;
				if (entry.formArmor.armorHasBackpack && entry.formArmor.armorHasBackpack != armor->GetArmorValue(3)) continue;

				if (entry.formArmor.armorDT && entry.formArmor.armorDT > armor->damageThreshold) continue;
				if (entry.formArmor.armorDR && entry.formArmor.armorDR > armor->armorRating) continue;
				//			if (entry.formArmor.armorChangesAV && entry.formArmor.armorChangesAV > armor->armorRating) continue;
			}
			else if (entry.formType == 31) {
				if (entry.formMisc.miscComponent && !IsCraftingComponent(form)) continue;
			}
			else if (entry.formType == 47) {
				const auto aid = DYNAMIC_CAST(form, TESForm, AlchemyItem);
				if (!aid) continue;
				if (entry.formAid.aidRestoresAV && !aid->HasBaseEffectRestoresAV(entry.formAid.aidRestoresAV)) continue;
				if (entry.formAid.aidDamagesAV && !aid->HasBaseEffectDamagesAV(entry.formAid.aidDamagesAV)) continue;
				if (entry.formAid.aidIsAddictive && !aid->IsAddictive()) continue;
				if (entry.formAid.aidIsFood && !aid->IsFood()) continue;
				if (entry.formAid.aidIsWater && !aid->IsWaterAlt()) continue;
				if (entry.formAid.aidIsPoisonous && !aid->IsPoison()) continue;
				if (entry.formAid.aidIsMedicine && !aid->IsMedicine()) continue;
			}

			SI::g_Items.emplace(form, entry.tag);
			return true;
		}
		return false;
	}

}

namespace SI
{
	void InjectTemplates()
	{
		for (auto& iter : g_XMLPaths)
		{
			g_HUDMainMenu->InjectUIXML(iter.generic_string().c_str());
			g_RepairMenu->InjectUIXML(iter.generic_string().c_str());
			g_InventoryMenu->InjectUIXML(iter.generic_string().c_str());
		}
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

	bool TryGetTypeOfFirstEntry()
	{
		__try {
			switch (*reinterpret_cast<UInt32*>(firstEntry)) {
			case kVtbl_TESObjectARMO:
			case kVtbl_TESObjectBOOK:
			case kVtbl_TESObjectLIGH:
			case kVtbl_TESObjectMISC:
			case kVtbl_TESObjectWEAP:
			case kVtbl_IngredientItem:
			case kVtbl_TESAmmo:
				return true;
			default:
				bool whaa = static_cast<bool>(firstEntry->typeID);
				return whaa;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}
	
	void InjectIconTileLastFix()
	{
		if (!firstEntry) return;
		if (!TryGetTypeOfFirstEntry()) {
			firstEntry = nullptr;
			return;
		}
		if (RefreshItemsListForm(firstEntry)) firstEntry = nullptr;
	}

	bool IsTagForItem(TESForm* form)
	{
		if (form && g_Items.find(form) != g_Items.end()) return true;
		return false;
	}

	bool IsTagForItem(ContChangesEntry* entry)
	{
		if (entry && entry->type && g_Items.find(entry->type) != g_Items.end()) return true;
		return false;
	}
	
	std::string GetTagForItem(TESForm* form)
	{
		if (!form) return "";
		if (!IsTagForItem(form)) SI_Files::AssignTagToItem(form);
		return g_Items[form];
	}

	std::string GetTagForItem(ContChangesEntry* entry)
	{
		if (!entry || !entry->type) return "";
		if (!IsTagForItem(entry)) SI_Files::AssignTagToItem(entry->type);
		return g_Items[entry->type];
	}
	void InjectIconTile(const std::string& tag, MenuItemEntryList* list, Tile* tile, ContChangesEntry* entry)
	{
		//	if (g_Items.find(entry->type) == g_Items.end()) return;
		if (g_Tags[tag].filename.empty()) return;

		Tile* tilemenu = tile;

		do if IS_TYPE(tilemenu, TileMenu) break;
		while ((tilemenu = tilemenu->parent));

		auto* menu = DYNAMIC_CAST(tilemenu, Tile, TileMenu);

		if (!menu || !menu->menu) return;

		auto text = tile->GetChild("ListItemText");

		if (!text) return;

		if (!menu->menu->GetTemplateExists(g_Tags[tag].xmltemplate.c_str()))
		{
			if (menu == TileMenu::GetTileMenu(kMenuType_Barter) || menu == TileMenu::GetTileMenu(kMenuType_Container) ||
				menu == TileMenu::GetTileMenu(kMenuType_RepairServices))
			{
				for (auto& iter : g_XMLPaths) menu->InjectUIXML(iter.generic_string().c_str());
				firstEntry = entry->type;
//				menu->menu->AddTileFromTemplate(text->parent->parent, g_Tags[tag].xmltemplate.c_str(), 0);
			}
			if (!menu->menu->GetTemplateExists(g_Tags[tag].xmltemplate.c_str())) return;
		}

		auto icon = menu->menu->AddTileFromTemplate(text, g_Tags[tag].xmltemplate.c_str(), 0);

		if (!icon) return;

		if (!g_Tags[tag].filename.empty()) icon->SetString(kTileValue_filename, g_Tags[tag].filename.c_str(), false);
		if (!g_Tags[tag].texatlas.empty()) icon->SetString(kTileValue_texatlas, g_Tags[tag].texatlas.c_str(), false);
		if (!g_Tags[tag].systemcolor) {
			icon->SetFloat(kTileValue_systemcolor, menu->GetValueFloat(kTileValue_systemcolor));
		} else {
			icon->SetFloat(kTileValue_systemcolor, g_Tags[tag].systemcolor, false);
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

	void __fastcall ListGetCountInjectTile(Tile* tile, ContChangesEntry* entry, MenuItemEntryList* list, UInt32 tilevalue, char* tileText, bool propagate)
	{
		tile->SetString(tilevalue, tileText, propagate);
		if (entry && entry->type) InjectIconTile(GetTagForItem(entry), list, tile, entry);
	}

	signed int __fastcall CompareItemsWithTags(ContChangesEntry* a2, ContChangesEntry* a1, Tile* tile1, Tile* tile2)
	{
		TESForm* form1 = nullptr, * form2 = nullptr;

		if (a1 && a1->type) form1 = a1->type;
		if (a2 && a2->type) form2 = a2->type;

		signed int cmp;

		if (g_ySI_Sort && !g_Items.empty())
		{
			std::string tag1, tag2;

			if (form1) tag1 = GetTagForItem(a1);
			if (form2) tag2 = GetTagForItem(a2);

			if (g_ySI_Categories && !g_Categories.empty())
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
		if (mods1 != mods2) return mods1 > mods2 ? -1 : 1;

		const float condition1 = ContGetHealthPercent(a1);
		const float condition2 = ContGetHealthPercent(a2);
		if (condition1 != condition2) return condition1 > condition2 ? -1 : 1;

		const bool equipped1 = ContGetEquipped(a1);
		const bool equipped2 = ContGetEquipped(a2);
		if (equipped1 != equipped2) return equipped1 > equipped2 ? -1 : 1;

		const UInt32 refID1 = form1->refID;
		const UInt32 refID2 = form2->refID;
		if (refID1 != refID2) return refID1 > refID2 ? -1 : 1;

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

		if (!IsTagForItem(entry)) {
			tile->SetString(tilevalue, src, propagate);
			return;
		}

		const std::string tag = GetTagForItem(entry);

		tile->SetString(tilevalue, g_Tags[tag].filename.empty() ? src : g_Tags[tag].filename.c_str(), propagate);
	}

	void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, enum TileValues tilevalue, char* src, char propagate)
	{
		if (!tile) return;

		if (compassRoseX == 0) compassRoseX = tile->GetValueFloat(kTileValue_x);
		if (compassRoseY == 0) compassRoseY = tile->GetValueFloat(kTileValue_y);

		tile->SetFloat(kTileValue_width, 44, propagate);
		tile->SetFloat(kTileValue_height, 44, propagate);
		tile->SetFloat(kTileValue_x, compassRoseX + 3, propagate);
		tile->SetFloat(kTileValue_y, compassRoseY + 3, propagate);

		if (!IsTagForItem(entry)) {
			tile->SetString(tilevalue, src, propagate);
			return;
		}

		const std::string tag = GetTagForItem(entry);

		tile->SetString(tilevalue, g_Tags[tag].filename.empty() ? src : g_Tags[tag].filename.c_str(), propagate);
	}

	bool __fastcall HasContainerChangesEntry(ContChangesEntry* entry)
	{
		if (entry && entry->type) return false;
		return true;
	}

	bool __fastcall KeyringShowCategories(Tile* tile)
	{
		std::string tag;
		if (tile->GetValue(kTileValue_user16)) tag = tile->GetValue(kTileValue_user16)->str;
		if (g_Tags[tag].tab != InventoryMenu::GetSingleton()->filter) return true;

		const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
		UInt32 keys = 0;
		if (!entryDataList) return true;
		for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			if (iter->data && iter->data->type && g_Tags[GetTagForItem(iter->data)].category._Equal(tag)) keys += iter->data->countDelta;
		if (!keys) return true;
		std::string keyringname = g_Tags[tag].name;
		if (keyringname.find("&-") == 0)
			keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
		if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";

//		tile->SetString(kTileValue_string, keyringname.c_str(), false);
		return false;
	}

	bool __fastcall KeyringHideKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->type) return false;
		if (!g_Tags[GetTagForItem(entry)].category.empty()) return true;
		return false;
	}

	bool __cdecl KeyringHideNonKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->type) return true;
		if (g_Tags[GetTagForItem(entry)].category._Equal(openCategory)) {
			if (stringStewie.empty() || stringStewie._Equal("_")) return false;
			return !stristr(entry->type->GetTheName(), stringStewie.c_str());
		}
		return true;
	}

	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile)
	{
		if (tile && tile->GetValue(kTileValue_user16)) openCategory = tile->GetValue(kTileValue_user16)->str;
		invmenu->itemsList.Filter(KeyringHideNonKeys);
		invmenu->itemsList.ForEach((void(__cdecl*)(Tile*, ContChangesEntry*))0x780C00);
		invmenu->ResetInventorySelectionAndHideDataTile();
	}
	
	void __fastcall AddSortingCategories()
	{
		const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
		for (auto& entry : g_Categories)
		{
			UInt32 keys = 0;
			if (!entryDataList) return;
			for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			{
				if (!iter->data || !iter->data->type) continue;
				auto tag = g_Tags[GetTagForItem(iter->data)];
				if (!tag.category._Equal(entry)) continue;
				if (g_Tags[entry].count == 0) {
					keys = 1;
					break;
				}
				else if (g_Tags[entry].count == 1)
					keys += 1;
				else if (g_Tags[entry].count == 2)
					keys += iter->data->countDelta;
			}
			if (keys) {
				std::string keyringname = g_Tags[entry].name;
				if (keyringname.find("&-") == 0)
					keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
				if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";
				auto tile = InventoryMenu::GetSingleton()->itemsList.Insert(nullptr, keyringname.c_str(), nullptr, nullptr);
				tile->SetFloat(kTileValue_id, 30);
				tile->SetString(kTileValue_user16, entry.c_str());
				InjectIconTile(entry, &InventoryMenu::GetSingleton()->itemsList, tile, nullptr);
			}
		}
	}
}

namespace SI_Hooks
{
	__declspec(naked) void IconInjectTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::ListGetCountInjectTile);
		static const UInt32 retnAddr = 0x71A3DA;
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
	__declspec(naked) void IconHotkeyHUDTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetStringValueTagImage);
		static const UInt32 retnAddr = 0x7018A3;
		__asm
		{
			mov		edx, [ebp - 0x24]
			call    SetStringValue
			jmp		retnAddr
		}
	}

	__declspec(naked) void IconHotkeyPipBoyTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetStringValueTagRose);
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
		static const auto CompareItems = reinterpret_cast<UInt32>(SI::CompareItemsWithTags);
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

			got0 :
			mov edx, [ebp + 0xC]
				mov ecx, [edx + 0x4]
				call ContChangesEntry_GetFullName
				jmp retnAddr
		}
	}

	__declspec(naked) void SortingBarterContainerMenuHook()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(SI::CompareItemsWithTags);
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
		static const auto ShouldHide = reinterpret_cast<UInt32>(SI::KeyringHideKeys);
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
		static const auto HasContainerEntry = reinterpret_cast<UInt32>(SI::HasContainerChangesEntry);
		static const auto ShowCategories = reinterpret_cast<UInt32>(SI::KeyringShowCategories);
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
		static const auto AddCategories = reinterpret_cast<UInt32>(SI::AddSortingCategories);
		__asm
		{
			call AddCategories
			jmp retnAddr
		}
	}

	__declspec(naked) void KeyringHideNonKeysHook()
	{
		static const UInt32 retnAddr = 0x78083F;
		static const auto HideNonKeysAndGetTile = reinterpret_cast<UInt32>(SI::HideNonKeysGetTile);
		__asm
		{
			mov edx, [ebp + 0xC]
			call HideNonKeysAndGetTile
			jmp retnAddr
		}
	}

	__declspec(naked) void ContainerEntryListBoxFilterHookPre()
	{
		// push additional arg to filter function
		static const UInt32 retnAddr = 0x730C8C;
		__asm
		{			
			mov ecx, [ecx]					// ListBox::ListItem*
			push dword ptr ds : [ecx]		// ListItem->tile
			push dword ptr ds : [ecx + 4]	// ListItem->object	
			jmp retnAddr
//			call dword ptr ss : [ebp + 8] // shouldHide
//			pop		ecx // pop the extra pushed arg (ListItem->tile)
		}
	}

	__declspec(naked) void ContainerEntryListBoxFilterHookPost()
	{
		// fix stack
		static const UInt32 retnAddr = 0x730CA9;
		__asm
		{
			add esp, 4
			test al, al
			jz wah
			mov [ebp-0x24], 1
			jmp retnAddr
		wah:
			mov[ebp - 0x24], 0
			jmp retnAddr
		}
	}


	
	__declspec(naked) void KeyringEnableEquipHook()
	{
		static const UInt32 retnAddr = 0x78047D;
		__asm
		{
			mov eax, 0
			jmp retnAddr
		}
	}

	__declspec(naked) void KeyringEnableDropHook()
	{
		static const UInt32 retnAddr = 0x78093A;
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

	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, enum TileValues tilevalue, char* string, int propagate)
	{
		std::string stringnew = string;
		if (auto clickedtile = InventoryMenu::GetSingleton()->itemsList.selected; clickedtile->GetValue(kTileValue_user16)
			&& !SI::g_Tags[clickedtile->GetValue(kTileValue_user16)->str].icon.empty())
			stringnew = SI::g_Tags[clickedtile->GetValue(kTileValue_user16)->str].icon;
		tile->SetString(tilevalue, stringnew.c_str(), true);
	}
}