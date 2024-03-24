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

class BGSDefaultObjectManager : public TESForm {
public:
	BGSDefaultObjectManager();
	~BGSDefaultObjectManager();

	static BGSDefaultObjectManager* GetSingleton();

	enum {
		kDefaultObject_Max = 34,
	};

	typedef TESForm* FormArray[kDefaultObject_Max];

	struct FormStruct {
		TESForm* Stimpak;
		TESForm* SuperStimpak;
		TESForm* RadX;
		TESForm* RadAway;
		TESForm* Morphine;
		TESForm* PerkParalysis;
		TESForm* PlayerFaction;
		TESForm* MysteriousStranger;
		TESForm* MysteriousStrangerFaction;
		TESForm* DefaultMusic;
		TESForm* BattleMusic;
		TESForm* DefaultDeath;
		TESForm* SuccessMusic;
		TESForm* LevelUpMusic;
		TESForm* PlayerVoiceMale;
		TESForm* PlayerVoiceMaleChild;
		TESForm* PlayerVoiceFemale;
		TESForm* PlayerVoiceFemaleChild;
		TESForm* EatPackageDefaultFood;
		TESForm* EveryActorAbility;
		TESForm* DrugWearOffImageSpace;
		// FNV
		TESForm* DoctorsBag;
		TESForm* MissFortuneNPC;
		TESForm* MissFortuneFaction;
		TESForm* MeltdownExplosion;
		TESForm* UnarmedForwardPA;
		TESForm* UnarmedBackwardPA;
		TESForm* UnarmedLeftPA;
		TESForm* UnarmedRightPA;
		TESForm* UnarmedCrouchPA;
		TESForm* UnarmedCounterPA;
		TESForm* SpotterEffect;
		TESForm* ItemDetectedEffect;
		TESForm* CateyeMobileEffectNYI;
	};

	union DefaultObjects {
		FormStruct	asStruct;
		FormArray	asArray;
	};

	DefaultObjects	defaultObjects;	// 018
};

ASSERT_SIZE(BGSDefaultObjectManager, 0xA0);