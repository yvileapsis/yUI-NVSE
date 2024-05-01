#pragma once
#include "Menu.hpp"

class LevelUpMenu : public Menu
{
public:
	LevelUpMenu();
	~LevelUpMenu();

	enum Buttons
	{
		kLevelUp_Reset = 0x6,
		kLevelUp_Done = 0x7,
		kLevelUp_Back = 0x8,
		kLevelUp_Skill = 0xB,
		kLevelUp_Perk = 0xC,
		kLevelUp_Minus = 0xD,
		kLevelUp_Plus = 0xE,
	};

	enum Pages
	{
		kSkillSelection = 0,
		kPerkSelection = 1,
		kCloseMenu = 999  // (any value >= 2 could work)
	};

	UInt32						currentPage; // 0 for skills, 1 for perks
	TileText*					tileTitle;
	TileImage*					tileSkills;
	TileImage*					tilePerks;
	TileImage*					tileSelectionIcon;
	TileText*					tileSelectionText;
	TileText*					tilePointCounter;
	TileImage*					tileBtnReset;
	TileImage*					tileBtnContinue;
	TileImage*					tileBtnBack;
	TileImage*					tileStatsIconBadge;
	UInt32						numAssignedSkillPoints;
	UInt32						numAssignedPerks;
	UInt32						numSkillPointsToAssign;
	UInt32						numPerksToAssign;
	ListBox<ActorValueCode>		skillListBox;
	ListBox<BGSPerk>			perkListBox;
	TList<BGSPerk>				availablePerks; // perks to show in the perk listBox

	void SetCurrentPage(Pages newPage) { ThisCall(0x785830, this, newPage); }

	static LevelUpMenu* GetSingleton() { return *reinterpret_cast<LevelUpMenu**>(0x11D9FDC); }
};
static_assert(sizeof(LevelUpMenu) == 0xCC);