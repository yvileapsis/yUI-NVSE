#pragma once

#include "BSSimpleArray.hpp"
#include "NavMeshEdgeHandle.hpp"

class NavMeshEdgeHandleArrays {
public:
	BSSimpleArray<NavMeshEdgeHandle> kArray0;
	BSSimpleArray<NavMeshEdgeHandle> kArray1;
};

ASSERT_SIZE(NavMeshEdgeHandleArrays, 0x20);