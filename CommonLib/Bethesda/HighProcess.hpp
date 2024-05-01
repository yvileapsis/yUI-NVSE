#pragma once
#include "MiddleHighProcess.hpp"
#include "BSSoundHandle.hpp"
#include "NiSmartPointer.hpp"

class NiBSBoneLODController;
class NiBSplineCompTransformInterpolator;
class ProjectileData;
class DetectionEvent;
class Actor;
class bhkShapePhantom;
class DialogueItem;
class LipSynchAnim;
class MoveToMarker;
class PathingRequest;
class ActorPathingMessageQueue;

NiSmartPointer(PathingRequest);
NiSmartPointer(ActorPathingMessageQueue);

class HighProcess : public MiddleHighProcess
{
public:
	HighProcess();
	~HighProcess();

	struct HeadTracking {
		union {
			TESObjectREFR* pHeadTrackingTargets[6];
			struct {
				TESObjectREFR* pTargetDefault;
				TESObjectREFR* pTargetAction;
				TESObjectREFR* pTargetCombat;
				TESObjectREFR* pTargetDialog;
				TESObjectREFR* pTargetScript;
				TESObjectREFR* pTargetProcedure;
			};
		};
		UInt8 ucHeadTrackingTargetFlags[6];
	};

	enum FADE_STATE {
		FADE_NORMAL = 0x0,
		FADE_IN = 0x1,
		FADE_OUT = 0x2,
		FADE_TELEPORT_IN = 0x3,
		FADE_TELEPORT_OUT = 0x4,
		FADE_OUT_DISABLE = 0x5,
		FADE_OUT_DELETE = 0x6,
		FADE_MOVE_TO_MARKER = 0x7,
		FADE_STATE_COUNT = 0x8,
	};

	enum
	{
		kAnimAction_None = -1,
		kAnimAction_Equip_Weapon,
		kAnimAction_Unequip_Weapon,
		kAnimAction_Attack,
		kAnimAction_Attack_Follow_Through,
		kAnimAction_Attack_Latency,
		kAnimAction_Attack_Throw_Attach,
		kAnimAction_Attack_Throw_Release,
		kAnimAction_Block,
		kAnimAction_Stagger,
		kAnimAction_Reload,
		kAnimAction_Dodge,
		kAnimAction_Wait_For_Lower_Body_Anim,
		kAnimAction_Wait_For_Special_Idle,
		kAnimAction_Force_Script_Anim
	};

	enum
	{
		kSitSleepState_Normal = 0x0,
		kSitSleepState_LoadSitIdle = 0x1,
		kSitSleepState_WantToSit = 0x2,
		kSitSleepState_WaitingForSitAnim = 0x3,
		kSitSleepState_Sitting = 0x4,
		kSitSleepState_WantToStand = 0x5,
		kSitSleepState_LoadingSleepIdle = 0x6,
		kSitSleepState_WantToSleep = 0x7,
		kSitSleepState_WaitingForSleepAnim = 0x8,
		kSitSleepState_Sleeping = 0x9,
		kSitSleepState_WantToWake = 0xA,
	};

	struct ActorValueArray
	{
		struct Value
		{
			UInt8 isModified;
			UInt8 gap01[3];
			float value;
		};

		Value avs[77];
	};

