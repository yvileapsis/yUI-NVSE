#pragma once

#include "BaseFormComponent.hpp"

class BGSExplosion;
class BGSDebris;
class TESModelTextureSwap;

struct DestructibleObjectStage {
	enum
	{
		kFlags_CapDamage = 1,
		kFlags_DisableObject = 2,
		kFlags_DestroyObject = 4,
	};

	UInt8					cModelDamageStage;		// 00
	UInt8					cHealthPercentage;		// 01
	UInt16					cFlags;			// 02
	UInt32					iSelfDamagePerSecond;		// 04
	BGSExplosion*			pExplosion;		// 08
	BGSDebris*				pDebris;		// 0C
	UInt32					iDebrisCount;	// 10
	TESModelTextureSwap*	pReplacementModel;	// 14
};

ASSERT_SIZE(DestructibleObjectStage, 0x18);

struct DestructibleObjectData {
	UInt32				iHealth;		// 00
	UInt8				cNumStages;	// 04
	bool				targetable;	// 05
	UInt8				unk06[2];	// 06
	DestructibleObjectStage** pStagesArray;	// 08
};

// 08
class BGSDestructibleObjectForm : public BaseFormComponent {
public:
	BGSDestructibleObjectForm();
	~BGSDestructibleObjectForm();

	DestructibleObjectData* pData;			// 04
};

ASSERT_SIZE(BGSDestructibleObjectForm, 0x8);