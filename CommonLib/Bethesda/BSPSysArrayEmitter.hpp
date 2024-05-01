#pragma once

#include "NiPSysVolumeEmitter.hpp"
#include "NiTArray.hpp"

NiSmartPointer(BSPSysArrayEmitter);

class BSPSysArrayEmitter : public NiPSysVolumeEmitter {
public:
	BSPSysArrayEmitter();
	~BSPSysArrayEmitter();

	UInt32 dword58;
	NiTPrimitiveArray<NiPointer<NiAVObject>> objects;

	CREATE_OBJECT(BSPSysArrayEmitter, 0xC61E70)
};

ASSERT_SIZE(BSPSysArrayEmitter, 0x6C)