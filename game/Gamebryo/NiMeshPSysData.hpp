#pragma once

#include "NiPSysData.hpp"
#include "NiAVObject.hpp"
#include "NiNode.hpp"

NiSmartPointer(NiMeshPSysData);

typedef NiTObjectArray<NiAVObjectPtr> NiAVObjectArray;

class NiMeshPSysData : public NiPSysData {
public:
	NiNodePtr							m_spParticleMeshes;
	bool								m_bFillPoolsOnLoad;
	UInt32								m_uiDefaultPoolSize;
	NiTPrimitiveArray<NiAVObjectArray*>	m_kPools;

	CREATE_OBJECT(NiMeshPSysData, 0xC30CA0);
};

ASSERT_SIZE(NiMeshPSysData, 0x98);