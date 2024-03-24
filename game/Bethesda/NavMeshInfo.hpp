#pragma once

#include "NavMeshBounds.hpp"

class TESObjectCELL;
class TESObjectREFR;
class TESWorldSpace;
class NavMesh;

class NavMeshInfo {
public:
	enum Flags {
		UNK_1		= 1 << 0,
		UNK_2		= 1 << 1,
		UNK_4		= 1 << 2,
		UNK_8		= 1 << 3,
		UNK_10		= 1 << 4,
		HAS_BOUNDS	= 1 << 5,
	};

	UInt32							uiNavMeshID;
	UInt32							uiNavmeshWorldspaceOrCellRefID;
	Bitfield32						uiFlags;
	union {
		struct {
			SInt16					sPosX;
			SInt16					sPosY;
		};
		UInt32 uiCellCoords;
	};
	NiPoint3						kApproxLocation;
	union {
		TESObjectCELL*				pParentCell;
		TESWorldSpace*				pParentWorldspace;
	};
	float							fPreferredPercent;
	BSSimpleArray<NavMeshInfo*>		kNavmeshInfoArray24;
	BSSimpleArray<NavMeshInfo*>		kNavmeshInfoArray34;
	BSSimpleArray<TESObjectREFR*>	kConnectedDoors;
	NavMesh*						pNavMesh;
	NavMeshBounds*					pBoundData;
};

ASSERT_SIZE(NavMeshInfo, 0x5C);