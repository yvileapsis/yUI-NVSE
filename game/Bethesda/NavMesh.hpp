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

class NavMeshPtr : public NiPointer<NavMesh> {
public:
	NavMeshPtr() : NiPointer<NavMesh>() {}
	NavMeshPtr(NavMesh* apNavMesh) : NiPointer<NavMesh>(apNavMesh) {}
	NavMeshPtr(const NavMeshPtr& arPointer) : NiPointer<NavMesh>(arPointer) {}
	NavMeshPtr(const NavMeshPtr* apPointer) : NiPointer<NavMesh>(apPointer->m_pObject) {}
};

class NavMeshArray : public BSSimpleArray<NavMeshPtr> {
public:
	inline NavMeshPtr& GetAt(UInt32 auiIndex) {
		NavMeshPtr kPtr;
		if (auiIndex >= uiSize)
			return kPtr;
		else
			return pBuffer[auiIndex];
	}
};