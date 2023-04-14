#include "SortingIcons.h"

#include <Setting.h>
#include <Tile.h>
#include <InterfaceManager.h>
#include <functions.h>

#include <SafeWrite.h>

namespace SortingIcons
{
	inline bool Item::Common::Satisfies(TESForm* form) const
	{
		if (!formIDs.empty() &&				!formIDs.contains(form->refID)) return false;
		if (!formType.empty() &&			!formType.contains(form->typeID)) return false;

		if (questItem.has_value() &&		questItem.value() != form->IsQuestItem2()) return false;
		if (miscComponent.has_value() &&	miscComponent.value() != CraftingComponents::IsComponent(form)) return false;
		if (miscProduct.has_value() &&		miscProduct.value() != CraftingComponents::IsProduct(form)) return false;

		return true;
	}

	inline bool Item::Weapon::Satisfies(TESObjectWEAP* weapon) const
	{
		if (!skill.empty() && !skill.contains(weapon->weaponSkill)) return false;
		if (!type.empty() && !type.contains(weapon->eWeaponType)) return false;
		if (!handgrip.empty() && !handgrip.contains(weapon->HandGrip())) return false;
		if (!attackAnim.empty() && !attackAnim.contains(weapon->AttackAnimation())) return false;
		if (!reloadAnim.empty() && !reloadAnim.contains(weapon->reloadAnim)) return false;
		if (!type.empty() && !type.contains(weapon->eWeaponType)) return false;

		if (isAutomatic.has_value() && isAutomatic.value() != weapon->IsAutomatic()) return false;
		if (hasScope.has_value() && hasScope.value() != weapon->HasScopeAlt()) return false;
		if (ignoresDTDR.has_value() && ignoresDTDR.value() != weapon->IgnoresDTDR()) return false;

		const auto clipRounds = static_cast<UInt32>(weapon->GetClipRounds(false));
		if (clipRoundsMin.has_value() && clipRoundsMin.value() > clipRounds) return false;
		if (clipRoundsMax.has_value() && clipRoundsMax.value() < clipRounds) return false;

		const auto numProjectiles = weapon->numProjectiles;
		if (numProjectilesMin.has_value() && numProjectilesMin.value() > numProjectiles) return false;
		if (numProjectilesMax.has_value() && numProjectilesMax.value() < numProjectiles) return false;

		if (!soundLevel.empty() && !soundLevel.contains(weapon->soundLevel)) return false;
		if (!ammoIDs.empty() && !ammoIDs.contains(weapon->ammo.ammo->refID)) return false;

		return true;
	}

	inline bool Item::Armor::Satisfies(TESObjectARMO* armor) const
	{
		if (slotsMaskWL && (slotsMaskWL & armor->GetArmorValue(6)) != slotsMaskWL) return false;
		if (slotsMaskBL && (slotsMaskBL & armor->GetArmorValue(6)) != 0) return false;
		if (armorClass && armorClass != armor->GetArmorValue(1)) return false;
		if (powerArmor && powerArmor != armor->GetArmorValue(2)) return false;
		if (hasBackpack && hasBackpack != armor->GetArmorValue(3)) return false;

		if (dt && dt > armor->damageThreshold) return false;
		if (dr && dr > armor->armorRating) return false;
//		if (armorChangesAV &&	armorChangesAV > armor->armorRating) continue;


		return true;
	}

	inline bool Item::Aid::Satisfies(AlchemyItem* aid) const
	{
		if (restoresAV && !aid->HasBaseEffectRestoresAV(restoresAV)) return false;
		if (damagesAV && !aid->HasBaseEffectDamagesAV(damagesAV)) return false;
		if (isAddictive && !aid->IsAddictive()) return false;
		if (isFood && !aid->IsFood()) return false;
		if (isWater && !aid->IsWaterAlt()) return false;
		if (isPoisonous && !aid->IsPoison()) return false;
		if (isMedicine && !aid->IsMedicine()) return false;

		return true;
	}

	bool Item::Satisfies(TESForm* form) const
	{
		if (!common.Satisfies(form)) return false;

		if (form->typeID == kFormType_TESObjectWEAP && !weapon.Satisfies(reinterpret_cast<TESObjectWEAP*>(form))) return false;
		if (form->typeID == kFormType_TESObjectARMO && !armor.Satisfies(reinterpret_cast<TESObjectARMO*>(form))) return false;
		if (form->typeID == kFormType_AlchemyItem && !aid.Satisfies(reinterpret_cast<AlchemyItem*>(form))) return false;

		return true;
	}

