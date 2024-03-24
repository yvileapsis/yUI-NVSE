#pragma once

#include "TESForm.hpp"
#include "BSSimpleArray.hpp"

class NavMeshInfo;

typedef UInt32 FormID;
typedef UInt32 CellCoords;
typedef BSSimpleArray<NavMeshInfo*> NavMeshInfoArray;

class NavMeshInfoMap : public TESForm {
	bool													bOld;
	NiTPointerMap<FormID, NavMeshInfo*>						kNavMeshInfos;			// Form ID to NavMeshInfo
	NiTMap<FormID, NiTMap<CellCoords, NavMeshInfoArray*>>	kWorldNavMeshInfos;		// World Form ID -> Cell Coords -> NavMeshInfo
	bool													bInit;
};

ASSERT_SIZE(NavMeshInfoMap, 0x40)