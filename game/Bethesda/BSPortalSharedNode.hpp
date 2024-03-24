#pragma once

#include "NiNode.hpp"
#include "BSSimpleArray.hpp"

class BSCompoundFrustum;
class BSPortal;
class BSCullingProcess;
class ShadowSceneLight;

class BSPortalSharedNode : public NiNode {
public:
	BSPortalSharedNode();
	virtual ~BSPortalSharedNode();

	BSCompoundFrustum*					pCompoundFrustum;
	BSSimpleArray<ShadowSceneLight*>	kLights;
	BSPortal*							pPortal;

	NIRTTI_ADDRESS(0x12030B8);

	inline BSCompoundFrustum* GetFrustum() const { return pCompoundFrustum; };
	inline BSPortal* GetPortal() const { return pPortal; };
	inline BSSimpleArray<ShadowSceneLight*>& GetLights() { return kLights; };

	static void __fastcall OnVisibleEx(BSPortalSharedNode* apThis, void*, BSCullingProcess* apCuller);
};

ASSERT_SIZE(BSPortalSharedNode, 0xC4);