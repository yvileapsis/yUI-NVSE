#pragma once

#include "BSOcclusionPlane.hpp"
#include "BSPortalSharedNode.hpp"

class BSMultiBoundRoom;

class BSPortal : public BSOcclusionPlane {
public:
	BSMultiBoundRoom*				pMultiBoundRoom[2];
	NiPointer<BSPortalSharedNode>	spSharedGeometry;

	NIRTTI_ADDRESS(0x12030B0);

	inline BSPortalSharedNode* GetSharedGeometry() const { return spSharedGeometry.m_pObject; };
};

ASSERT_SIZE(BSPortal, 0x108)