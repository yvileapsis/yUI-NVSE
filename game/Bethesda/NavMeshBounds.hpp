#pragma once

#include "NiPoint3.hpp"
#include "BSSimpleArray.hpp"

class NavMeshBounds {
public:
	struct NavMeshBoundsTriangle {
		UInt16	sVertices[3];
	};

	NiPoint3								kPoint30;
	NiPoint3								kPoint3C;
	BSSimpleArray<NavMeshBoundsTriangle>	kBoundTriangles;
	BSSimpleArray<NiPoint3>					kPoints;
};

ASSERT_SIZE(NavMeshBounds, 0x38);
ASSERT_SIZE(NavMeshBounds::NavMeshBoundsTriangle, 0x6);