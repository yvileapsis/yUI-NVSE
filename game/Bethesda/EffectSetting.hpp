#pragma once

#include "BSSimpleList.hpp"
#include "TESIcon.hpp"
#include "TESFullName.hpp"
#include "TESModel.hpp"
#include "TESDescription.hpp"
#include "TESForm.hpp"

class TESObjectLIGH;
class TESEffectShader;
class TESSound;

class EffectSetting : public TESForm, public TESModel, public TESDescription, public TESFullName, public TESIcon {
public:
	EffectSetting();
	~EffectSetting();

	enum
	{
		kArchType_ValueModifier = 0,
		kArchType_Script,
		kArchType_Dispel,
		kArchType_CureDisease,
		kArchType_Invisibility = 11,
		kArchType_Chameleon,
		kArchType_Light,
		kArchType_Lock = 16,
		kArchType_Open,
		kArchType_BoundItem,
		kArchType_SummonCreature,
		kArchType_Paralysis = 24,
		kArchType_CureParalysis = 30,
		kArchType_CureAddiction,
		kArchType_CurePoison,
		kArchType_Concussion,
		kArchType_ValueAndParts,
		kArchType_LimbCondition,
		kArchType_Turbo,
	};

	enum EffectFlags {
		HOSTILE						= 0x1,
		RECOVER						= 0x2,
		DETRIMENTAL					= 0x4,
		UNK_8						= 0x8,
		SELF						= 0x10,
		TOUCH						= 0x20,
		TARGET						= 0x40,
		NO_DURATION					= 0x80,
		NO_MAGNITUDE				= 0x100,
		NO_AREA						= 0x200,
		PERSIST						= 0x400,
		CREATE_SPELLMAKING			= 0x800,
		GORY_VISUALS				= 0x1000,
		kDisplayNameOnly			= 0x2000,
		kRadioBroadcastSomething	= 0x8000,
		kUseSkill					= 0x80000,
		kUseAttribute				= 0x100000,
		PAINLESS					= 0x1000000,
		kSprayProjectileType		= 0x2000000,
		kBoltProjectileType			= 0x4000000,
		NO_HIT_EFFECT				= 0x8000000,
		NO_DEATH_DISPEL				= 0x10000000,
	};

	struct EffectSettingData {
		UInt32				uiEffectFlags;
		float				fBaseCost;
		TESForm*			pAssociatedItem;
		UInt32				magicSchool;
		UInt32				uiResistVal;
		UInt16				usCounterEffectCount;
		TESObjectLIGH*		pLight;
		float				fProjectileSpeed;
		TESEffectShader*	pEffectShader;
		TESEffectShader*	pObjectDisplayShader;
		TESSound*			pEffectSound;
		TESSound*			pBoltSound;
		TESSound*			pHitSound;
		TESSound*			pAreaSound;
		float				fMagicDefaultCEEnchantFactor;
		float				fMagicDefaultCEBarterFactor;
		UInt32				uiArchType;
		UInt32				uiActorVal;
	};



	UInt32							unk50;
	UInt32							unk54;
	EffectSettingData				kData;
	BSSimpleList<EffectSetting*>	kCounterEffects;
	UInt32							unkA8;
	UInt32							unkAC;

	Script* GetScript() const { return 1 == kData.uiArchType ? (Script*)kData.pAssociatedItem : NULL; };
	bool HasScript() const { return 1 == kData.uiArchType ? true : false; };
	Script* RemoveScript();
	Script* SetScript(Script* newScript);
};

ASSERT_SIZE(EffectSetting, 0xB0);