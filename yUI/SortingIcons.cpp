#include <SortingIcons.h>
#include <GameRTTI.h>
#include <settings.h>
#include <functions.h>
#include <filesystem>

namespace SI
{
	std::string stringStewie;
	std::string openCategory;
	float compassRoseX = 0, compassRoseY = 0;
}

namespace SIFiles
{
	bool AssignTagToItem(TESForm* form)
	{
		for (const auto &entry : g_Items_JSON) {
			if (entry.formCommon.form && entry.formCommon.form->refID != form->refID) continue;
			if (entry.formCommon.formType && entry.formCommon.formType != form->typeID) continue;

			if (entry.formCommon.questItem && entry.formCommon.questItem != static_cast<UInt8>(form->IsQuestItem2())) continue;
			if (entry.formCommon.miscComponent && !IsCraftingComponent(form)) continue;
			if (entry.formCommon.miscProduct && !IsCraftingProduct(form)) continue;

			if (entry.formCommon.formType == 40) {
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
				if (entry.formWeapon.ammo && !FormContainsRecusive(entry.formWeapon.ammo, weapon->ammo.ammo)) continue;
			}
			else if (entry.formCommon.formType == 24) {
				const auto armor = DYNAMIC_CAST(form, TESForm, TESObjectARMO);
				if (!armor) continue;
				if (entry.formArmor.armorSlotsMaskWL && (entry.formArmor.armorSlotsMaskWL & armor->GetArmorValue(6)) != entry.formArmor.armorSlotsMaskWL) continue;
				if (entry.formArmor.armorSlotsMaskWL && (entry.formArmor.armorSlotsMaskBL & armor->GetArmorValue(6)) != 0) continue;
				if (entry.formArmor.armorClass && entry.formArmor.armorClass != armor->GetArmorValue(1)) continue;
				if (entry.formArmor.armorPower && entry.formArmor.armorPower != armor->GetArmorValue(2)) continue;
				if (entry.formArmor.armorHasBackpack && entry.formArmor.armorHasBackpack != armor->GetArmorValue(3)) continue;

				if (entry.formArmor.armorDT && entry.formArmor.armorDT > armor->damageThreshold) continue;
				if (entry.formArmor.armorDR && entry.formArmor.armorDR > armor->armorRating) continue;
//				if (entry.formArmor.armorChangesAV && entry.formArmor.armorChangesAV > armor->armorRating) continue;
			}
			else if (entry.formCommon.formType == 31) {
			}
			else if (entry.formCommon.formType == 47) {
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

			SI::g_Items.emplace(form, entry.formCommon.tag);
			return true;
		}
		return false;
	}

	void FillSIMapsFromJSON()
	{
		ra::sort(g_Items_JSON, [&](const JSONEntryItem& entry1, const JSONEntryItem& entry2)
		         { return entry1.formCommon.priority > entry2.formCommon.priority; });

		if (!g_ySI_JustInTime)
		{
			for (const auto form : *GetAllForms()) {
				if (!form || !form->IsInventoryObjectAlt()) continue;
				AssignTagToItem(form);
			}
			//		SIFiles::g_Items_JSON = std::vector<JSONEntryItem>();
		}

		ra::sort(g_Tags_JSON, [&](const JSONEntryTag& entry1, const JSONEntryTag& entry2)
		         { return entry1.priority > entry2.priority; });
		for (auto& entry : g_Tags_JSON) {
			if (!entry.name.empty()) SI::g_Categories.emplace(entry.tag);
			SI::g_Tags.emplace(entry.tag, std::move(entry));
		}
		g_Tags_JSON = std::vector<JSONEntryTag>();
	}
}

namespace SI
{
	void InjectTemplates()
	{
		for (auto& iter : g_XMLPaths)
		{
			HUDMainMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
			RepairMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
			InventoryMenu::GetSingleton()->tile->InjectUIXML(iter.generic_string().c_str());
		}
	}

