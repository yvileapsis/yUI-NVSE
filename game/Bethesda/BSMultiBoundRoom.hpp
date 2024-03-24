#pragma once

#include "BSMultiBoundNode.hpp"
#include "NiTPointerList.hpp"
#include "BSSimpleArray.hpp"

class BSPortal;
class ShadowSceneLight;
class BSCompoundFrustum;
class BSOcclusionPlane;

NiSmartPointer(BSMultiBoundRoom);

class BSMultiBoundRoom : public BSMultiBoundNode {
public:
	BSMultiBoundRoom();
	virtual ~BSMultiBoundRoom();

	NiTPointerList<BSPortal*>			kPortals;
	NiTPointerList<BSOcclusionPlane*>	kOccluders;
	BSCompoundFrustum*					pFrustum;
	NiTPointerList<BSMultiBound*>		kJoinedMultiBounds;
	BSSimpleArray<ShadowSceneLight*>	kLights;

	NIRTTI_ADDRESS(0x1202828);

	inline BSCompoundFrustum* GetFrustum() const { return pFrustum; };
	inline NiTPointerList<BSPortal*>& GetPortals() { return kPortals; };
	inline NiTPointerList<BSOcclusionPlane*>& GetOccluders() { return kOccluders; };
	inline NiTPointerList<BSMultiBound*>& GetJoinedMultiBounds() { return kJoinedMultiBounds; };
	inline BSSimpleArray<ShadowSceneLight*>& GetLights() { return kLights; };

	static void __fastcall OnVisibleEx(BSMultiBoundRoom* apThis, void*, BSCullingProcess* apCuller);
	static void __fastcall UpdateWorldBoundEx(BSMultiBoundRoom* apThis);

	void SetCullResult(BSMultiBoundShape::BSMBCullResult aeCullResult);

	void SetFrustum(BSCompoundFrustum* apFrustum);
	void RemoveFrustum();
};

ASSERT_SIZE(BSMultiBoundRoom, 0xEC);
