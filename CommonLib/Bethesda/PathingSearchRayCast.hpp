#pragma once

#include "PathingLocation.hpp"
#include "PathingRequest.hpp"

class bhkWorld;

class PathingSearchRayCast {
public:
	PathingSearchRayCast();
	virtual ~PathingSearchRayCast();

	PathingLocation		kStartLocation;
	PathingLocation		kGoalLocation;
	PathingRequestPtr	spRequest;
	UInt32				eType;
	NiPoint3			kBoundsMin;
	NiPoint3			kBoundsMax;
	float				fActorRadius;
	UInt32				uiStackSize;
	bhkWorld*			pHavokWorld;
	void*				pPickData;
	void*				pRayCollector;
	UInt32				unk088;
	float				fJumpFallHeightMin;
	float				unk090;
};

ASSERT_SIZE(PathingSearchRayCast, 0x94);