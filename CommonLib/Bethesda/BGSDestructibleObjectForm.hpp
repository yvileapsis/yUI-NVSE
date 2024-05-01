#pragma once
#include "BaseFormComponent.hpp"
#include "NiTArray.hpp"

class BGSExplosion;
class BGSDebris;
class TESModelTextureSwap;

// 0x18
struct DestructibleObjectStage 
{
	enum
	{
		kFlags_CapDamage = 1,
		kFlags_DisableObject = 2,
		kFlags_DestroyObject = 4,
	};

	UInt8					ucModelDamageStage;		// 00
	UInt8					ucHealthPercentage;		// 01
	UInt16					usFlags;				// 02
	UInt32					uiSelfDamagePerSecond;	// 04
	BGSExplosion*			pkExplosion;			// 08
	BGSDebris*				pkDebris;				// 0C
	UInt32					uiDebrisCount;			// 10
	TESModelTextureSwap*	pkReplacementModel;		// 14
};
static_assert(sizeof(DestructibleObjectStage) == 0x18);

// 0xC
struct DestructibleObjectData
{
	UInt32					uiHealth;				// 00
	UInt8					ucNumStages;			// 04
	bool					bTargetable;			// 05
	UInt8					unk06[2];				// 06
	NiTObjectArray<DestructibleObjectStage*>*	pkStagesArray;	// 08
};
static_assert(sizeof(DestructibleObjectData) == 0xC);

// 0x8
class BGSDestructibleObjectForm : public BaseFormComponent {
public:
	BGSDestructibleObjectForm();
	~BGSDestructibleObjectForm();

	DestructibleObjectData* pkData;			// 04
};
static_assert(sizeof(BGSDestructibleObjectForm) == 0x8);