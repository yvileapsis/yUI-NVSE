#pragma once

#include "NiRefObject.hpp"
#include "NiPoint3.hpp"
#include "PathingLocation.hpp"
#include "PathingAvoidNodeArray.hpp"

class TESForm;
class BGSSaveFormBuffer;
class PathingAvoidNodeArray;

NiSmartPointer(PathingRequest);

class PathingRequest : public NiRefObject {
public:
	virtual ~PathingRequest();

	virtual void	CopyTo(PathingRequest* apTarget);		// 02
	virtual void	Unk_03();								// 03
	virtual UInt32	GetType();								// 04
	virtual void	PrintDebugText(void*);					// 05
	virtual void	Write(BGSSaveFormBuffer* apBuffer);		// 06
	virtual void	Read(BGSSaveFormBuffer* apBuffer);		// 07
	virtual void	Unk_08();								// 08

	struct ActorData {
		TESForm*	pBaseForm;
		void*		pInventoryChanges;
		bool		bIsAlarmed;
		float		fRadius;
		float		fHeight;
	};

	UInt32						unk08;
	PathingLocation				kStart;
	PathingLocation				kDest;
	ActorData					kActorData;
	float						fGoalZDelta;
	float						fTargetRadius;
	float						fCenterRadius;
	NiPoint3					kTargetPt;
	float						unk88;
	float						fGoalAngle;
	float						fInitialPathHeading;
	PathingAvoidNodeArrayPtr	spAvoidNodeArray;
	bool						bCantOpenDoors;
	bool						bFaceTargetAtGoal;
	bool						byte9A;
	bool						bAllowIncompletePath;
	bool						byte9C;
	bool						bCanSwim;
	bool						bCanFly;
	bool						byte9F;
	bool						bInitialPathHeading;
	bool						bAllowDirectPathOnFail;
	bool						bCurvedPath;
	bool						byteA3;
	bool						bIgnoreLocks;
	UInt32						iSmoothingRetryCount;
	bool						byteAC;
};

ASSERT_SIZE(PathingRequest, 0xB0);