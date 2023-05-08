#pragma once
#include <Menu.h>

namespace SortingIcons
{
	class Tab;
	inline int enable = 0;

	inline int logLevel = Log::kMessage;

	inline int bSort			= 1;
	inline int bIcons			= 1;
	inline int bHotkeys			= 1;
	inline int bCategories		= 1;
	inline int bPrompt			= 1;

	namespace Files
	{
		class JSON;
	}

	class Object
	{
	public:
		std::string		tag;
		SInt32			priority = 0;

		Object(const Files::JSON& elem);

		virtual bool IsValid() const { return true; };

		static signed char Compare(const Object* lhs, const Object* rhs)
		{
			if (!lhs) return -1;
			if (!rhs) return 1;
			return (lhs->tag <=> rhs->tag)._Value;
		}
	};

	class Item : public Object
	{
		bool								forms;
		std::unordered_set<UInt32>			formIDs;

		std::unordered_set<UInt8>			formType;
		std::optional<bool>					questItem;
		std::optional<bool>					miscComponent;
		std::optional<bool>					miscProduct;

	public:
		Item(const Files::JSON& elem);

		bool IsValid() const override;
		virtual bool Satisfies(TESForm* form) const;

		static Item* Set(TESForm* form, Item* item);
		static Item* Get(TESForm* form);
	};

	class Weapon : public Item
	{
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

	public:
		Weapon(const Files::JSON& elem);

		bool Satisfies(TESForm* form) const override;
	};

	class Armor : public Item
	{
		UInt32			slotsMaskWL = 0;
		UInt32 			slotsMaskBL = 0;

		UInt16 			armorClass = 0;
		SInt8 			powerArmor = 0;
		SInt8 			hasBackpack = 0;

		Float32 		dt = 0;
		UInt16 			dr = 0;
		UInt16 			changesAV = 0;

	public:
		Armor(const Files::JSON& elem);

		bool Satisfies(TESForm* form) const override;
	};

	class Aid : public Item
	{
		UInt8  			restoresAV = 0;
		UInt8  			damagesAV = 0;
		UInt8  			isAddictive = 0;
		UInt8  			isWater = 0;
		UInt8  			isFood = 0;
		UInt8  			isMedicine = 0;
		UInt8  			isPoisonous = 0;

	public:
		Aid(const Files::JSON& elem);

		bool Satisfies(TESForm* form) const override;
	};

	class Category : public Object
	{
	public:
		std::string		name;

		std::unordered_set<UInt32>			types;
		std::unordered_set<std::string>		categories;

		UInt32			tabNew = 0;
		UInt32			tabMisc = 0;
	public:
		Category(const Files::JSON& elem);

		virtual bool IsKey() { return false; }
		virtual bool IsInventory() { return false; }
		bool SatisfiesForm(TESForm* form) const;
		bool SatisfiesTag(std::string tag) const;
		virtual bool Satisfies(TESForm* form) const;
	};

	class Tab : public Category
	{
	public:
		UInt32			inventory			= 0;
		UInt32			container			= 0;

		SInt32			tabPriority			= 0;

	public:
		Tab(const Files::JSON& elem);

		bool IsInventory() override { return inventory; }
	};

	class Keyring : public Category
	{
	public:
		std::string		icon;
		UInt32			keyring = 0;

	public:
		Keyring(const Files::JSON& elem);

		bool IsKey() override { return true; }

		static Keyring* Set(Tile* tile, Keyring* item);
		static Keyring* Get(Tile* tile);
	};

	class Icon : public Object
	{
	public:

		std::string				xmltemplate;
		std::string				filename;
		std::string				texatlas;
		std::optional<SInt32>	font;
		std::optional<SInt32>	systemcolor;

		Icon(const Files::JSON& elem);

		//		bool IsValid() const override;
		//		bool Satisfies(TESForm* form) const override;

		static Icon* Set(Object* object, Icon* icon);
		static Icon* Get(Object* object);

		static Icon* Get(Tab* tab);
		Icon* Set(Tab* tab);

	};

	inline Icon* categoryDefault = nullptr;

	inline std::vector<std::unique_ptr<Item>>		g_Items;
	inline std::vector<std::unique_ptr<Category>>	g_Categories;
	inline std::vector<std::unique_ptr<Icon>>		g_Icons;

	inline std::unordered_map<std::string, Tab*>		g_StringToTabs;

	inline std::vector<Keyring*>						g_Keyrings;
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
	void InjectIconTile(const Icon* category_, Tile* tile);
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