#pragma once

#include "NiPSysEmitterCtlr.hpp"

NiSmartPointer(BSPSysMultiTargetEmitterCtlr);

class BSPSysMultiTargetEmitterCtlr : public NiPSysEmitterCtlr {
public:
	BSPSysMultiTargetEmitterCtlr();
	~BSPSysMultiTargetEmitterCtlr();

	UInt32 unk5C;
	UInt16 unk60;

	CREATE_OBJECT(BSPSysMultiTargetEmitterCtlr, 0xC5CBB0);
};

ASSERT_SIZE(BSPSysMultiTargetEmitterCtlr, 0x64)