#pragma once

#include "PathingRequest.hpp"

class PathingRequestClosePoint : public PathingRequest {
public:
	float fMinDistance;
	float fMaxDistance;
};

ASSERT_SIZE(PathingRequestClosePoint, 0xB8);
