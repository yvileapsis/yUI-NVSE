#pragma once
#include "Menu.hpp"
#include "ListBox.hpp"
#include <ActorValue.hpp>

struct PerkRank
{
	BGSPerk*	perk;
	UInt8		rank;
	UInt8		pad05[3];
};

// 2A4
class StatsMenu : public Menu			// 1003
{
public:
	StatsMenu();
	~StatsMenu();

	enum EnumTabs
	{
		kStatusEffects = 0,
		kSPECIAL = 1,
		kSkills = 2,
		kPerks = 3,
		kGeneral = 4,
	};


	enum EnumMiscStatCode
	{
		kMiscStat_QuestsCompleted,
		kMiscStat_LocationsDiscovered,
		kMiscStat_PeopleKilled,
		kMiscStat_CreaturesKilled,
		kMiscStat_LocksPicked,
		kMiscStat_ComputersHacked,
		kMiscStat_StimpaksTaken,
		kMiscStat_RadXTaken,
		kMiscStat_RadAwayTaken,
		kMiscStat_ChemsTaken,
		kMiscStat_TimesAddicted,
		kMiscStat_MinesDisarmed,
		kMiscStat_SpeechSuccesses,
		kMiscStat_PocketsPicked,
		kMiscStat_PantsExploded,
		kMiscStat_BooksRead,
		kMiscStat_HealthFromStimpaks,
		kMiscStat_WeaponsCreated,
		kMiscStat_HealthFromFood,
		kMiscStat_WaterConsumed,
		kMiscStat_SandmanKills,
		kMiscStat_ParalyzingPunches,
		kMiscStat_RobotsDisabled,
		kMiscStat_TimesSlept,
		kMiscStat_CorpsesEaten,
		kMiscStat_MysteriousStrangerVisits,
		kMiscStat_DoctorBagsUsed,
		kMiscStat_ChallengesCompleted,
		kMiscStat_MissFortunateOccurrences,
		kMiscStat_Disintegrations,
		kMiscStat_HaveLimbsCrippled,
		kMiscStat_SpeechFailures,
		kMiscStat_ItemsCrafted,
		kMiscStat_WeaponModifications,
		kMiscStat_ItemsRepaired,
		kMiscStat_TotalThingsKilled,
		kMiscStat_DismemberedLimbs,
		kMiscStat_CaravanGamesWon,
		kMiscStat_CaravanGamesLost,
		kMiscStat_BarterAmountTraded,
		kMiscStat_RouletteGamesPlayed,
		kMiscStat_BlackjackGamesPlayed,
		kMiscStat_SlotsGamesPlayed,
		kMiscStat_Max
	};

	struct AlchItemData
	{
		AlchemyItem*	alchItem;
		TileImage*		tileImg;
		void*			dataPtr;
		bool			(*callback)(void* arg);
	};

	class StatusDataList
	{
	public:
		struct StatusData
		{
			UInt32 magnitude;
			UInt32 unk04;
			UInt32 duration;
			UInt32 unk0C;
			UInt32 avCode;
		};

		BSSimpleList<StatusData*> statusDataList;
	};

	class NamedEffectsList
	{
	public:
		BSSimpleList<void*> list04;
	};

	struct StatusEffect
	{
		SpellItem*			spellItem;
		StatusDataList		statusDataList;
		NamedEffectsList	namedEffectsList;
	};

	AlchItemData					alchItemData[4];	// 028	0: Stimpak, 1: Rad-X, 2: RadAway, 3: Doctor's Bag
	BSSimpleList<EffectSetting>		effectList;			// 068
	StatusEffect*					radEffects;
	StatusEffect*					h20effects;
	StatusEffect*					hungerEffects;
	StatusEffect*					sleepEffects;
	BSSimpleList<StatusEffect>		statusEffList;		// 084
	TileImage*						tile090;			// 090
	TileImage*						tile094;			// 094
	TileImage*						tile098;			// 098
	TileImage*						tile09C;			// 09C
	TileImage*						tile0A0;			// 0A0
	TileImage*						tile0A4;			// 0A4
	TileImage*						tile0A8;			// 0A8
	TileImage*						tile0AC;			// 0AC
	TileImage*						tile0B0;			// 0B0
	TileImage*						tile0B4;			// 0B4
	TileImage*						tile0B8;			// 0B8
	TileImage*						tile0BC;			// 0BC
	TileImage*						tile0C0;			// 0C0
	TileRect*						tile0C4;			// 0C4
	TileImage*						tile0C8;			// 0C8
	TileRect*						tile0CC;			// 0CC
	TileImage*						tile0D0;			// 0D0
	TileRect*						tile0D4;			// 0D4
	TileImage*						tile0D8;			// 0D8
	TileRect*						tile0DC;			// 0DC
	TileImage*						tile0E0;			// 0E0
	TileRect*						tile0E4;			// 0E4
	TileImage*						tile0E8;			// 0E8
	TileRect*						tile0EC;			// 0EC
	TileImage*						tile0F0;			// 0F0
	TileImage*						tile0F4;			// 0F4
	TileImage*						tile0F8;			// 0F8
	TileText*						tile0FC;			// 0FC
	TileImage*						tile100;			// 100
	TileImage*						tile104;			// 104
	TileImage*						tile108;			// 108
	TileImage*						tile10C;			// 10C
	TileImage*						tile110;			// 110
	TileImage*						tile114;			// 114
	TileImage*						tile118;			// 118
	TileText*						tile11C;			// 11C
	TileImage*						tile120;			// 120
	TileText*						tile124;			// 124
	TileText*						tile128;			// 128
	TileText*						tile12C;			// 12C
	TileRect*						tile130;			// 130
	TileRect*						tile134;			// 134
	TileRect*						tile138;			// 138
	TileRect*						tile13C;			// 13C
	TileText*						tile140;			// 140
	TileText*						tile144;			// 144
	TileImage*						tile148;			// 148
	TileImage*						tile14C;			// 14C
	TileText*						tile150;			// 150
	TileImage*						tile154;			// 154
	TileImage*						tile158;			// 158
	TileText*						tile15C;			// 15C
	TileImage*						tile160;			// 160
	TileImage*						tile164;			// 164
	TileImage*						tile168;			// 168
	TileImage*						tile16C;			// 16C
	TileImage*						tile170;			// 170
	TileImage*						tile174;			// 174
	TileImage*						tile178;			// 178
	TileImage*						tile17C;			// 17C
	ListBox<EnumActorValueCode>		kStatsList;			// 180
	ListBox<EnumActorValueCode>		kSkillList;			// 1B0
	ListBox<PerkRank>				kPerkRankList;		// 1E0
	ListBox<EnumMiscStatCode>		miscStatIDList;		// 210
	ListBox<StatusEffect>			statusEffListBox;	// 240
	ListBox<TESReputation>			reputationList;		// 270
	UInt8							isInHealLimbSelectMode; // 2A0
	UInt8							byte2A1[3];			  // 2A1

	bool ScrollNotes(MenuSpecialKeyboardInputCode direction, bool scaleScroll);

	static StatsMenu* GetSingleton() { return *reinterpret_cast<StatsMenu**>(0x11DACE0); };
};
static_assert(sizeof(StatsMenu) == 0x2A4);