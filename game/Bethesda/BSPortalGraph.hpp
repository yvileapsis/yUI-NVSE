#pragma once

#include "NiTPointerList.hpp"
#include "NiAVObject.hpp"
#include "BSSimpleArray.hpp"
#include "BSOcclusionPlane.hpp"
#include "BSMultiBoundRoom.hpp"


class NiNode;
class ShadowSceneLight;
class BSPortal;

NiSmartPointer(BSPortalGraph);

class BSPortalGraph : public NiRefObject {
public:
	BSPortalGraph();
	virtual ~BSPortalGraph();

	NiTPointerList<BSOcclusionPlane*>				kOccluders;
	NiTPointerList<BSPortal*>						kPortals;
	NiTPointerList<BSMultiBoundRoomPtr>				kMultiBoundRooms;
	BSMultiBoundRoomPtr								spRoom02C;
	NiTPointerList<BSMultiBoundRoomPtr>				kMultiBoundRoomAccumList;
	NiTObjectArray<NiAVObjectPtr>					kAlwaysRenderChildren;
	NiPointer<NiNode>								spPortalNodeRoot;
	NiTPointerList<ShadowSceneLight>				kAttachAlwaysRenderQueue;
	NiTPointerList<ShadowSceneLight>				kDetachAlwaysRenderQueue;
	BSSimpleArray<NiPointer<NiNode>>				kUnboundNodes;

	static BSPortalGraph* Create();

	NiTPointerList<BSOcclusionPlane*>& GetOccluders() { return kOccluders; }
	NiTPointerList<BSPortal*>& GetPortals() { return kPortals; }
	NiTPointerList<BSMultiBoundRoomPtr>& GetMultiBoundRooms() { return kMultiBoundRooms; }

	UInt16 GetAlwaysRenderArrayCount();
	NiAVObject* GetAlwaysRenderAt(UInt32 auiPos);
	void SetPortalNode(NiNode* apNode);
	NiNode* GetRootNode();
	void ClearMultiBoundRoomAccumList();

	void AddAccumRoom(BSMultiBoundRoom* apRoom);
};

ASSERT_SIZE(BSPortalGraph, 0x78);