	std::unordered_map<std::string, Category*>	g_StringToCategory;

	Category* Category::Set(const std::string tag)
	{
		return g_StringToCategory[tag] = this;
	}

	bool Category::IsValid() const
	{
		return categoryDefault;
	}

	Category* Category::Get(const std::string tag)
	{
		return g_StringToCategory[tag];
	}

	Category* Category::Satisfies(TESForm* form)
	{
		for (const auto& iter : g_Items) 
			if (iter->Satisfies(form)) return Get(iter->tag);
		return categoryDefault;
	}

	std::unordered_map<TESForm*, Category*> g_ItemToCategory;

	Category* Category::Set(TESForm* form)
	{
		return g_ItemToCategory[form] = this;
	}

	Category* Category::Get(TESForm* form)
	{
		return g_ItemToCategory.contains(form) ? g_ItemToCategory[form] : Satisfies(form)->Set(form);
	}

	bool Tab::Satisfies(TESForm* form) const
	{
		if (!tabMisc)
		{
			if (!types.empty() && !types.contains(form->typeID)) return false;
			if (const auto category = Category::Get(form); !categories.empty() && category->IsValid() &&
				!categories.contains(category->tag)) return false;
		}
		else
		{
			if (!types.empty() && types.contains(form->typeID)) return false;
			if (const auto category = Category::Get(form); !categories.empty() &&
				category->IsValid() && categories.contains(category->tag)) return false;
		}

		return true;
	}

}

namespace SortingIcons::Keyrings
{
	Tab* openTab;
	std::string stringStewie;

	std::unordered_map<Tab*, UInt32> itemCountsForKeyrings;

	bool update = false;

	void EquipUpdate()
	{
		update = true;
    }

	UInt32 __fastcall OpenKeyring(Tile* tile)
	{
		if (!tile) {}
		else if (const auto val = tile->GetValue(kTileValue_user16))
			openTab = g_StringToTabs[val->str];
		return *(UInt32*)0x011D9EB8;
	}

	UInt32 __fastcall CloseKeyring(Tile* tile)
	{
		openTab = nullptr;
		return *(UInt32*)0x011D9EB8;
	}

	UInt32 PostFilterUpdate()
	{
		const auto inventoryMenu = InventoryMenu::GetSingleton();
		if (!update) for (const auto list = inventoryMenu->itemsList.list; const auto iter : list)
			if (iter->tile && iter->tile->GetValue(kTileValue_user16)) { iter->tile->~Tile(); list.RemoveItem(iter); }

		for (const auto& key : itemCountsForKeyrings | std::views::keys)
		{
			const auto keys = itemCountsForKeyrings[key];

			std::string keyringname = key->name;
			if (keyringname.find("&-") == 0) keyringname = GetGameSetting(keyringname.substr(2, keyringname.length() - 3))->GetAsString();
			if (keys > 1) keyringname += " (" + std::to_string(keys) + ")";

			const auto listItem = inventoryMenu->itemsList.InsertAlt(nullptr, keyringname.c_str());
			const auto tile = listItem->tile;
			tile->Set(kTileValue_id, 30);
			tile->Set(kTileValue_user16, key->tag);
			inventoryMenu->itemsList.SortAlt(listItem, reinterpret_cast<ListBox<InventoryChanges>::SortingFunction>(0x7824E0));
			Icons::InjectIconTile(Category::Get(key->tag), tile);
		}

		itemCountsForKeyrings.clear();

		update = false;
		ThisCall(0x71A670, &inventoryMenu->itemsList);
		return inventoryMenu->filter;
	}

	bool VanillaTypeFilter(InventoryChanges* entry)
	{
		const auto type = entry->form->TryGetREFRParent()->typeID;
		const auto filter = InventoryMenu::GetSingleton()->filter;

		switch (type)
		{
		case kFormType_TESObjectWEAP:	return filter != 0;
		case kFormType_TESObjectARMO:
		case kFormType_TESObjectCLOT:	return filter != 1;
		case kFormType_IngredientItem: 
		case kFormType_AlchemyItem:
		case kFormType_TESObjectBOOK:	return filter != 2;
		case kFormType_TESAmmo:			return filter != 4;
		default:						return filter != 3;
		}
	}