	BSSimpleList<DetectionData>*			pDetectedActors;
	BSSimpleList<DetectionData>*			pDetectingActors;
	BSSimpleList<void*>*					actorList264;
	BSSimpleList<void*>*					detectionDataList268;
	BSSimpleList<void*>*					detectionDataList26C;
	UInt32									unk270;
	BSSimpleList<void*>						combatTargeBSSimpleList274;
	BSSimpleList<void*>						list27C;
	BSSimpleList<void*>						list284;
	BSSimpleList<void*>						kExteriorSpectators;
	float									fDetectListTimer;
	float									fIdleChatterCommentTimer;
	UInt8									byte29C;
	UInt8									bProcessGreetSayTo;
	UInt8									gap29E;
	UInt8									gap29F;
	float									fAITalkToSameNPCTimer;
	Actor*									unk2A4;
	float									flt2A8;
	TESObjectREFR*							unk2AC;
	float									fAlpha;
	float									fPackageEvalTimer;
	float									fUseItemTimer;
	float									flt2BC;
	UInt16									count2C0;
	UInt16									numVolleyShots;
	UInt16									count2C4;
	UInt8									byte2C6;
	UInt8									bSkippedUpdate;
	float									fSocialTimerForConversations;
	UInt32									unk2CC;
	float									flt2D0;
	float									flt2D4;
	float									flt2D8;
	UInt32									unk2DC;
	float									fEvaluateAcquireTimer;
	NiBSBoneLODController*					pBoneLODController;
	SInt32									iLODLevel;
	SInt16									usCurrentAction;
	BSAnimGroupSequence*					pCurrentSequence;
	bool									bForceFireWeapon;
	float									fDetectionTimer;
	UInt32									unk2FC[5];
	float									fTime310;
	BSSoundHandle							kSounds[2];
	UInt8									bGreetingFlag;
	UInt8									gap32D;
	UInt8									gap32E;
	UInt8									gap32F;
	float									fDyingTimer;
	float									fIdleTimer;
	float									flt338;
	float									fDiveBreath;
	UInt32									unk340;
	float									fPainSoundTimer344;
	UInt8									byte348;
	UInt8									bWeaponAlertDrawn;
	UInt8									gap34A;
	UInt8									gap34B;
	float									fAwarePlayerTimer;
	TESIdleForm*							pForcedIdleForm;
	UInt32									unk354[4];
	UInt8									bInDialogMenu;
	DialogueItem*							pGreetTopic;
	UInt32									unk36C;
	Actor*									pDialogTarget;
	bool									bIsContinuingPackagePCNear;
	bool 									bActivateAnim;
	float									fScriptPackageEndTime;
	float									flt37C;
	UInt32									unk380;
	float									flt384;
	float									flt388;
	BSSimpleList<void*>						list38C;
	BSSimpleList<void*>						list394;
	UInt32									iNumberGuardsPursuing;
	UInt8									bStop;
	UInt8									gap3A1;
	UInt8									gap3A2;
	UInt8									gap3A3;
	float									fReequipArmorTimer;				// 3A4
	bool									bUnequippedArmorToSwim;
	UInt32									unk3AC;
	UInt32									unk3B0;
	UInt32									unk3B4;
	UInt8									byte3B8;
	UInt8									byte3B9;
	UInt8									gap3BA;
	UInt8									gap3BB;
	float									fDetectionModifier;
	float									fDetectionModifierTimer;
	float									fLightAmount;
	float									fLightUpdateTimer;
	LipSynchAnim*							pLipSyncAnim;
	UInt8									byte3D0;
	UInt8									byte3D1;
	UInt8									gap3D2;
	UInt8									gap3D3;
	ProjectileData*							pProjData;
	UInt32									unk3D8;
	DetectionEvent*							pActorsGeneratedDetectionEvent;
	UInt8									byte3E0;
	DialogueItem*							pDialogueItem3E4;
	FADE_STATE								eFadeState;
	float									fFadeAlpha;
	TESObjectREFR*							pTeleportFadeRef;
	MoveToMarker*							pMoveToMarker;
	HeadTracking							kHeadTracking;
	float									fHoldHeadTrackTimer;
	TESObjectREFR*							pPackageTarget;
	UInt8									byte420;
	UInt32									uiQueuedIdleFlags;
	ActorValueArray*						pAV_428;
	float									flt42C;
	UInt32									unk430;
	bhkShapePhantom*						ptr434;
	float									fDetectDoorsForPathingTimer;
	float									fRadReevaluationTimer;
	float									fRadsSec440;
	bool									bHasLiveGrenade;
	bool									bForceSpeakToPlayerAfterGettingUp;
	float									fTakeBackTimer;
	BSSimpleList<NiNode*>					kAvoidNodes;
	DialogueItem*							pDialogueItem454;
	bool									bHiding;
	bool									bIsDoingSayTo;
	bool									gap45A;
	bool									gap45B;
	NiPointer<PathingRequest>				spPathingRequest0;
	NiPointer<PathingRequest>				spPathingRequest1;
	NiPointer<ActorPathingMessageQueue>		spPathingMessageQueue0;
	NiPointer<ActorPathingMessageQueue>		spPathingMessageQueue1;
};
static_assert(sizeof(HighProcess) == 0x46C);