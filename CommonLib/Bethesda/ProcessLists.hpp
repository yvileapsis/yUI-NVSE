#pragma once

#include "ActorsScriptTaskData.hpp"
#include "ActorUpdateTaskData.hpp"
#include "AnimationTaskData.hpp"
#include "BaseProcess.hpp"
#include "BSSemaphore.hpp"
#include "BSSimpleList.hpp"
#include "BSTFreeList.hpp"
#include "DetectionTaskData.hpp"
#include "LipTask.hpp"
#include "LockFreeMap.hpp"
#include "MagicHitEffect.hpp"
#include "MobileObjectMessage.hpp"
#include "MovementTaskData.hpp"
#include "NiSmartPointer.hpp"
#include "PackageUpdateTaskData.hpp"

class MobileObject;
class BSTempEffect;
class MuzzleFlash;
class Crime;
class Actor;

struct ProcessLists {
	struct TaskData {
		DetectionTaskData							kDetectionTaskData;
		AnimationTaskData							kAnimationTaskData;
		PackageUpdateTaskData						kPackageUpdateTaskData;
		ActorUpdateTaskData							kActorUpdateTaskData;
		ActorsScriptTaskData						kActorsScriptTaskData;
		MovementTaskData							kMovementTaskData;
		BSTStaticFreeList<MobileObjectMessage,4096> kMobileObjectMessageList;
		bool										byte10180;
	};

	struct ProcessArrays {
		NiTPrimitiveArray<MobileObject*>	kMobileObjects;
		UInt32								uiBeginOffsets[BaseProcess::ProcessLevel::COUNT];
		UInt32								uiEndOffsets[BaseProcess::ProcessLevel::COUNT];
		UInt32								unk034[BaseProcess::ProcessLevel::COUNT];

		MobileObject* GetAt(UInt32 auiIndex) const {
			return kMobileObjects[auiIndex];
		};
	};

	UInt32											unk000;
	ProcessArrays									kAllProcessArrays;
	BSSimpleList<Crime*>*							pCrimes[5];
	BSSimpleList<BSTempEffectPtr>					kTempEffects;
	BSSimpleList<MagicHitEffectPtr>					kHitEffects;
	BSSimpleList<MuzzleFlash*>						kMuzzleFlashList;
	BSSimpleList<void*>								kArrowProjectileList;
	BSSimpleList<void*>								kTempShouldMoveList;
	BSSimpleList<Actor*>							kHighActors;
	Actor*											pNearbyActors[50];
	UInt32											uiNearbyActorCount;
	float											fCommentOnPlayerActionsTimer;
	float											fCommentOnPlayerKnockingThingsTimer;
	bool											bPlayerInRadiation1;
	bool											bPlayerInRadiation2;
	TaskData										kTaskData;
	BSSimpleList<UInt32>							kList102E4;
	UInt32											unk102EC;
	UInt32											unk102F0;
	UInt32											unk102F4;
	UInt32											unk102F8;
	UInt32											unk102FC;
	BSSpinLock										kNearbyActorLock;
	BSSpinLock										kLock_10320;
	BSSpinLock										kLock_10340;
	LockFreeMap<MobileObject*, NiPointer<LipTask>>	kLipTaskMap;
	bool											bToggleAI;
	bool											bToggleDetection;
	bool											bToggleDetectionStats;
	UInt8											byte103A3;
	UInt32											uiDetectionStats;
	bool											bToggleHighProcess;
	bool											bToggleLowProcess;
	bool											bToggleMidHighProcess;
	bool											bToggleMidLowProcess;
	bool											bToggleAISchedules;
	bool											bShowSubtitle;
	UInt8											byte103AE;
	UInt8											byte103AF;
	UInt32											uiNumberHighActors;
	float											fCrimeUpdateTimer;
	UInt32											uiCrimeNumber;
	float											fRemoveExcessComplexDeadTime;
	BSSemaphore										semaphore103C0;

	__forceinline static ProcessLists* GetSingleton() { return (ProcessLists*)0x11E0E80; }

	__forceinline int GetTotalDetectionValue(Actor* actor, bool arg2 = false) {
		return ThisStdCall<int>(0x973710, this, actor, arg2);
	}

	void UpdateTempEffects(float afTime);

	__forceinline UInt32 GetArrayStart(UInt32 auiArray) const {
		return kAllProcessArrays.uiBeginOffsets[auiArray];
	};

	__forceinline UInt32 GetArrayEnd(UInt32 auiArray) const {
		return kAllProcessArrays.uiEndOffsets[auiArray];
	};

	static SInt32* const iGameDay;
	static SInt32* const iGameMonth;
	static SInt32* const iGameYear;
	static float*  const fGameHour;
};

ASSERT_SIZE(ProcessLists, 0x103CC);