	bool KeyringHideKeys(InventoryChanges* entry)
	{
		const auto form = entry->form->TryGetREFRParent();

		bool shouldHide = false;
		for (const auto& keyring : g_Keyrings)
		{
			if (!keyring->Satisfies(form)) continue;

			shouldHide = true;
			if (keyring->keyring == 1) itemCountsForKeyrings[keyring] = 1;
			else if (keyring->keyring == 2) itemCountsForKeyrings[keyring] += 1;
			else if (keyring->keyring == 3) itemCountsForKeyrings[keyring] += entry->countDelta;
		}

		return shouldHide;
	}


	bool __fastcall KeyringFilter(InventoryChanges* entry, Tile* tile)
	{
		if (!entry || !entry->form) return true;

		const auto form = entry->form->TryGetREFRParent();

		if (!form->IsItemPlayable()) return true;
		if (form->refID == 0xF) return true;
		if (form->IsQuestItem() && !PlayerCharacter::GetSingleton()->showQuestItems) return true;

		if (VanillaTypeFilter(entry)) return true;

		return openTab && openTab->keyring ? !openTab->Satisfies(form) : KeyringHideKeys(entry);
		/* TODO:
			if (Categories::ItemGetCategory(entry)->category._Equal(openCategory)) {
			if (stringStewie.empty() || stringStewie._Equal("_")) return false;
			return !stristr(entry->form->GetTheName(), stringStewie.c_str());
		}*/
	}

	/*
	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile)
	{
		if (tile && tile->GetValue(kTileValue_user16)) openCategory = tile->GetValue(kTileValue_user16)->str; else openCategory = "";
		invmenu->itemsList.Filter(KeyringHideNonKeys);
		invmenu->itemsList.ForEach(reinterpret_cast<void(*)(Tile*, InventoryChanges*)>(0x780C00));
		invmenu->ResetInventorySelectionAndHideDataTile();
	}
	*/

	/*
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
//			scrollPosKeyring[openCategory].listIndex = listIndex;
//			scrollPosKeyring[openCategory].currentValue = currentValue;
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
//			if (scrollPosKeyring.contains(openCategory))
			{
//				listIndex = scrollPosKeyring[openCategory].listIndex;
//				currentValue = scrollPosKeyring[openCategory].currentValue;
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

	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* string, int propagate)
	{
		
		std::string stringnew = string;
		if (const auto clickedtile = InventoryMenu::GetSingleton()->itemsList.selected; clickedtile->GetValue(kTileValue_user16)
			&& !g_StringToCategory[clickedtile->GetValue(kTileValue_user16)->str]->icon.empty())
			stringnew = g_StringToCategory[clickedtile->GetValue(kTileValue_user16)->str]->icon;
		tile->SetString(tilevalue, stringnew.c_str(), true);
	}

	void KeyringRefreshPostStewie()
	{
		if (CdeclCall<bool>(0x702360) && InterfaceManager::IsMenuVisible(kMenuType_Inventory) && InventoryMenu::GetSingleton()->IsKeyringOpen()) {
			if (Tile* stew = InventoryMenu::GetSingleton()->tile->GetChild("IM_SearchBar"); stew) {
				if (const auto string = stew->GetValue(kTileValue_string)->str; !stringStewie._Equal(string)) {
					stringStewie = string;
					InventoryMenu::GetSingleton()->itemsList.Filter(KeyringHideNonKeys);
					InventoryMenu::GetSingleton()->itemsList.ForEach(reinterpret_cast<void(*)(Tile*, InventoryChanges*)>(0x780C00));
				}
			}
			else stringStewie.clear();
		}
	}
	*/
}

namespace SortingIcons::Keyrings::Hook
{
	__declspec(naked) void KeyringHideKeysShowCategories()
	{
		static const auto Filter = reinterpret_cast<UInt32>(Keyrings::KeyringFilter);
		__asm
		{
			mov		ecx, [ebp + 0x8] // a1
			mov		edx, [ebp + 0xC]
			call	Filter

			mov		esp, ebp
			pop		ebp
			ret
		}
	}

