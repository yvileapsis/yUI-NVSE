#pragma once

#include "BaseProcess.hpp"
#include "BSSimpleList.hpp"

class BGSListForm;
class TESBoundObject;

class LowProcess : public BaseProcess {
public:
	LowProcess();
	~LowProcess();

	struct ActorValueModifier {
		UInt8	ucActorValue;	// 00 Might allow for other values
		float	fDamage;		// 04
	};

	struct ActorValueList : public BSSimpleList<ActorValueModifier*> {
		UInt8								unk008;
		void***								modifiedAV;
	};

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

	UInt8					ucFlags_30;
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
	float					fEssentialDownTimer;
	float					fGameDayDied;
	float					fPlayerDamageDealt;
	bool					bSkippedTimeStampForPathing;
};

ASSERT_SIZE(LowProcess, 0xB4);