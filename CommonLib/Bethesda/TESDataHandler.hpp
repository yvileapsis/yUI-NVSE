#pragma once
#include "TESFile.hpp"
#include "NiTArray.hpp"
#include "NiTPointerList.hpp"

class BGSAcousticSpace;
class BGSAddonNode;
class BGSBodyPartData;
class BGSCameraShot;
class BGSDebris;
class BGSDehydrationStage;
class BGSEncounterZone;
class BGSExplosion;
class BGSHeadPart;
class BGSHungerStage;
class BGSImpactData;
class BGSImpactDataSet;
class BGSLightingTemplate;
class BGSListForm;
class BGSMenuIcon;
class BGSMessage;
class BGSMusicType;
class BGSNote;
class BGSPerk;
class BGSProjectile;
class BGSRadiationStage;
class BGSRagdoll;
class BGSSleepDeprivationStage;
class BGSVoiceType;
class EnchantmentItem;
class MediaLocationController;
class MediaSet;
class Script;
class SpellItem;
class TESAmmoEffect;
class TESBoundObject;
class TESCaravanDeck;
class TESCasino;
class TESChallenge;
class TESClass;
class TESClimate;
class TESCombatStyle;
class TESEffectShader;
class TESEyes;
class TESFaction;
class TESGlobal;
class TESHair;
class TESImageSpace;
class TESImageSpaceModifier;
class TESLandTexture;
class TESLoadScreen;
class TESLoadScreenType;
class TESObjectANIO;
class TESObjectCELL;
class TESObjectList;
class TESPackage;
class TESQuest;
class TESRace;
class TESRecipe;
class TESRecipeCategory;
class TESRegionList;
class TESRegionDataManager;
class TESReputation;
class TESSound;
class TESTopic;
class TESTopicInfo;
class TESWaterForm;
class TESWeather;
class TESWorldSpace;

struct ModList 
{
	BSSimpleList<TESFile*>	kModInfos;
	UInt32					uiLoadedModCount;
	TESFile*				pLoadedMods[0xFF];
};
static_assert(sizeof(ModList) == 0x408);

// 5B8
class TESDataHandler {
public:
	TESDataHandler();
	~TESDataHandler();


