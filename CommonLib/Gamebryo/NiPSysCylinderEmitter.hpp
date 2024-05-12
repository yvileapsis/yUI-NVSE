#pragma once

#include "NiPSysVolumeEmitter.hpp"

NiSmartPointer(NiPSysCylinderEmitter);

class NiPSysCylinderEmitter : public NiPSysVolumeEmitter {
public:
	NiPSysCylinderEmitter();
	virtual ~NiPSysCylinderEmitter();

	float m_fEmitterRadius;
	float m_fEmitterHeight;

	CREATE_OBJECT(NiPSysCylinderEmitter, 0xC1FBF0)
};

ASSERT_SIZE(NiPSysCylinderEmitter, 0x60);