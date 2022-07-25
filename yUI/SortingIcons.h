#pragma once
#include <GameUI.h>
#include <filesystem>
#include <set>
#include <unordered_set>
#include <unordered_map>

void LoadSIMapsFromFiles();

namespace SIFiles
{
	class JSONEntryItemCommon
	{
	public:
		std::string		tag;
		SInt32			priority		= 0;
		TESForm*		form			= nullptr;
		UInt32			formType		= 0;
		UInt8			questItem		= 0;
		UInt8			miscComponent	= 0;
		UInt8			miscProduct		= 0;

		JSONEntryItemCommon() = default;
	};

	class JSONEntryItemWeapon
	{
	public:
		UInt32			skill;
		UInt32			type;
		UInt32			handgrip;
		UInt32 			attackAnim;
		UInt32 			reloadAnim;
		UInt32 			isAutomatic;
		UInt32 			hasScope;
		UInt32 			ignoresDTDR;
		UInt32 			clipRounds;
		UInt32 			numProjectiles;
		UInt32 			soundLevel;

		TESForm*		ammo;

		JSONEntryItemWeapon() = default;
	};

	class JSONEntryItemArmor
	{
	public:
		UInt32			slotsMaskWL;
		UInt32 			slotsMaskBL;

		UInt16 			armorClass;
		SInt8 			powerArmor;
		SInt8 			hasBackpack;

		Float32 		dt;
		UInt16 			dr;
		UInt16 			changesAV;

		JSONEntryItemArmor() = default;
	};

	class JSONEntryItemAid
	{
	public:
		UInt8  			restoresAV;
		UInt8  			damagesAV;
		UInt8  			isAddictive;
		UInt8  			isWater;
		UInt8  			isFood;
		UInt8  			isMedicine;
		UInt8  			isPoisonous;

		JSONEntryItemAid() = default;
	};

	class JSONEntryItemMisc
	{
	public:
		JSONEntryItemMisc() = default;
	};

	class JSONEntryItem
	{
	public:

		JSONEntryItemCommon			common{};
		union {
			JSONEntryItemWeapon		weapon{};
			JSONEntryItemArmor		armor;
			JSONEntryItemAid		aid;
			JSONEntryItemMisc		misc;
		};

		JSONEntryItem(JSONEntryItemCommon common) : common(std::move(common)) {}
		JSONEntryItem(JSONEntryItemCommon common, JSONEntryItemWeapon weapon) : common(std::move(common)), weapon(weapon) {}
		JSONEntryItem(JSONEntryItemCommon common, JSONEntryItemArmor armor) : common(std::move(common)), armor(armor) {}
		JSONEntryItem(JSONEntryItemCommon common, JSONEntryItemAid aid) : common(std::move(common)), aid(aid) {}
		JSONEntryItem(JSONEntryItemCommon common, JSONEntryItemMisc misc) : common(std::move(common)), misc(misc) {}
		JSONEntryItem(JSONEntryItemCommon common, TESForm* form) : common(std::move(common)) { common.form = form; }
	};

	class JSONEntryCategory
	{
	public:
		std::string		tag;
		SInt32			priority		= 0;

		std::string		xmltemplate;
		std::string		filename;
		std::string		texatlas;
		SInt32			systemcolor		= 0;

		std::string		category;
		std::string		name;
		std::string		icon;
		UInt32			tab				= 0;
		UInt32			count			= 0;

		JSONEntryCategory() = default;
	};

	class JSONEntryTab
	{
	public:
		std::string		tab;
		SInt32			priority		= 0;

		std::string		name;
		SInt32			tabPriority		= 0;
		UInt32			tabNew			= 0;
		std::unordered_set<std::string>	tabMisc;
		UInt32			inventory		= 0;
		UInt32			container		= 0;

		std::unordered_set<UInt32>			types;
		std::unordered_set<std::string>		categories;

		JSONEntryTab() = default;

	};

	inline std::vector<JSONEntryItem>				g_Items_JSON;
	inline std::vector<JSONEntryCategory>			g_Categories_JSON;
	inline std::vector<JSONEntryTab>				g_Tabs_JSON;

	bool AssignCategoryToItem(TESForm* form);
	void HandleSIJson(const std::filesystem::path& path);
	void FillSIMapsFromJSON();
}

namespace SI
{
	inline std::unordered_map<std::string, SIFiles::JSONEntryCategory>		g_Categories;
	inline std::unordered_map<std::string, SIFiles::JSONEntryTab>			g_Tabs;

	inline std::unordered_map<TESForm*, std::string>						g_ItemToCategory;
	inline std::unordered_map<TESForm*, std::unordered_set<std::string>>	g_ItemToFilter;

	inline std::unordered_set<std::string>									g_Keyrings;
	inline std::vector<std::string>											g_Tabline;
	inline std::vector<std::filesystem::path>								g_XMLPaths;


	void InjectTemplates();
	void KeyringRefreshPostStewie();
	std::string GetCategoryForItem(TESForm* form);
	std::string GetCategoryForItem(ContChangesEntry* entry);
	bool KeyringHideNonKeys(ContChangesEntry*);