	template<UInt32 retn> __declspec(naked) void KeyringEnableEquipDrop()
	{
		static const UInt32 retnAddr = retn;
		static const auto AddCategories = reinterpret_cast<UInt32>(Keyrings::EquipUpdate);
		__asm
		{
			call	AddCategories
			mov		eax, 0
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void PostFilterUpdate()
	{
		static const UInt32 retnAddr = retn;
		static const auto AddCategories = reinterpret_cast<UInt32>(Keyrings::PostFilterUpdate);
		__asm
		{
			call	AddCategories
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

/*
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
	*/

	template <UInt32 retn> __declspec(naked) void OpenKeyring()
	{
		static const UInt32 retnAddr = retn;
		static const auto ShouldHide = reinterpret_cast<UInt32>(Keyrings::OpenKeyring);
		__asm
		{
			mov ecx, [ebp + 0xC]
			call ShouldHide
			jmp retnAddr
		}
	}


	template <UInt32 retn> __declspec(naked) void CloseKeyring()
	{
		static const UInt32 retnAddr = retn;
		static const auto ShouldHide = reinterpret_cast<UInt32>(Keyrings::CloseKeyring);
		__asm
		{
			mov ecx, [ebp + 0xC]
			call ShouldHide
			mov ecx, eax
			jmp retnAddr
		}
	}

	template <UInt32 retn> __declspec(naked) void IsKey()
	{
		static const UInt32 retnAddr = retn;
		static const auto ShouldHide = reinterpret_cast<UInt32>(Keyrings::KeyringFilter);
		__asm
		{
			call ShouldHide
			mov	[ebp - 0x4], eax
			jmp retnAddr
		}
	}
}

namespace SortingIcons::Tabs
{
//	std::unordered_map<TESForm*, std::unordered_set<TabPtr>> g_ItemToTabs;

	void ItemAssignTabs(TESForm* form)
	{
		std::unordered_set<Tab*> set;
/*
		for (const auto& iter : g_Tabs) {
			const auto& entry = *iter;
//			if (!entry.tabMisc.empty()) continue;
			if (!entry.types.empty() && !entry.types.contains(form->typeID)) continue;
//			if (!entry.categories.empty() && !entry.categories.contains(Categories::ItemGetCategory(form)->category)) continue;
			set.emplace(iter);
		}

		for (const auto& iter : g_Tabs) {
			const auto& entry = *iter;
//			if (entry.tabMisc.empty()) continue;
			bool misc = true;
			for (const auto& it : entry.tabMisc) if (set.contains(g_StringToTabs[it])) { misc = false; break; }
			if (misc) set.emplace(iter);
		}

		g_ItemToTabs.emplace(form, std::move(set));*/
	}

	void ItemAssignTabs(const InventoryChanges* entry)
	{
		return ItemAssignTabs(entry->form->TryGetREFRParent());
	}

	bool ItemHasTabs(TESForm* form)
	{
		if (!form) return false;
//		if (!g_ItemToTabs.contains(form)) return false;
		return true;
	}

	bool ItemHasTabs(const InventoryChanges* entry)
	{
		if (!entry) return false;
		return ItemHasTabs(entry->form->TryGetREFRParent());
	}


	std::vector<Tile*> tablineTiles;

	void SetUpTabline(TileRect* tabline, int traitID, const char* strWeapon, const char* strApparel, const char* strAid,
		const char* strMisc, const char* strAmmo, char* zero)
	{
		UInt32 listIndex = 0;
		Float32 width = 0;
		const auto menu = tabline->GetParentMenu();

		tabline->Set(kTileValue_clips, true);
		int i = 0;
		for (const auto& tab : g_Tabline)
		{
			const auto tile = menu->AddTileFromTemplate(tabline, "TabButtonTemplate", 0);

			auto string = tab->name;
	//		Log(tab->tab);
			Log() << (string);
			if (string.find("&-") == 0)
				string = GetGameSetting(string.substr(2, string.length() - 3))->GetAsString();
			Log() << (string);
			tile->Set(kTileValue_string, string, true);
			tile->Set(kTileValue_listindex, listIndex);
			tile->Set(kTileValue_id, traitID); // + listIndex
			tile->Set(kTileValue_clips, true);
			CdeclCall<void>(0xA04200, 0);
			if (i < 5) width += trunc(tile->Get(kTileValue_width));
			listIndex++;
			tablineTiles.push_back(tile);
			i++;
		}

		const auto tablineWidth = tabline->Get(kTileValue_width);
		const auto leftLineLength = trunc((tablineWidth - width) / (listIndex + 1));
		tabline->Set(("_LeftLineLength"), leftLineLength);
		tabline->Set(("_ButtonCount"), tablineWidth);

		auto _x = leftLineLength;
		for (const auto tile : tablineTiles)
		{
			tile->Set(("_x"), _x);
			_x += leftLineLength + trunc(tile->Get(kTileValue_width));
			i++;
		}

		//	CdeclCall(0x707BE0, tabline, traitID, "<<", strWeapon, strApparel, strAid, ">>", zero);
	}

	UInt32 __fastcall InventoryMenuHandleClickGetFilter(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile)
	{
		return clickedTile->Get(kTileValue_listindex);
	}

	void __fastcall InventoryMenuSetupData(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile)
	{
		const auto listIndex = clickedTile->Get(kTileValue_listindex);

		if (listIndex == 0) {
			menu->tileModButton->Set(kTileValue_visible, true);
		}
		else if (listIndex == 1) {
			ThisCall(0x718630, &InterfaceManager::GetSingleton()->help, InterfaceManager::kHelpApparel, InventoryMenu::GetMenuID(), 512);
			menu->tileModButton->Set(kTileValue_visible, false);
			menu->tileItemIconBadge->Set(kTileValue_visible, false);
			menu->tileStrReq->Set(kTileValue_visible, false);
		}
		else if (listIndex == 4) {
			ThisCall(0x718630, &InterfaceManager::GetSingleton()->help, InterfaceManager::kHelpAmmo, InventoryMenu::GetMenuID(), 500);
			menu->itemsList.ForEach(reinterpret_cast<ListBox<InventoryChanges>::ForEachFunc>(0x782850), 0, 0x7FFFFFFF);
		}
		else {
			menu->tileModButton->Set(kTileValue_visible, false);
			menu->tileItemIconBadge->Set(kTileValue_visible, false);
			menu->tileStrReq->Set(kTileValue_visible, false);
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

	UInt8 __fastcall InventoryMenuShouldHideItem(InventoryChanges* entry)
	{
		if (!entry || !entry->form) return true;

		const auto filter = InventoryMenu::GetSingleton()->filter;
		if (!Tabs::ItemHasTabs(entry)) Tabs::ItemAssignTabs(entry);
		//	if (SI::g_Tabs[SI::g_Tabline[filter]].tabNew) return false;

//		if (Tabs::g_ItemToTabs[entry->form].contains(g_Tabline[filter])) return false;

		return true;
	}
}

namespace SortingIcons::Tabs::Hook
{
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

namespace SortingIcons::Sorting
{
	SInt32 CompareWithTags(const ListBoxItem<InventoryChanges>* unk1, const ListBoxItem<InventoryChanges>* unk2)
	{
		if (bSort)
		{
			const auto a1 = unk1->object;
			const auto a2 = unk2->object;
			const auto tile1 = unk1->tile;
			const auto tile2 = unk2->tile;

			std::string tag1, tag2;

			if (a1 && a1->form)
				if (const auto form1 = a1->form->TryGetREFRParent())
					if (const auto category1 = Category::Get(form1)) 
						tag1 = category1->tag;
			if (a2 && a2->form)
				if (const auto form2 = a2->form->TryGetREFRParent())
					if (const auto category2 = Category::Get(form2))
						tag2 = category2->tag;

			if (bCategories && !g_Keyrings.empty())
			{
				if (tag1.empty() && tile1 && tile1->GetValue(kTileValue_user16)) tag1 = tile1->GetValue(kTileValue_user16)->str;
				if (tag2.empty() && tile2 && tile2->GetValue(kTileValue_user16)) tag2 = tile2->GetValue(kTileValue_user16)->str;
			}

			if (const auto cmp = tag1 <=> tag2; cmp != nullptr) return cmp._Value;

		}
		return 0;
	}

	SInt32 __fastcall CompareItems(const ListBoxItem<InventoryChanges>* unk1, const ListBoxItem<InventoryChanges>* unk2)
	{
		const auto a1 = unk1->object;
		const auto a2 = unk2->object;
		const auto tile1 = unk1->tile;
		const auto tile2 = unk2->tile;

		TESForm* form1 = nullptr, *form2 = nullptr;

		if (a1 && a1->form) form1 = a1->form->TryGetREFRParent();
		if (a2 && a2->form) form2 = a2->form->TryGetREFRParent();

		if (const auto cmp = CompareWithTags(unk1, unk2)) return cmp;

		std::string name1, name2;

		if (form1) name1 = form1->GetTheName();
		if (form2) name2 = form2->GetTheName();

		if (name1.empty() && tile1->GetValue(kTileValue_string)) name1 = tile1->GetValue(kTileValue_string)->str;
		if (name2.empty() && tile2->GetValue(kTileValue_string)) name2 = tile2->GetValue(kTileValue_string)->str;

		if (const auto cmp = name1 <=> name2; cmp != nullptr) return cmp._Value;

		if (!form1) return form2 ? -1 : 0;
		if (!form2) return 1;
		
		if (const auto weaponMods = a2->GetWeaponMod() <=> a1->GetWeaponMod(); weaponMods != nullptr) return weaponMods._Value;
		if (const auto condition = a2->GetHealthPercent() <=> a1->GetHealthPercent(); condition != nullptr) return condition._Value;
		if (const auto equipped = a2->GetEquipped() <=> a1->GetEquipped(); equipped != nullptr) return equipped._Value;
		if (const auto refID = form2->refID <=> form1->refID; refID != nullptr) return refID._Value;

		return 0;
	}
}

namespace SortingIcons::Sorting::Hook
{
	inline __declspec(naked) void BarterContainerMenu()
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

	template<UInt32 retn> __declspec(naked) void InventoryMenu()
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

}

namespace SortingIcons::Patch
{
	void AlterSorting(const bool bEnable)
	{
		if (bEnable)
		{
			WriteRelJump(0x7824F6, Sorting::Hook::InventoryMenu<0x78251B>);
			WriteRelJump(0x78250B, Sorting::Hook::InventoryMenu<0x78251B>);
			WriteRelJump(0x782516, Sorting::Hook::InventoryMenu<0x78251B>);

			WriteRelJump(0x72F0ED, Sorting::Hook::BarterContainerMenu);
			WriteRelJump(0x75D138, Sorting::Hook::BarterContainerMenu);
		}
		else
		{
			UndoSafeWrite(0x7824F6);
			UndoSafeWrite(0x78250B);
			UndoSafeWrite(0x782516);

			UndoSafeWrite(0x72F0ED);
			UndoSafeWrite(0x75D138);
		}
	}

		void AddKeyrings(const bool bEnable)
	{
		if (bEnable)
		{
			// Gives more information to ShouldHide
			WriteRelJump(0x730C81, Keyrings::Hook::ContainerEntryListBoxFilterPre<0x730C8C>);
			WriteRelJump(0x730C97, Keyrings::Hook::ContainerEntryListBoxFilterPost<0x730CA9>);

			// Skips creating keyring before we count up the keys
			WriteRelJump(0x7831C1, 0x783213);

			// Removes keys, adds to keyring TODO:: unite this with new categories thing
			WriteRelJump(0x782665, Keyrings::Hook::KeyringHideKeysShowCategories);

			// Makes keyrings
			WriteRelJump(0x7800C6, Keyrings::Hook::PostFilterUpdate<0x7800FA>);

			// Opens keyring
			WriteRelJump(0x7807F5, Keyrings::Hook::OpenKeyring<0x7807FA>);

			// Items in keyring category TODO:: unite this with new categories thing
			WriteRelJump(0x78281D, Keyrings::Hook::IsKey<0x78283D>);

			// Closes keyring
			WriteRelJump(0x7801F2, Keyrings::Hook::CloseKeyring<0x7801F8>);
			WriteRelJump(0x78087D, Keyrings::Hook::CloseKeyring<0x780883>);

			
			// Patching keyring interactions so it doesn't die
			WriteRelJump(0x780478, Keyrings::Hook::KeyringEnableEquipDrop<0x78048D>);
			WriteRelJump(0x780934, Keyrings::Hook::KeyringEnableEquipDrop<0x78094A>);
			WriteRelJump(0x782F2E, 0x782F47);
		}
		else
		{

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
		WriteRelJump(0x7801B2, Tabs::Hook::InventoryMenuHandleClick1<0x7801B8>);
		WriteRelJump(0x780215, Tabs::Hook::InventoryMenuHandleClick2<0x78021B>);
		WriteRelJump(0x78027C, Tabs::Hook::InventoryMenuHandleClick3<0x780281>);
		WriteRelJump(0x7802C9, Tabs::Hook::InventoryMenuHandleClick4<0x780386>);
		WriteRelJump(0x78232C, Tabs::Hook::InventoryMenuHandleSpecialInput<0x782371>);
	//	WriteRelJump(0x77FFD6, Tabs::Hook::InventoryMenuSaveScrollPosition<0x7800BB>);
	//	WriteRelJump(0x7800C6, Tabs::Hook::InventoryMenuRestoreScrollPosition<0x78013B>);
		WriteRelJump(0x782704, Tabs::Hook::InventoryMenuShouldHideItem<0x7827F1>);

		WriteRelCall(0x77FDF3, Tabs::SetUpTabline);
	//	WriteRelJump(0x78232C, 0x782362);
	}
}