	void KeyringRefreshPostStewie()
	{
		if (CdeclCall<bool>(0x702360) && InterfaceManager::IsMenuVisible(kMenuType_Inventory) && InventoryMenu::GetSingleton()->IsKeyringOpen()) {
			if (Tile* stew = InventoryMenu::GetSingleton()->tile->GetChild("IM_SearchBar"); stew) {
				if (const auto string = stew->GetValue(kTileValue_string)->str; !stringStewie._Equal(string)) {
					stringStewie = string;
					InventoryMenu::GetSingleton()->itemsList.Filter(KeyringHideNonKeys);
					InventoryMenu::GetSingleton()->itemsList.ForEach(reinterpret_cast<void(*)(Tile*, ContChangesEntry*)>(0x780C00));
				}
			}
			else stringStewie.clear();
		}
	}

	bool IsTagForItem(TESForm* form)
	{
		if (form && g_Items.contains(form)) return true;
		return false;
	}

	bool IsTagForItem(ContChangesEntry* entry)
	{
		if (entry && entry->form && g_Items.contains(entry->form)) return true;
		return false;
	}
	
	std::string GetTagForItem(TESForm* form)
	{
		if (!form) return "";
		if (!IsTagForItem(form)) SIFiles::AssignTagToItem(form);
		return g_Items[form];
	}

	std::string GetTagForItem(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return "";
		if (!IsTagForItem(entry)) SIFiles::AssignTagToItem(entry->form);
		return g_Items[entry->form];
	}
	
	void InjectIconTile(const std::string& tag, MenuItemEntryList* list, Tile* tile, ContChangesEntry* entry)
	{
		//	if (g_Items.find(entry->type) == g_Items.end()) return;
		if (g_Tags[tag].filename.empty()) return;

		Tile* tilemenu = tile;

		do if IS_TYPE(tilemenu, TileMenu) break;
		while ((tilemenu = tilemenu->parent));

		const auto menu = DYNAMIC_CAST(tilemenu, Tile, TileMenu);

		if (!menu || !menu->menu) return;

		const auto text = tile->children.Tail()->data;

		if (!text || !std::string("ListItemText")._Equal(text->name.CStr())) return;

		if (!menu->menu->GetTemplateExists(g_Tags[tag].xmltemplate.c_str()))
		{
			if (menu != TileMenu::GetTileMenu(kMenuType_Barter) && menu != TileMenu::GetTileMenu(kMenuType_Container) && menu != TileMenu::GetTileMenu(kMenuType_RepairServices)) return;
			for (auto& iter : g_XMLPaths) menu->InjectUIXML(iter.generic_string().c_str());
			if (!menu->menu->GetTemplateExists(g_Tags[tag].xmltemplate.c_str())) return;
		}

		const auto last = tile->children.Head();

		const auto icon = menu->menu->AddTileFromTemplate(tile, g_Tags[tag].xmltemplate.c_str(), 0);

		const auto icondata = tile->children.Head();

		tile->children.ExchangeNodeData(icondata, last);
		
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

		icon->SetFloat(kTileValue_x, x, true);

		x += icon->GetValueFloat(kTileValue_width);

		if (icon->GetValue(kTileValue_user1)) x += icon->GetValueFloat(kTileValue_user1);

		const float wrapwidth = text->GetValueFloat(kTileValue_wrapwidth) - x;

		text->SetFloat(kTileValue_x, x, true);
		text->SetFloat(kTileValue_wrapwidth, wrapwidth, true);
	}

	void __fastcall SetTileStringInjectTile(Tile* tile, ContChangesEntry* entry, MenuItemEntryList* list, eTileValue tilevalue, char* tileText, bool propagate)
	{
		tile->SetString(tilevalue, tileText, propagate);
		if (entry && entry->form && TryGetTypeOfForm(entry->form)) InjectIconTile(GetTagForItem(entry), list, tile, entry);
	}