	void __fastcall SetTileStringInjectTile(Tile* tile, ContChangesEntry* entry, MenuItemEntryList* list, eTileValue tilevalue, const char* tileText, bool propagate);
	bool __fastcall HasContainerChangesEntry(ContChangesEntry* entry);
	bool __fastcall KeyringShowCategories(Tile* tile);
	void __fastcall AddSortingCategories();
	bool __fastcall KeyringHideKeys(ContChangesEntry* entry);
	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile);
	SInt32 __fastcall CompareItemsWithTags(const TileContChangesEntryUnk* unk1, const TileContChangesEntryUnk* unk2);
	void __fastcall SetStringValueTagImage(Tile* tile, ContChangesEntry* entry, eTileValue tilevalue, char* src, char propagate);
	void __fastcall SetStringValueTagRose(Tile* tile, ContChangesEntry* entry, eTileValue tilevalue, char* src, char propagate);



	void __cdecl SetUpTabline(TileRect* tabline, int traitID, const char* strWeapon, const char* strApparel, const char* strAid, const char* strMisc, const char* strAmmo, char* zero);
	UInt32 __fastcall InventoryMenuHandleClickGetFilter(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile);
	void __fastcall InventoryMenuSetupData(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile);
	Tile* __fastcall InventoryMenuChooseTab(SInt32 key, UInt32 filter);
	void InventoryMenuSaveScrollPosition();
	void InventoryMenuRestoreScrollPosition();
	UInt8 __fastcall InventoryMenuShouldHideItem(ContChangesEntry* entry);


	struct AddRemoveEventParams
	{
		TESForm* form1;
		TESForm* form2;
	};

///	void TestFunc(TESObjectREFR* thisObj, AddRemoveEventParams* parameters)
//	{
//		Log(FormatString(parameters->form1->GetTheName()), 2);
	//	Log(FormatString(parameters->form2->GetTheName()), 2);
	//}

}

namespace SIHooks
{

	template <UInt32 retn> __declspec(naked) void IconInjectTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetTileStringInjectTile);
		static const UInt32 retnAddr = retn;
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
	
	template<UInt32 retn> __declspec(naked) void SortingInventoryMenuHook()
	{
		static const auto CompareItems = reinterpret_cast<UInt32>(SI::CompareItemsWithTags);
		static const UInt32 ContChangesEntry_GetFullName = 0x4BE2D0;
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
			call	ContChangesEntry_GetFullName
			jmp		retnAddr
		}
	}
	void SortingBarterContainerMenuHook();

	template<UInt32 retn> __declspec(naked) void IconHotkeyHUDTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetStringValueTagImage);
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		edx, [ebp - 0x24]
			call    SetStringValue
			jmp		retnAddr
		}
	}

	template<UInt32 retn> __declspec(naked) void IconHotkeyPipBoyTileSetStringValueHook() {
		static const auto SetStringValue = reinterpret_cast<UInt32>(SI::SetStringValueTagRose);
		static const UInt32 retnAddr = retn;
		static const UInt32 g_inventoryMenuSelection = 0x011D9EA8;
		__asm
		{
			mov		edx, dword ptr ds : [0x011D9EA8]
			call    SetStringValue
			jmp		retnAddr
		}
	}


	template<UInt32 retn1, UInt32 retn2> __declspec(naked) void KeyringHideKeysHook()
	{
		static const UInt32 retnAddr1 = retn1;
		static const UInt32 retnAddr2 = retn2;
		static const auto ShouldHide = reinterpret_cast<UInt32>(SI::KeyringHideKeys);
		__asm
		{
			mov		ecx, [ebp + 0x8] // a1
			call	ShouldHide
			test	al, al
			jz		shouldnot
			jmp		retnAddr1
		shouldnot :
			jmp		retnAddr2
		}
	}
	
	template<UInt32 retn> __declspec(naked) void KeyringHideKeysShowCategoriesHook()
	{
		static const UInt32 retnAddr = retn;
		static const auto HasContainerEntry = reinterpret_cast<UInt32>(SI::HasContainerChangesEntry);
		static const auto ShowCategories = reinterpret_cast<UInt32>(SI::KeyringShowCategories);
		__asm
		{
			mov		ecx, [ebp + 0x8] // a1
			call	HasContainerEntry
			test	al, al
			jz		hasnot

			mov		ecx, [ebp + 0xC]
			call	ShowCategories
			mov		esp, ebp
			pop		ebp
			ret
		hasnot :
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void KeyringHideNonKeysHook()
	{
		static const UInt32 retnAddr = retn;
		static const auto HideNonKeysAndGetTile = reinterpret_cast<UInt32>(SI::HideNonKeysGetTile);
		__asm
		{
			mov		edx, [ebp + 0xC]
			call	HideNonKeysAndGetTile
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void KeyringAddCategoriesHook()
	{
		static const UInt32 retnAddr = retn;
		static const auto AddCategories = reinterpret_cast<UInt32>(SI::AddSortingCategories);
		__asm
		{
			call	AddCategories
			jmp		retnAddr
		}
	}
	
	template<UInt32 retn> __declspec(naked) void ContainerEntryListBoxFilterHookPre()
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

	template<UInt32 retn> __declspec(naked) void ContainerEntryListBoxFilterHookPost()
	{
		// fix stack
		static const UInt32 retnAddr = retn;
		__asm
		{
			add		esp, 4
			test	al, al
			jz		wah
			mov		[ebp - 0x24], 1
			jmp		retnAddr
		wah :
			mov		[ebp - 0x24], 0
			jmp		retnAddr
		}
	}
	template<UInt32 retn> __declspec(naked) void KeyringEnableEquipDropHook()
	{
		static const UInt32 retnAddr = retn;
		__asm
		{
			mov		eax, 0
			jmp		retnAddr
		}
	}

	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, enum eTileValue tilevalue, signed int a1);
	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, enum eTileValue tilevalue, char* string, int propagate);

}