	UInt32									unk00;					// 000
	TESObjectList*							pObjects;				// 004
	BSSimpleList<TESPackage*>				kPackages;				// 008
	BSSimpleList<TESWorldSpace*>			kWorldSpaces;			// 010
	BSSimpleList<TESClimate*>				kClimates;				// 019
	BSSimpleList<TESImageSpace*>			kImageSpaces;			// 020
	BSSimpleList<TESImageSpaceModifier*>	kImageSpaceMods;		// 028
	BSSimpleList<TESWeather*>				kWeathers;				// 030
	BSSimpleList<EnchantmentItem*>			kEnchantmentItems;		// 038
	BSSimpleList<SpellItem*>				kSpellItems;			// 040
	BSSimpleList<BGSHeadPart*>				kHeadParts;				// 048
	BSSimpleList<TESHair*>					kHairs;					// 050
	BSSimpleList<TESEyes*>					kEyes;					// 058
	BSSimpleList<TESRace*>					kRaces;					// 060
	BSSimpleList<BGSEncounterZone*>			kEncounterZones;		// 068
	BSSimpleList<TESLandTexture*>			kLandTextures;			// 070
	BSSimpleList<BGSCameraShot*>			kCameraShots;			// 078
	BSSimpleList<TESClass*>					kClasses;				// 080
	BSSimpleList<TESFaction*>				kFactions;				// 088
	BSSimpleList<TESReputation*>			kReputations;			// 090
	BSSimpleList<TESChallenge*>				kChallenges;			// 098
	BSSimpleList<TESRecipe*>				kRecipes;				// 0A0
	BSSimpleList<TESRecipeCategory*>		kRecipeCategories;		// 0A8
	BSSimpleList<TESAmmoEffect*>			kAmmoEffects;			// 0B0
	BSSimpleList<TESCasino*>				kCasinos;				// 0B8
	BSSimpleList<TESCaravanDeck*>			kCaravanDecks;			// 0C0
	BSSimpleList<Script*>					kScripts;				// 0C8
	BSSimpleList<TESSound*>					kSounds;				// 0D0
	BSSimpleList<BGSAcousticSpace*>			kAcousticSpaces;		// 0D8
	BSSimpleList<BGSRagdoll*>				kRagdolls;				// 0E0
	BSSimpleList<TESGlobal*>				kGlobals;				// 0E8
	BSSimpleList<BGSVoiceType*>				kVoiceTypes;			// 0F0
	BSSimpleList<BGSImpactData*>			kImpactData;			// 0F8
	BSSimpleList<BGSImpactDataSet*>			kImpactDataSets;		// 100
	BSSimpleList<TESTopic*>					kTopics;				// 108
	BSSimpleList<TESTopicInfo*>				kTopicInfos;			// 110
	BSSimpleList<TESQuest*>					kQuests;				// 118
	BSSimpleList<TESCombatStyle*>			kCombatStyles;			// 120
	BSSimpleList<TESLoadScreen*>			kLoadScreens;			// 128
	BSSimpleList<TESWaterForm*>				kWaterForms;			// 130
	BSSimpleList<TESEffectShader*>			kEffectShaders;			// 138
	BSSimpleList<BGSProjectile*>			kProjectiles;			// 140
	BSSimpleList<BGSExplosion*>				kExplosions;			// 148
	BSSimpleList<BGSRadiationStage*>		kRadiationStages;		// 150
	BSSimpleList<BGSDehydrationStage*>		kDehydrationStages;		// 158
	BSSimpleList<BGSHungerStage*>			kHungerStages;			// 160
	BSSimpleList<BGSSleepDeprivationStage*>	kSleepDepriveStages;	// 168
	BSSimpleList<BGSDebris*>				kDebris;				// 170
	BSSimpleList<BGSPerk*>					kPerks;					// 178
	BSSimpleList<BGSBodyPartData*>			kBodyPartData;			// 180
	BSSimpleList<BGSNote*>					kNotes;					// 188
	BSSimpleList<BGSListForm*>				kListForms;				// 190
	BSSimpleList<BGSMenuIcon*>				kMenuIcons;				// 198
	BSSimpleList<TESObjectANIO*>			kAnimatedObjects;		// 1A0
	BSSimpleList<BGSMessage*>				kMessages;				// 1A8
	BSSimpleList<BGSLightingTemplate*>		kLightningTemplates;	// 1B0
	BSSimpleList<BGSMusicType*>				kMusicTypes;			// 1B8
	BSSimpleList<TESLoadScreenType*>		kLoadScreenTypes;		// 1C0
	BSSimpleList<MediaSet*>					kMediaSets;				// 1C8
	BSSimpleList<MediaLocationController*>	kMediaLocControllers;	// 1D0
	TESRegionList*							pRegions;				// 1D8
	NiTPrimitiveArray<TESObjectCELL*>		kCellArray;				// 1DC
	NiTPrimitiveArray<BGSAddonNode*>		kAddonArray;			// 1EC
	NiTPointerList<TESForm*>				kBadForms;				// 1FC
	UInt32									uiNextCreatedRefID;		// 208
	TESFile*								pActiveFile;			// 20C	last unselected mod in modList. GECK: active ESM
	ModList									kMods;					// 210
	bool									unk618;					// 618
	bool									bSaveLoadGame;			// 619
	bool									unk61A;					// 61A	referenced during LoadForm (ie TESSpellList). bit 1 might mean refID to pointer conversion not done. For GECK means save in progress
	bool									unk61B;					// 61B
	bool									unk61C;					// 61C
	bool									unk61D;					// 61D
	bool									bIsClosingFile;			// 61E
	bool									unk61F;					// 61F
	bool									bAssignFormIDs;			// 620
	bool									bIsLoading;				// 621
	UInt8									ucGameSettingsLoadState;// 622
	bool									unk623;					// 623
	TESRegionDataManager*					pRegionManager;			// 624
	void*									pVendorContainer;		// 628
	UInt32									unk62C;					// 62C
	UInt32									unk630;					// 630
	UInt32									unk634;					// 634
	UInt32									unk638;					// 638

	static TESDataHandler* GetSingleton() { return *reinterpret_cast<TESDataHandler**>(0x011C3F2C); }

	std::vector<TESFile*> GetActiveModList()
	{
		std::vector<TESFile*> activeMods;
		if (activeMods.empty())
			for (const auto iter : kMods.kModInfos)
				if (iter->IsLoaded()) activeMods.push_back(iter);
		return activeMods;
	};		// returns array of modEntry* corresponding to loaded mods sorted by mod index
	const TESFile* LookupModByName(const char* modName);
	UInt8 GetModIndex(const char* modName);
	UInt8 GetActiveModCount() const;
	const char* GetNthModName(UInt32 modIndex)
	{
		if (modIndex == 0xFF) return "Runtime";
		if (const auto iter = GetActiveModList()[modIndex]) return iter->m_Filename;
		return "";
	};

	void DisableAssignFormIDs(bool shouldAsssign);

	void RemoveIDFromDataHandler(UInt32 auiFormID);

	bool DoAddForm(TESForm* apForm);

	TESQuest* GetQuestByName(const char* questName);

	static TESForm* CreateFormFromID(UInt8 aucID);

	TESObjectCELL* GetCellFromCellCoord(SInt32 aiX, SInt32 aiY, TESWorldSpace* apWorldSpace, bool abUnk);
};
static_assert(sizeof(TESDataHandler) == 0x63C);