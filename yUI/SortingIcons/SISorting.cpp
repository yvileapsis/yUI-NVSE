#include "SI.h"
#include <GameSettings.h>
#include <InterfaceTiles.h>

#include "InterfaceManager.h"

namespace SI::Sorting
{

	std::string openCategory;
	std::string stringStewie;

	bool IsTagForItem(TESForm* form)
	{
		if (form && g_ItemToCategory.contains(form)) return true;
		return false;
	}

	bool IsTagForItem(ContChangesEntry* entry)
	{
		if (entry && entry->form && g_ItemToCategory.contains(entry->form)) return true;
		return false;
	}

	std::string GetCategoryForItem(TESForm* form)
	{
		if (!form) return "";
		if (!IsTagForItem(form)) Files::AssignCategoryToItem(form);
		return g_ItemToCategory[form];
	}

	std::string GetCategoryForItem(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return "";
		if (!IsTagForItem(entry)) Files::AssignCategoryToItem(entry->form);
		return g_ItemToCategory[entry->form];
	}

	bool IsFilterForItem(TESForm* form)
	{
		if (form && g_ItemToFilter.contains(form)) return true;
		return false;
	}

	bool IsFilterForItem(ContChangesEntry* entry)
	{
		if (entry && entry->form && g_ItemToFilter.contains(entry->form)) return true;
		return false;
	}


	SInt32 __fastcall CompareItemsWithTags(const TileContChangesEntryUnk* unk1, const TileContChangesEntryUnk* unk2)
	{
		const auto a1 = unk1->entry;
		const auto a2 = unk2->entry;
		const auto tile1 = unk1->tile;
		const auto tile2 = unk2->tile;

		TESForm* form1 = nullptr, * form2 = nullptr;

		if (a1 && a1->form) form1 = a1->form->TryGetREFRParent();
		if (a2 && a2->form) form2 = a2->form->TryGetREFRParent();

		signed int cmp;

		if (bSort)
		{
			std::string tag1, tag2;

			if (form1) tag1 = GetCategoryForItem(form1);
			if (form2) tag2 = GetCategoryForItem(form2);

			if (bCategories && !g_Keyrings.empty())
			{
				if (tag1.empty() && tile1 && tile1->GetValue(kTileValue_user16)) tag1 = tile1->GetValue(kTileValue_user16)->str;
				if (tag2.empty() && tile2 && tile2->GetValue(kTileValue_user16)) tag2 = tile2->GetValue(kTileValue_user16)->str;
			}

			if (tag1.empty())
			{
				if (!tag2.empty()) return 1;
			}
			else if (tag2.empty()) return -1;
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

		const SInt16 mods1 = a1->GetWeaponMod();
		const SInt16 mods2 = a2->GetWeaponMod();
		if (mods1 != mods2) return mods1 > mods2 ? -1 : 1;

		const float condition1 = a1->GetHealthPercent();
		const float condition2 = a2->GetHealthPercent();
		if (condition1 != condition2) return condition1 > condition2 ? -1 : 1;

		const bool equipped1 = a1->GetEquipped();
		const bool equipped2 = a2->GetEquipped();
		if (equipped1 != equipped2) return equipped1 > equipped2 ? -1 : 1;

		const UInt32 refID1 = form1->refID;
		const UInt32 refID2 = form2->refID;
		if (refID1 != refID2) return refID1 > refID2 ? -1 : 1;

		return 0;
	}

	bool __fastcall KeyringShowCategories(Tile* tile)
	{
		std::string tag;
		//&& *((UInt32*)0x78028E) == (UInt32)0x782620 && stringStewie.empty()
		if (tile && tile->GetValue(kTileValue_user16)) tag = tile->GetValue(kTileValue_user16)->str;
		if (g_StringToCategory[tag].tab != InventoryMenu::GetSingleton()->filter) return true;

		const auto entryDataList = PlayerCharacter::GetSingleton()->GetContainerChangesList();
		UInt32 keys = 0;
		if (!entryDataList) return true;
		for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			if (iter->data && iter->data->form && g_StringToCategory[GetCategoryForItem(iter->data)].category._Equal(tag))
			{
				if (g_StringToCategory[tag].count == 0) {
					keys = 1;
					break;
				}
				else if (g_StringToCategory[tag].count == 1)
					keys += 1;
				else if (g_StringToCategory[tag].count == 2)
					keys += iter->data->countDelta;
			}
		if (!keys) return true;
		std::string keyringname = g_StringToCategory[tag].name;
		if (keyringname.find("&-") == 0)
			keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
		if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";

		tile->SetString(kTileValue_string, keyringname.c_str(), false);
		return false;
	}

	bool __fastcall KeyringHideKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return false;
		if (!g_StringToCategory[GetCategoryForItem(entry)].category.empty()) return true;
		return false;
	}

