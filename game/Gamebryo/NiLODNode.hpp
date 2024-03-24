#pragma once

#include "NiSwitchNode.hpp"
#include "NiLODData.hpp"

NiSmartPointer(NiLODNode);

class NiLODNode : public NiSwitchNode {
public:
	NiLODNode();
	virtual ~NiLODNode();

	NiLODDataPtr	m_spLODData;
	bool			m_bLODActive;

	CREATE_OBJECT(NiLODNode, 0xA99710);

	static SInt32 ms_iGlobalLOD;

	static void __fastcall OnVisibleEx(NiLODNode* apThis, void*, NiCullingProcess* apCuller);
};

ASSERT_SIZE(NiLODNode, 0xD4)