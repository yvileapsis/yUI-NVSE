#pragma once

#include "PathingRequest.hpp"

class PathingRequestCover : public PathingRequest {
public:
	NiPoint3	kThreatPos;
	float		fMaxCoverDistance;
	float		fActorCrouchHeight;
	float		fMaxHeightChange;
	UInt32		uiMaxResults;
	bool		bCanCrouch;
	bool		bFindBothSides;
	bool		bFindBehindTarget;
	bool		bSortFarthestFirst;
	bool		bSortDistanceFromTarget;
	bool		bHideCover;
	NiPoint3	kDistanceProjectionVector;
	NiPoint3	kActorPos;
};

ASSERT_SIZE(PathingRequestCover, 0xEC);
