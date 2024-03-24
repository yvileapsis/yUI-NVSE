#pragma once

#include "NiPSysVolumeEmitter.hpp"

NiSmartPointer(NiPSysBoxEmitter);

class NiPSysBoxEmitter : public NiPSysVolumeEmitter {
public:
	NiPSysBoxEmitter();
	virtual ~NiPSysBoxEmitter();

	float m_fEmitterWidth;
	float m_fEmitterHeight;
	float m_fEmitterDepth;

	CREATE_OBJECT(NiPSysBoxEmitter, 0xC20350)
};

ASSERT_SIZE(NiPSysBoxEmitter, 0x64);