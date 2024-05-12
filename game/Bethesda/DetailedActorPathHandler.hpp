#pragma once

#include "ActorPathHandler.hpp"
#include "NiPoint3.hpp"
#include "BSSimpleList.hpp"

class DetailedActorPathHandler : public ActorPathHandler {
public:
	enum PathingState {
		INITIALIZING				= 1,
		MOVING						= 2,
		MOVING_WHILE_WAITING		= 3,
		WAITING_FOR_UPDATED_PATH	= 4,
		GOAL_REACHED				= 5,
		ROTATING_WITHOUT_PATH		= 6,
	};

	enum ActorAvoidState {
		OK				= 1,
		WAITING			= 2,
		REPATHING		= 3,
		REPATH_IGNORE	= 4,
		FAILED			= 5,
	};

	float				float14;
	float				float18;
	NiPoint3			nipoint31C;
	NiPoint3			nipoint328;
	NiPoint3			nipoint334;
	NiPoint3			kActorPosition;
	NiPoint3			nipoint34C;
	BSSimpleList<void*> bssimplelist_void58;
	DWORD				uiFlags60;
	DWORD				dword64;
	float				gap68;
	float				float6C;
	float				float70;
	float				fAvoidanceMinWaitTime;
	float				fDetect;
	float				gap7C;
	float				fFallTime;
	float				gap84;
	float				gap88;
	float				float8C;
	float				float90;
	float				fTurnAnimTime;
	float				gap98;
	float				gap9C;
	NiPoint3			charA0;
	float				floatAC;
	float				gapB0;
	float				floatB4;
	float				floatB8;
	float				floatBC;
	BYTE				byteC0;
	BYTE				byteC1;
	BYTE				gapC2[2];
	float				floatC4;
	PathingState		ePathingState;
	DWORD				dwordCC;
	ActorAvoidState		eActorAvoidState;
	BYTE				gapD4[4];
	DWORD				dwordD8;
	bool				bRotating;
	bool				gapDD;
	bool				bStaticAvoidance;
	BYTE				byteDF;
	BYTE				byteE0;
	BYTE				byteE1;
	BYTE				byteE2;
};

ASSERT_SIZE(DetailedActorPathHandler, 0xE4)