#pragma once

#include "BSSimpleArray.hpp"
#include "NiPoint3.hpp"
#include "NiRefObject.hpp"
#include "NiTMap.hpp"
#include "TESChildCell.hpp"
#include "TESForm.hpp"

class NavMeshInfo;
struct ObstacleUndoData {};
struct NavMeshTriangleDoorPortal {};
struct NavMeshClosedDoorInfo {};
struct NavMeshStaticAvoidNode {};

struct ObstacleData {};

struct NavMeshTriangle {
	SInt16		sVertices[3];
	SInt16		sSides[3];
	Bitfield32	uiFlags;
};

struct EdgeExtraInfo {
	struct Connection {
		NavMeshInfo*	pNavMeshInfo;
		SInt16			sTriangle;
	};

	SInt32		unk000;
	Connection	kConnectTo;
};

struct NavMeshPOVData {

};

class NavMesh : public TESForm, public TESChildCell, public NiRefObject {
public:
	TESObjectCELL*								pParentCell;
	BSSimpleArray<NiPoint3>						kVertices;
	BSSimpleArray<NavMeshTriangle>				kTriangles;
	BSSimpleArray<EdgeExtraInfo>				kEdgeInfos;
	BSSimpleArray<NavMeshTriangleDoorPortal>	kDoorPortals;
	BSSimpleArray<NavMeshClosedDoorInfo>		kClosedDoorInfos;
	BSSimpleArray<UInt16>						unk078Arr;
	NiTMap<UInt16, NavMeshPOVData*>				kPOVDatas;
	BSSimpleArray<UInt16>						unk098Arr;
	UInt32										unk0A8;
	float										unk0AC[8];
	BSSimpleArray<UInt16>*						pArray0CC;
	BSSimpleArray<ObstacleUndoData*>			kObstacleUndos;
	NiTMap<UInt16, NiPointer<ObstacleData>>*	pObstacleDataMap;
	BSSimpleArray<UInt16>						unk0E4Arr;
	BSSimpleArray<NavMeshStaticAvoidNode>		kAvoidNodes;
	NavMeshInfo*								pNavMeshInfo;
};

ASSERT_SIZE(NavMesh, 0x108);

// Bizarre class
class NavMeshPtr : public NiPointer<NavMesh> {
public:
	using NiPointer<NavMesh>::NiPointer;
	NavMeshPtr& operator=(NavMesh* apObject) { NiPointer<NavMesh>::operator=(apObject); return *this; }

	// GAME - 0x464FC0
	static NavMeshPtr Create(NavMesh* apNavMesh) {
		return NavMeshPtr(apNavMesh);
	}
};

class NavMeshArray {
public:
	BSSimpleArray<NavMeshPtr> kNavMeshes;

	// GAME - 0x464F60
	NavMeshPtr GetNavMesh(uint32_t auiIndex) const {
		if (auiIndex >= kNavMeshes.GetSize())
			return NavMeshPtr::Create(nullptr);
		else
			return kNavMeshes.GetAt(auiIndex);
	}

	uint32_t GetNavMeshCount() const {
		return kNavMeshes.GetSize();
	}
};