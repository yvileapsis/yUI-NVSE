#pragma once

#include "NiPSysEmitter.hpp"
#include "NiAVObject.hpp"

NiSmartPointer(NiPSysVolumeEmitter);

class NiPSysVolumeEmitter : public NiPSysEmitter {
public:
	NiPSysVolumeEmitter();
	virtual ~NiPSysVolumeEmitter();

	NiAVObject* m_pkEmitterObj;
};

ASSERT_SIZE(NiPSysVolumeEmitter, 0x58)