	signed int __fastcall CompareItemsWithTags(ContChangesEntry* a2, ContChangesEntry* a1, Tile* tile1, Tile* tile2)
	{
		TESForm* form1 = nullptr, * form2 = nullptr;

		if (a1 && a1->form) form1 = a1->form;
		if (a2 && a2->form) form2 = a2->form;

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

	void __fastcall SetStringValueTagImage(Tile* tile, ContChangesEntry* entry, eTileValue tilevalue, char* src, char propagate)
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

	void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, eTileValue tilevalue, char* src, char propagate)
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
		if (entry && entry->form) return false;
		return true;
	}

	bool __fastcall KeyringShowCategories(Tile* tile)
	{
		std::string tag;
		//&& *((UInt32*)0x78028E) == (UInt32)0x782620 && stringStewie.empty()
		if (tile && tile->GetValue(kTileValue_user16)) tag = tile->GetValue(kTileValue_user16)->str;
		if (g_Tags[tag].tab != InventoryMenu::GetSingleton()->filter) return true;

		const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
		UInt32 keys = 0;
		if (!entryDataList) return true;
		for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			if (iter->data && iter->data->form && g_Tags[GetTagForItem(iter->data)].category._Equal(tag))
			{
				if (g_Tags[tag].count == 0) {
					keys = 1;
					break;
				}
				else if (g_Tags[tag].count == 1)
					keys += 1;
				else if (g_Tags[tag].count == 2)
					keys += iter->data->countDelta;
			}
		if (!keys) return true;
		std::string keyringname = g_Tags[tag].name;
		if (keyringname.find("&-") == 0)
			keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
		if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";

		tile->SetString(kTileValue_string, keyringname.c_str(), false);
		return false;
	}

	bool __fastcall KeyringHideKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return false;
		if (!g_Tags[GetTagForItem(entry)].category.empty()) return true;
		return false;
	}

	bool __cdecl KeyringHideNonKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return true;
		if (g_Tags[GetTagForItem(entry)].category._Equal(openCategory)) {
			if (stringStewie.empty() || stringStewie._Equal("_")) return false;
			return !stristr(entry->form->GetTheName(), stringStewie.c_str());
		}
		return true;
	}

	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile)
	{
		if (tile && tile->GetValue(kTileValue_user16)) openCategory = tile->GetValue(kTileValue_user16)->str; else openCategory = "";
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
				if (!iter->data || !iter->data->form) continue;
				if (auto tag = g_Tags[GetTagForItem(iter->data)]; !tag.category._Equal(entry)) continue;
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

namespace SIHooks
{
	__declspec(naked) void SortingBarterContainerMenuHook()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(SI::CompareItemsWithTags);
		_asm
		{
			pop		eax
			mov		eax, [ebp + 0xC]	// a2
			mov		ecx, [eax]
			push	ecx					// tile2
			mov		ecx, [eax + 0x4]	// entry2

			mov		eax, [ebp + 0x8]	// a1
			mov		edx, [eax]
			push	edx					// tile1
			mov		edx, [eax + 0x4]	// entry1

			call	CompareItems
			mov		esp, ebp
			pop		ebp
			ret
		}
	}
	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, signed int a1)
	{
		tile->SetFloat(tilevalue, InventoryMenu::GetSingleton()->IsKeyringOpen(), true);
	}

	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* string, int propagate)
	{
		std::string stringnew = string;
		if (const auto clickedtile = InventoryMenu::GetSingleton()->itemsList.selected; clickedtile->GetValue(kTileValue_user16)
			&& !SI::g_Tags[clickedtile->GetValue(kTileValue_user16)->str].icon.empty())
			stringnew = SI::g_Tags[clickedtile->GetValue(kTileValue_user16)->str].icon;
		tile->SetString(tilevalue, stringnew.c_str(), true);
	}
}