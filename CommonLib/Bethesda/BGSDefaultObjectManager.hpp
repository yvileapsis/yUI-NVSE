#pragma once
#include "TESForm.hpp"

const char kDefaultObjectNames[34][28] = {	// 0x0118C360 is an array of struct: { char * Name, UInt8 kFormType , UInt8 pad[3] }
	  "Stimpack",
	  "SuperStimpack",
	  "RadX",
	  "RadAway",
	  "Morphine",
	  "Perk Paralysis",
	  "Player Faction",
	  "Mysterious Stranger NPC",
	  "Mysterious Stranger Faction",
	  "Default Music",
	  "Battle Music",
	  "Death Music",
	  "Success Music",
	  "Level Up Music",
	  "Player Voice (Male)",
	  "Player Voice (Male Child)",
	  "Player Voice (Female)",
	  "Player Voice (Female Child)",
	  "Eat Package Default Food",
	  "Every Actor Ability",
	  "Drug Wears Off Image Space",
	  "Doctor's Bag",
	  "Miss Fortune NPC",
	  "Miss Fortune Faction",
	  "Meltdown Explosion",
	  "Unarmed Forward PA",
	  "Unarmed Backward PA",
	  "Unarmed Left PA",
	  "Unarmed Right PA",
	  "Unarmed Crouch PA",
	  "Unarmed Counter PA",
	  "Spotter Effect",
	  "Item Detected Effect",
	  "Cateye Mobile Effect (NYI)"
};

// 0xA0
class BGSDefaultObjectManager : public TESForm
{
public:
	BGSDefaultObjectManager();
	~BGSDefaultObjectManager();

	static BGSDefaultObjectManager* GetSingleton();

	enum {
		kDefaultObject_Max = 34,
	};

	typedef TESForm* FormArray[kDefaultObject_Max];

	struct FormStruct 
	{
		TESForm*	pkStimpak;
		TESForm*	pkSuperStimpak;
		TESForm*	pkRadX;
		TESForm*	pkRadAway;
		TESForm*	pkMorphine;
		TESForm*	pkPerkParalysis;
		TESForm*	pkPlayerFaction;
		TESForm*	pkMysteriousStranger;
		TESForm*	pkMysteriousStrangerFaction;
		TESForm*	pkDefaultMusic;
		TESForm*	pkBattleMusic;
		TESForm*	pkDefaultDeath;
		TESForm*	pkSuccessMusic;
		TESForm*	pkLevelUpMusic;
		TESForm*	pkPlayerVoiceMale;
		TESForm*	pkPlayerVoiceMaleChild;
		TESForm*	pkPlayerVoiceFemale;
		TESForm*	pkPlayerVoiceFemaleChild;
		TESForm*	pkEatPackageDefaultFood;
		TESForm*	pkEveryActorAbility;
		TESForm*	pkDrugWearOffImageSpace;
		// FNV
		TESForm*	pkDoctorsBag;
		TESForm*	pkMissFortuneNPC;
		TESForm*	pkMissFortuneFaction;
		TESForm*	pkMeltdownExplosion;
		TESForm*	pkUnarmedForwardPA;
		TESForm*	pkUnarmedBackwardPA;
		TESForm*	pkUnarmedLeftPA;
		TESForm*	pkUnarmedRightPA;
		TESForm*	pkUnarmedCrouchPA;
		TESForm*	pkUnarmedCounterPA;
		TESForm*	pkSpotterEffect;
		TESForm*	pkItemDetectedEffect;
		TESForm*	pkCateyeMobileEffectNYI;
	};

	union DefaultObjects {
		FormStruct	asStruct;
		FormArray	asArray;
	};

	DefaultObjects	kDefaultObjects;	// 018
};
static_assert(sizeof(BGSDefaultObjectManager) == 0xA0);