	bool __cdecl KeyringHideNonKeys(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return true;
		if (g_StringToCategory[GetCategoryForItem(entry)].category._Equal(openCategory)) {
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
		for (auto& entry : g_Keyrings)
		{
			UInt32 keys = 0;
			if (!entryDataList) return;
			for (auto iter = entryDataList->Head(); iter; iter = iter->next)
			{
				if (!iter->data || !iter->data->form) continue;
				if (auto tag = g_StringToCategory[GetCategoryForItem(iter->data)]; !tag.category._Equal(entry)) continue;
				if (g_StringToCategory[entry].count == 0) {
					keys = 1;
					break;
				}
				else if (g_StringToCategory[entry].count == 1)
					keys += 1;
				else if (g_StringToCategory[entry].count == 2)
					keys += iter->data->countDelta;
			}
			if (keys) {
				std::string keyringname = g_StringToCategory[entry].name;
				if (keyringname.find("&-") == 0)
					keyringname = GetStringFromGameSettingFromString(keyringname.substr(2, keyringname.length() - 3));
				if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";
				const auto tile = InventoryMenu::GetSingleton()->itemsList.Insert(nullptr, keyringname.c_str(), nullptr, nullptr);
				tile->SetFloat(kTileValue_id, 30);
				tile->SetString(kTileValue_user16, entry.c_str());
				Icons::InjectIconTile(g_StringToCategory[entry], &InventoryMenu::GetSingleton()->itemsList, tile, nullptr);
			}
		}
	}

	bool __fastcall HasContainerChangesEntry(ContChangesEntry* entry)
	{
		if (entry && entry->form) return false;
		return true;
	}




	std::vector<Tile*> tablineTiles;

	void SetUpTabline(TileRect* tabline, int traitID, const char* strWeapon, const char* strApparel, const char* strAid,
		const char* strMisc, const char* strAmmo, char* zero)
	{
		UInt32 listIndex = 0;
		Float32 width = 0;
		const auto menu = tabline->GetParentMenu();

		tabline->SetFloat(kTileValue_clips, true);
		int i = 0;
		for (const auto& tab : g_Tabline)
		{
			const auto tile = menu->AddTileFromTemplate(tabline, "TabButtonTemplate", 0);

			auto string = g_StringToTabs[tab].name;
			Log(tab);
			Log(string);
			if (string.find("&-") == 0)
				string = GetStringFromGameSettingFromString(string.substr(2, string.length() - 3));
			Log(string);
			tile->SetString(kTileValue_string, string.c_str(), true);
			tile->SetFloat(kTileValue_listindex, listIndex);
			tile->SetFloat(kTileValue_id, traitID); // + listIndex
			tile->SetFloat(kTileValue_clips, true);
			CdeclCall<void>(0xA04200, 0);
			if (i < 5) width += trunc(tile->GetFloat(kTileValue_width));
			listIndex++;
			tablineTiles.push_back(tile);
			i++;
		}

		const auto tablineWidth = tabline->GetFloat(kTileValue_width);
		const auto leftLineLength = trunc((tablineWidth - width) / (listIndex + 1));
		tabline->SetFloat(Tile::TraitNameToID("_LeftLineLength"), leftLineLength);
		tabline->SetFloat(Tile::TraitNameToID("_ButtonCount"), tablineWidth);

		auto _x = leftLineLength;
		for (const auto tile : tablineTiles)
		{
			tile->SetFloat(Tile::TraitNameToID("_x"), _x);
			_x += leftLineLength + trunc(tile->GetFloat(kTileValue_width));
			i++;
		}

		//	CdeclCall(0x707BE0, tabline, traitID, "<<", strWeapon, strApparel, strAid, ">>", zero);
	}

	UInt32 __fastcall InventoryMenuHandleClickGetFilter(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile)
	{
		return clickedTile->GetFloat(kTileValue_listindex);
	}

	void __fastcall InventoryMenuSetupData(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile)
	{
		const auto listIndex = clickedTile->GetFloat(kTileValue_listindex);

		if (listIndex == 0) {
			menu->tileModButton->SetFloat(kTileValue_visible, true);
		}
		else if (listIndex == 1) {
			ThisCall(0x718630, &InterfaceManager::GetSingleton()->help, InterfaceManager::kHelpApparel, InventoryMenu::GetMenuID(), 512);
			menu->tileModButton->SetFloat(kTileValue_visible, false);
			menu->tileItemIconBadge->SetFloat(kTileValue_visible, false);
			menu->tileStrReq->SetFloat(kTileValue_visible, false);
		}
		else if (listIndex == 4) {
			ThisCall(0x718630, &InterfaceManager::GetSingleton()->help, InterfaceManager::kHelpAmmo, InventoryMenu::GetMenuID(), 500);
			menu->itemsList.ForEach(reinterpret_cast<ListBox<ContChangesEntry>::ForEachFunc>(0x782850), 0, 0x7FFFFFFF);
		}
		else {
			menu->tileModButton->SetFloat(kTileValue_visible, false);
			menu->tileItemIconBadge->SetFloat(kTileValue_visible, false);
			menu->tileStrReq->SetFloat(kTileValue_visible, false);
		}
	}

	Tile* __fastcall InventoryMenuChooseTab(SInt32 key, UInt32 filter)
	{
		SInt32 newFilter = filter;
		if (key == kMenu_RightArrow) newFilter++;
		if (key == kMenu_LeftArrow) newFilter--;
		if (newFilter < 0) newFilter = tablineTiles.size() - 1;
		if (newFilter > tablineTiles.size() - 1) newFilter = 0;
		return tablineTiles[newFilter];
	}

	std::unordered_map<UInt32, InventoryMenu::ScrollPos> scrollPosTab;
	std::unordered_map<std::string, InventoryMenu::ScrollPos> scrollPosKeyring;

	void InventoryMenuSaveScrollPosition()
	{
		const auto menu = InventoryMenu::GetSingleton();
		SInt32 listIndex;
		if (const auto item = menu->itemsList.GetSelectedTile())
			listIndex = trunc(item->GetFloat(kTileValue_listindex));
		else
			listIndex = -1;
		const SInt32 currentValue = menu->itemsList.scrollBar->GetFloat("_current_value");

		if (InventoryMenu::IsKeyringOpen())
		{
			scrollPosKeyring[openCategory].listIndex = listIndex;
			scrollPosKeyring[openCategory].currentValue = currentValue;
		}
		else
		{
			scrollPosTab[menu->filter].listIndex = listIndex;
			scrollPosTab[menu->filter].currentValue = currentValue;
		}

	}

	void InventoryMenuRestoreScrollPosition()
	{
		const auto menu = InventoryMenu::GetSingleton();
		SInt32 listIndex = 0;
		SInt32 currentValue = 0;
		if (InventoryMenu::IsKeyringOpen())
		{
			if (scrollPosKeyring.contains(openCategory))
			{
				listIndex = scrollPosKeyring[openCategory].listIndex;
				currentValue = scrollPosKeyring[openCategory].currentValue;
			}
		}
		else
		{
			if (const auto filter = menu->filter; scrollPosTab.contains(filter))
			{
				listIndex = scrollPosTab[filter].listIndex;
				currentValue = scrollPosTab[filter].currentValue;
			}
		}
		menu->itemsList.RestoreScrollPositionProxy(listIndex, currentValue);
	}

	UInt8 __fastcall InventoryMenuShouldHideItem(ContChangesEntry* entry)
	{
		if (!entry || !entry->form) return true;

		const auto filter = InventoryMenu::GetSingleton()->filter;
		if (!IsFilterForItem(entry)) Files::AssignFiltersToItem(entry->form);
		//	if (SI::g_Tabs[SI::g_Tabline[filter]].tabNew) return false;

		if (g_ItemToFilter[entry->form].contains(g_Tabline[filter])) return false;

		return true;
	}

	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, signed int a1)
	{
		tile->SetFloat(tilevalue, InventoryMenu::GetSingleton()->IsKeyringOpen(), true);
	}

	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* string, int propagate)
	{
		std::string stringnew = string;
		if (const auto clickedtile = InventoryMenu::GetSingleton()->itemsList.selected; clickedtile->GetValue(kTileValue_user16)
			&& !g_StringToCategory[clickedtile->GetValue(kTileValue_user16)->str].icon.empty())
			stringnew = g_StringToCategory[clickedtile->GetValue(kTileValue_user16)->str].icon;
		tile->SetString(tilevalue, stringnew.c_str(), true);
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
}
