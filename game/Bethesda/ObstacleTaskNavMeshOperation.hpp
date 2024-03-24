#pragma once

#include "NavMesh.hpp"

class ObstacleTaskNavMeshOperation {
public:
	NiPointer<NiRefObject>	spObject0;
	UInt32					unk4;
	UInt32					unk8;
	NavMeshPtr				spNavMesh;
};

ASSERT_SIZE(ObstacleTaskNavMeshOperation, 0x10)