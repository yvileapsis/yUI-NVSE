#pragma once

#include "NiNode.hpp"
#include "NiPlane.hpp"

NiSmartPointer(NiBSPNode);

class NiBSPNode : public NiNode {
public:
	NiBSPNode();
	virtual ~NiBSPNode();

	NiPlane m_kModelPlane;
	NiPlane m_kWorldPlane;

	CREATE_OBJECT(NiBSPNode, 0xA9AEA0);
	NIRTTI_ADDRESS(0x11F5F54);

	static void __fastcall OnVisibleEx(NiBSPNode* apThis, void*, NiCullingProcess* apCuller);
};

ASSERT_SIZE(NiBSPNode, 0xCC)