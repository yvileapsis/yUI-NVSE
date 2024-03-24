#pragma once

#include "BSWin32TaskletData.hpp"
#include "ObstacleTaskNavMeshOperation.hpp"
#include "NavMeshEdgeHandleArrays.hpp"

class ObstacleTaskData : public BSWin32TaskletData {
public:
	NavMeshInfo*								pNavMeshInfo;
	NavMeshPtr									spNavMesh1C;
	NavMeshPtr									spNavMesh20;
	BSSimpleArray<ObstacleTaskNavMeshOperation> kOperations;
	NavMeshEdgeHandleArrays						kEdgeHandles;
	BYTE										byte54;
};

ASSERT_SIZE(ObstacleTaskData, 0x58);