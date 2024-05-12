#pragma once

#include "PathingRequest.hpp"

class PathingRequestFlee : public PathingRequest {
public:
	float fMinFleeDistance;
};

ASSERT_SIZE(PathingRequestFlee, 0xB4);