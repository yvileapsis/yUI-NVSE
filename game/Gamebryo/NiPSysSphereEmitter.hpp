#pragma once

#include "NiPSysVolumeEmitter.hpp"

NiSmartPointer(NiPSysSphereEmitter);

class NiPSysSphereEmitter : public NiPSysVolumeEmitter {
public:
	NiPSysSphereEmitter();
	virtual ~NiPSysSphereEmitter();

	float m_fEmitterRadius;

	CREATE_OBJECT(NiPSysSphereEmitter, 0xC1FFF0)
};

ASSERT_SIZE(NiPSysSphereEmitter, 0x5C);