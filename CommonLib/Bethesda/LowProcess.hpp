#pragma once
#include "BaseProcess.hpp"
#include "NiPoint2.hpp"
#include "BSSimpleList.hpp"

class LowProcess : public BaseProcess
{
public:

	struct ActorValueModifier
	{
		UInt8			ucActorValue;	// 00 Might allow for other values
		UInt8			pad[3];			// 01
		Float32			fDamage;		// 04
	};

	struct ActorValueList : public BSSimpleList<ActorValueModifier*>
	{
		UInt8			unk008;			// 08
		UInt8			pad009[3];		// 09
		void***			modifiedAV;		// 0C	array of damaged actorValue
	};	// 10

	struct ObjectToAcquire {
		TESObjectREFR*	pRef;
		TESForm*		pForm;
		bool			bSteal;
		bool			bKill;
		UInt32			uiAmount;
		UInt32			uiWantValue;
		ExtraDataList*	pExtraDataList;
		UInt32			eType;
	};


	LowProcess();
	~LowProcess() override;
	virtual void	ProcessEscort();
	virtual void	ProcessSleep();
	virtual void	ProcessEat();
	virtual void	Unk_1F0();
	virtual void	ProcessAlarm();
	virtual void	ProcessActivate();
	virtual void	ProcessAccompany();
	virtual void	ProcessAcquire();
	virtual void	ProcessAmbushWait();
	virtual void	ProcessUseItemAt();
	virtual void	ProcessUseWeapon();
	virtual void	ProcessChooseCast();
	virtual void	ProcessFleeNonCombat();
	virtual void	ProcessAvoidRadiation();
	virtual void	ProcessSandbox();
	virtual void	ProcessMovementBlocked();
	virtual void	ProcessGuard();
	virtual void	ProcessGuardStartTravelToLocation();
	virtual void	ProcessPatrol();
	virtual void	ProcessSummonCreatureDefend();
	virtual void	ProcessClearMountPosition();
	virtual void	ProcessNotify();
	virtual void	CheckIfHasObject();
	virtual void	Unk_204();
	virtual void	GetLocationForPackage();
	virtual void	Unk_206();

	UInt8					ucFlags_30;		// 8 = IsAlerted
	UInt8					pad31[3];
	TESBoundObject*			pUsedItem;
	NiPoint2				kTimer;
	Actor*					pTarget;
	TESForm*				pGenericLocation;
	TESForm*				pSecondLocation;
	BGSListForm*			pList4C;
	UInt16					usCount50;
	TESForm*				unk54;
	UInt32					uiNumberItemsActivate;
	BSSimpleList<UInt32>	kObjectList;
	ObjectToAcquire*		pAcquireObject;
	UInt32					unk68;
	BSSimpleList<UInt32>	kGenericLocationsList;
	BSSimpleList<UInt32>	kToAcquireList;
	BSSimpleList<UInt32>	kList7C;
	TESForm*				pForm84;
	UInt32					unk88;
	UInt32					ePackageObjectType;
	TESFaction*				pFaction90;
	ActorValueList			kActorValues;
	Float32					fEssentialDownTimer;
	Float32					fGameDayDied;
	Float32					fPlayerDamageDealt;
	bool					bSkippedTimeStampForPathing;
};
static_assert(sizeof(LowProcess) == 0xB4);