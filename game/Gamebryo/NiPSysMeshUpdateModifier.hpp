#pragma once

#include "NiPSysModifier.hpp"
#include "NiAVObject.hpp"

NiSmartPointer(NiPSysMeshUpdateModifier);

class NiPSysMeshUpdateModifier : public NiPSysModifier {
public:
	NiPSysMeshUpdateModifier();
	virtual ~NiPSysMeshUpdateModifier();

	NiTObjectArray<NiAVObjectPtr> m_kMeshes;

	CREATE_OBJECT(NiPSysMeshUpdateModifier, 0xC217F0)
};

ASSERT_SIZE(NiPSysMeshUpdateModifier, 0x28);