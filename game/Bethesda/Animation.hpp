#pragma once
#include "NiPoint3.hpp"
#include "NiTPointerMap.hpp"
#include "BSSimpleList.hpp"
#include "NiRefObject.hpp"

class AnimSequenceBase;

// 0x38
class AnimIdle : public NiRefObject
{
public:
	UInt32 unk008;
	UInt32 unk00C;
	UInt32 unk010;
	UInt32 sequenceID;
	BSAnimGroupSequence* unk018;
	NiRefObject* unk01C[2];
	NiRefObject* unk024[2];
	TESIdleForm* idleForm;
	UInt32 unk030;
	Actor* actor;
};
static_assert(sizeof(AnimIdle) == 0x38);

// 0x13C
struct Animation
{
	struct Unk124
	{
		struct Unk18
		{
			UInt32					unk00[9];
			UInt32					unk24;
		};

		UInt32						unk00[6];
		Unk18*						unk18;
	};

	struct Unk128
	{
		UInt32						unk00[11];
		TESIdleForm*				idle2C;
	};

	UInt32							unk000;				// 000
	Actor*  						actor;				// 004
	NiNode*  						nSceneRoot;			// 008
	NiNode*  						nBip01;				// 00C
	NiPoint3						pt010;				// 010
	NiPoint3						pt01C;				// 01C
	NiNode*  						nPelvis;			// 028
	NiNode*  						nBip01Copy;			// 02C
	NiNode*  						nLForearm;			// 030
	NiNode*							nHead;				// 034
	NiNode* 						nWeapon;			// 038
	NiNode*							UNUSED03C;			// 03C
	NiNode*							UNUSED040;			// 040
	NiNode*							nNeck1;				// 044
	float							unk048;				// 048
	UInt16							groupIDs[8];		// 04C
	SInt32							sequenceState1[8];	// 05C
	SInt32							sequenceState2[8];	// 07C
	UInt16							unk09C[8];			// 09C
	UInt32							unk0AC[8];			// 0AC
	UInt8							byte0CC;			// 0CC
	UInt8							byte0CD;			// 0CD
	UInt8							byte0CE;			// 0CE
	UInt8							byte0CF;			// 0CF
	float							timePassed;			// 0D0
	UInt32							unk0D4;				// 0D4
	NiControllerManager*  			controllerManager;			// 0D8
	NiTPointerMap<AnimSequenceBase>* mapAnimSequenceBase;// 0DC
	BSAnimGroupSequence*			animSequence[8];	// 0E0
	BSAnimGroupSequence*			animSeq100;		// 100
	BSSimpleList<KFModel*>			loadingAnims;
	float							movementSpeedMult;
	float							rateOfFire;
	float 							turboSpeedMult;
	float 							weaponReloadSpeed;
	float 							equipSpeed;
	bool 							noBlend120;
	UInt8 							byte121;
	UInt16 							unk122;
	AnimIdle* 						idleAnim;
	AnimIdle* 						idleAnimQueued;
	NiNode* 						node12C;
	NiNode* 						node130;
	BSSimpleList<void*>				list134;

	TESAnimGroup::AnimGroupID GetNextAttackGroupID() const;

};
static_assert(sizeof(Animation) == 0x13C);