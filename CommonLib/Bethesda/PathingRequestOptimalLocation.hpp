#pragma once

#include "PathingRequest.hpp"

class PathingRequestOptimalLocation : public PathingRequest {
public:
	float fMinDistance;
	float fMaxDistance;
	float fAbsoluteMinDistance;
	float fAbsoluteMaxDistance;
};

ASSERT_SIZE(PathingRequestOptimalLocation, 0xC0);