#pragma once

#include "PathingRequest.hpp"

class PathingRequestLOS : public PathingRequest {
public:
	NiRefObjectPtr			spLOSGridMap;
	BSSimpleArray<NiPoint3> kIgnoreLocations;
	float					fIgnoreLocationRadius;
	NiPoint3				kSearchCenter;
	float					fSearchCenterRadius;
};

ASSERT_SIZE(PathingRequestLOS, 0xD8);