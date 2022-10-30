#pragma once
#include <Menus.h>
#include <GameExtraData.h>
#include <filesystem>
#include <unordered_set>

namespace SortingIcons
{
	inline int g_FixIndefiniteSorting = 1;

	inline int enable = 0;

	inline int logLevel = kToNone;

	inline int bSort			= 1;
	inline int bIcons			= 1;
	inline int bHotkeys			= 1;
	inline int bCategories		= 1;
	inline int bJustInTime		= 1;

	class Item
	{
	public:
		class Common
		{
		public:
			std::string		tag;
			SInt32			priority		= 0;
			TESForm*		form			= nullptr;
			UInt32			formType		= 0;
			UInt8			questItem		= 0;
			UInt8			miscComponent	= 0;
			UInt8			miscProduct		= 0;

			Common() = default;
		};

		class Weapon
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

			TESForm* ammo;

			Weapon() = default;
		};

		class Armor
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

			Armor() = default;
		};

		class Aid
		{
		public:
			UInt8  			restoresAV;
			UInt8  			damagesAV;
			UInt8  			isAddictive;
			UInt8  			isWater;
			UInt8  			isFood;
			UInt8  			isMedicine;
			UInt8  			isPoisonous;

			Aid() = default;
		};

		class Misc
		{
		public:
			Misc() = default;
		};

		Common			common{};
		union {
			Weapon		weapon;
			Armor		armor;
			Aid			aid;
			Misc		misc{};
		};

		Item(Common common) : common(std::move(common)) {}
		Item(Common common, Weapon weapon) : common(std::move(common)), weapon(weapon) {}
		Item(Common common, Armor armor) : common(std::move(common)), armor(armor) {}
		Item(Common common, Aid aid) : common(std::move(common)), aid(aid) {}
		Item(Common common, Misc misc) : common(std::move(common)), misc(misc) {}
	};

	class Category
	{
	public:
		std::string		tag;
		SInt32			priority	= 0;

		std::string		xmltemplate;
		std::string		filename;
		std::string		texatlas;
		SInt32			systemcolor	= 0;

		std::string		category;
		std::string		name;
		std::string		icon;
		UInt32			tab			= 0;
		UInt32			count		= 0;

		Category() = default;
	};

	class Tab
	{
	public:
		std::string		tab;
		SInt32			priority			= 0;

		std::string		name;
		SInt32			tabPriority			= 0;
		UInt32			tabNew				= 0;
		std::unordered_set<std::string>	tabMisc;
		UInt32			inventory			= 0;
		UInt32			container			= 0;

		std::unordered_set<UInt32>			types;
		std::unordered_set<std::string>		categories;

		Tab() = default;

	};

	inline std::vector<Item>				g_Items;
	inline std::vector<Category>			g_Categories;
	inline std::vector<Tab>					g_Tabs;

	inline std::unordered_map<std::string, Category>						g_StringToCategory;
	inline std::unordered_map<std::string, Tab>								g_StringToTabs;

	inline std::unordered_set<std::string>									g_Keyrings;
	inline std::vector<std::string>											g_Tabline;
	inline std::vector<std::filesystem::path>								g_XMLPaths;
}

namespace SortingIcons::Commands
{
	void Register();
}

namespace SortingIcons::Patches
{
	void AlterSorting(const bool bEnable);
	void AddIcons(const bool bEnable);
	void ReplaceHotkeyIcons(const bool bEnable);
	void AddKeyrings(const bool bEnable);
	void AddTabs(const bool bEnable);
	void AddPromptIcon();
}

namespace SortingIcons::Files
{
	bool AssignCategoryToItem(TESForm* form);
	bool AssignFiltersToItem(TESForm* form);
	void HandleJson(const std::filesystem::path& path);
	void HandleXML(const std::filesystem::path& path);
}

namespace SortingIcons::Sorting
{
	void KeyringRefreshPostStewie();
	std::string GetCategoryForItem(TESForm* form);
	std::string GetCategoryForItem(InventoryChanges* entry);
	bool IsTagForItem(TESForm* form);
	bool IsTagForItem(InventoryChanges* entry);

	SInt32 __fastcall CompareItemsWithTags(const TileInventoryChangesUnk* unk1, const TileInventoryChangesUnk* unk2);

	void __fastcall HideNonKeysGetTile(InventoryMenu* invmenu, Tile* tile);
	void __fastcall AddSortingCategories();

	bool __fastcall KeyringHideKeys(InventoryChanges* entry);
	UInt8 __fastcall InventoryMenuShouldHideItem(InventoryChanges* entry);
	void __fastcall KeyringEnableCancelHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, signed int a1);
	void __fastcall KeyringPipBoyIconHook(Tile* tile, void* dummyEDX, eTileValue tilevalue, char* string, int propagate);
	void SetUpTabline(TileRect* tabline, int traitID, const char* strWeapon, const char* strApparel, const char* strAid,
		const char* strMisc, const char* strAmmo, char* zero);

	Tile* __fastcall InventoryMenuChooseTab(SInt32 key, UInt32 filter);
	void InventoryMenuSaveScrollPosition();
	UInt32 __fastcall InventoryMenuHandleClickGetFilter(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile);
	void __fastcall InventoryMenuSetupData(InventoryMenu* menu, SInt32 tileID, Tile* clickedTile);
	void InventoryMenuRestoreScrollPosition();


	bool __fastcall HasContainerChangesEntry(InventoryChanges* entry);
	bool __fastcall KeyringShowCategories(Tile* tile);
}

namespace SortingIcons::Icons
{
	void InjectTemplates();
	void InjectIconTile(const Category& category, MenuItemEntryList* list, Tile* tile, InventoryChanges* entry);
	void __fastcall SetTileStringInjectTile(Tile* tile, InventoryChanges* entry, MenuItemEntryList* list, const eTileValue tilevalue, const char* tileText, bool propagate);
	void __fastcall SetStringValueTagImage(Tile* tile, InventoryChanges* entry, eTileValue tilevalue, char* src, char propagate);
	void __fastcall SetStringValueTagRose(Tile* tile, InventoryChanges* entry, eTileValue tilevalue, char* src, char propagate);
	void __fastcall Tile__PropagateIntValue(Tile* tile, void* dummyedx, UInt32 a2, signed int a3);
}
