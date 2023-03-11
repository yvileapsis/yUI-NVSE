#pragma once
#include <Menus.h>

namespace SortingIcons
{
	inline int enable = 0;

	inline int logLevel = Log::kMessage;

	inline int bSort			= 1;
	inline int bIcons			= 1;
	inline int bHotkeys			= 1;
	inline int bCategories		= 1;
	inline int bPrompt			= 1;
	
	class Item
	{
	public:
		class Common
		{
		public:
			std::unordered_set<UInt32>		formIDs;

			std::unordered_set<UInt8>			formType;
			std::optional<bool>					questItem;
			std::optional<bool>					miscComponent;
			std::optional<bool>					miscProduct;

			__forceinline bool Satisfies(TESForm* form) const;
		};

		class Weapon
		{
		public:
			std::unordered_set<UInt32>			skill;
			std::unordered_set<UInt32>			type;
			std::unordered_set<UInt32>			handgrip;
			std::unordered_set<UInt32> 			attackAnim;
			std::unordered_set<UInt32> 			reloadAnim;

			std::optional<bool>	 			isAutomatic;
			std::optional<bool>	 			hasScope;
			std::optional<bool>	 			ignoresDTDR;

			std::optional<UInt32> 			clipRoundsMin;
			std::optional<UInt32> 			clipRoundsMax;
			std::optional<UInt32> 			numProjectilesMin;
			std::optional<UInt32> 			numProjectilesMax;

			std::unordered_set<UInt32>		soundLevel;

			std::unordered_set<UInt32>		ammoIDs;

			__forceinline bool Satisfies(TESObjectWEAP* weapon) const;
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

			__forceinline bool Satisfies(TESObjectARMO* armor) const;
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

			__forceinline bool Satisfies(AlchemyItem* aid) const;
		};

		std::string		tag;
		SInt32			priority = 0;

		Common			common{};
		Weapon			weapon{};
		Armor			armor{};
		Aid				aid{};

		bool Satisfies(TESForm* form) const;
	};


	class Category
	{
	public:
		std::string				tag;
		SInt32					priority = 0;

		std::string				xmltemplate;
		std::string				filename;
		std::string				texatlas;
		std::optional<SInt32>	font;
		std::optional<SInt32>	systemcolor;

		bool IsValid() const;

		static Category* Get(const std::string tag);
		Category* Set(const std::string tag);

		static Category* Satisfies(TESForm* form);
		static Category* Get(TESForm* form);
		Category* Set(TESForm* form);
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

		bool Satisfies(TESForm* form) const;
	};

	inline Category* categoryDefault = nullptr;

	inline std::vector<std::unique_ptr<Item>>		g_Items;
	inline std::vector<std::unique_ptr<Category>>	g_Categories;
	inline std::vector<std::unique_ptr<Tab>>		g_Tabs;

	inline std::unordered_map<std::string, Tab*>		g_StringToTabs;

	inline std::vector<Tab*>							g_Keyrings;
	inline std::vector<Tab*>							g_Tabline;
	inline std::vector<std::filesystem::path>							g_XMLPaths;
}

namespace SortingIcons::Commands
{
	void Register();
}

namespace SortingIcons::Files
{
	void HandleJSON(const std::filesystem::path& path);
	void HandleXML(const std::filesystem::path& path);
}

namespace SortingIcons::Icons
{

	void InjectTemplates();
	void InjectIconTile(const Category* category_, Tile* tile);
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