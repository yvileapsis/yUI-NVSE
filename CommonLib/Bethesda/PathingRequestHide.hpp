#pragma once

#include "PathingRequest.hpp"

class PathingRequestHide : public PathingRequest {
public:
	PathingLocation kThreatLocation;
	float			fMinHideDistance;
};

ASSERT_SIZE(PathingRequestHide, 0xDC);