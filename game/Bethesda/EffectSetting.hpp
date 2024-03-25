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

// B0
class EffectSetting :
	public TESForm,
	public TESModel,			// 18
	public TESDescription,		// 30
	public TESFullName,			// 38
	public TESIcon				// 44
{
public:
	EffectSetting();
	~EffectSetting();

	enum EnumArchType
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

	enum EnumEffectFlags : UInt32
	{
		kHostile					= 1 << 0,
		kRecover					= 1 << 1,
		kDetrimental				= 1 << 2,
		kSelf						= 1 << 4,
		kTouch						= 1 << 5,
		kTarget						= 1 << 6,
		kNoDuration					= 1 << 7,
		kNoMagnitude				= 1 << 8,
		kNoArea						= 1 << 9,
		kFXPersist					= 1 << 10,
		kGoryVisuals				= 1 << 12,
		kDisplayNameOnly			= 1 << 13,
		kRadioBroadcastSomething	= 1 << 15,
		kUseSkill					= 1 << 19,
		kUseAttribute				= 1 << 20,
		kPainless					= 1 << 24,
		kSprayProjectileType		= 1 << 25,
		kBoltProjectileType			= 1 << 26,
		kNoHitEffect				= 1 << 27,
		kNoDeathDispel				= 1 << 28,
	};

	struct EffectSettingData {
		EnumEffectFlags			eEffectFlags;			// 58 - start of DATA
		Float32					fBaseCost;				// 5C
		TESForm*				pkAssociatedItem;		// 60 - Script* for ScriptEffects
		UInt32					uiMagicSchool;			// 64 - unused
		UInt32					uiResistValue;			// 68 - actor value for resistance
		UInt16					usCounterEffectCount;	// 6C - counter effects count
		UInt8					pad6E[2];				// 6E
		TESObjectLIGH*			pkLight;				// 70
		Float32					fProjectileSpeed;		// 74
		TESEffectShader*		pkEffectShader;			// 78 - effect shader
		TESEffectShader*		pkObjectDisplayShader;	// 7C
		TESSound*				pkEffectSound;			// 80
		TESSound*				pkBoltSound;			// 84
		TESSound*				pkHitSound;				// 88
		TESSound*				pkAreaSound;			// 8C
		Float32					fMagicDefaultCEEnchantFactor;
		Float32					fMagicDefaultCEBarterFactor;
		EnumArchType			eArchetypeType;			// 98
		UInt32					uiActorVal;				// 9C - actor value - last field of DATA
	};

	UInt32					unk50;					// 50
	UInt32					unk54;					// 54
	EffectSettingData		kData;
	BSSimpleList<EffectSetting*> kCounterEffects;	// A0 - counter effects list
	UInt32					unkA8;					// A8
	UInt32					unkAC;					// AC

	Script* GetScript() const { return 1 == kData.eArchetypeType ? (Script*)kData.pkAssociatedItem : NULL; };
	bool HasScript() const { return 1 == kData.eArchetypeType ? true : false; };
	Script* RemoveScript();
	Script* SetScript(Script* newScript);
};
static_assert(sizeof(EffectSetting) == 0xB0);