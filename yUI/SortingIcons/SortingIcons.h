#pragma once
#include <Menus.h>
#include <InventoryChanges.h>

namespace SortingIcons
{
	inline int enable = 0;

	inline int logLevel = Log::kNone;

	inline int bSort			= 1;
	inline int bIcons			= 1;
	inline int bHotkeys			= 1;
	inline int bCategories		= 1;
	
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
			UInt32			skill			= 0;
			UInt32			type			= 0;
			UInt32			handgrip		= 0;
			UInt32 			attackAnim		= 0;
			UInt32 			reloadAnim		= 0;
			UInt32 			isAutomatic		= 0;
			UInt32 			hasScope		= 0;
			UInt32 			ignoresDTDR		= 0;
			UInt32 			clipRounds		= 0;
			UInt32 			numProjectiles	= 0;
			UInt32 			soundLevel		= 0;

			TESForm*		ammo			= nullptr;

			Weapon() = default;
		};

		class Armor
		{
		public:
			UInt32			slotsMaskWL		= 0;
			UInt32 			slotsMaskBL		= 0;

			UInt16 			armorClass		= 0;
			SInt8 			powerArmor		= 0;
			SInt8 			hasBackpack		= 0;

			Float32 		dt				= 0;
			UInt16 			dr				= 0;
			UInt16 			changesAV		= 0;

			Armor() = default;
		};

		class Aid
		{
		public:
			UInt8  			restoresAV		= 0;
			UInt8  			damagesAV		= 0;
			UInt8  			isAddictive		= 0;
			UInt8  			isWater			= 0;
			UInt8  			isFood			= 0;
			UInt8  			isMedicine		= 0;
			UInt8  			isPoisonous		= 0;

			Aid() = default;
		};

		Common			common{};
		Weapon			weapon{};
		Armor			armor{};
		Aid				aid{};

		Item(Common common) : common(std::move(common)) {}
		Item(Common common, Weapon weapon) : common(std::move(common)), weapon(weapon) {}
		Item(Common common, Armor armor) : common(std::move(common)), armor(armor) {}
		Item(Common common, Aid aid) : common(std::move(common)), aid(aid) {}
	};

	class Category
	{
	public:
		std::string		tag;
		SInt32			priority = 0;

		std::string		xmltemplate;
		std::string		filename;
		std::string		texatlas;
		SInt32			systemcolor = 0;

		Category() = default;
	};

	class Tab
	{
	public:

		std::string		tag;
		SInt32			priority			= 0;

		std::string		name;

		std::unordered_set<UInt32>			types;
		std::unordered_set<std::string>		categories;

		std::string		icon;
		
		UInt32			keyring				= 0;
		UInt32			inventory			= 0;
		UInt32			container			= 0;

		SInt32			tabPriority			= 0;
		UInt32			tabNew				= 0;
		UInt32			tabMisc				= 0;

		Tab() = default;
	};

	typedef std::shared_ptr<Item> ItemPtr;
	typedef std::shared_ptr<Category> CategoryPtr;
	typedef std::shared_ptr<Tab> TabPtr;

	inline CategoryPtr categoryDefault;

	inline std::vector<ItemPtr>		g_Items;
	inline std::vector<CategoryPtr>	g_Categories;
	inline std::vector<TabPtr>		g_Tabs;

	inline std::unordered_map<std::string, CategoryPtr>	g_StringToCategory;
	inline std::unordered_map<std::string, TabPtr>		g_StringToTabs;

	inline std::vector<TabPtr>							g_Keyrings;
	inline std::vector<TabPtr>							g_Tabline;
	inline std::vector<std::filesystem::path>							g_XMLPaths;
}

namespace SortingIcons::Commands
{
	void Register();
}

namespace SortingIcons::Categories
{
	CategoryPtr& ItemGetCategory(TESForm* form);
	CategoryPtr& ItemGetCategory(const InventoryChanges* form);
	void ItemSetCategory(TESForm* form, const CategoryPtr& category);
	void ItemSetCategory(const InventoryChanges* entry, const CategoryPtr& category);
}

namespace SortingIcons::Files
{
	void HandleJSON(const std::filesystem::path& path);
	void HandleXML(const std::filesystem::path& path);
}

namespace SortingIcons::Icons
{

	void InjectTemplates();
	void InjectIconTile(const CategoryPtr& category_, Tile* tile);
}

namespace SortingIcons::Patch
{
	void AlterSorting(const bool bEnable);
	void AddIcons(const bool bEnable);
	void ReplaceHotkeyIcons(const bool bEnable);
	void AddKeyrings(const bool bEnable);
	void AddTabs(const bool bEnable);
	void AddPromptIcon(const bool bEnable);

	void AddKeyrings2(const bool bEnable);
}