#pragma once

#include "BSSimpleArray.hpp"

class TESObjectCELL;
class TESWorldSpace;
class BGSSaveFormBuffer;
class NavMeshInfo;

class PathingLocation {
public:
	virtual void  Write(BGSSaveFormBuffer* apBuffer);
	virtual void  Read(BGSSaveFormBuffer* apBuffer);

	NiPoint3						kLocation;
	NavMeshInfo*					pNavMeshInfo;
	BSSimpleArray<NavMeshInfo*>*	pNavMepNavMeshesshInfo;
	TESObjectCELL*					pCell;
	TESWorldSpace*					pWorldSpace;
	UInt32							uiCellCoords;
	UInt16							usTriangle;
	UInt8							ucFlags;
	UInt8							ucClientData;
};

ASSERT_SIZE(PathingLocation, 0x28);