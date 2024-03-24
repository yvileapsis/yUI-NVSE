#pragma once

#include "NiPSysModifier.hpp"

NiSmartPointer(BSPSysStripUpdateModifier);

class BSPSysStripUpdateModifier : public NiPSysModifier {
public:
	BSPSysStripUpdateModifier();
	~BSPSysStripUpdateModifier();

	float fUpdateDeltaTime;

	CREATE_OBJECT(BSPSysStripUpdateModifier, 0xC5FEE0)
};

ASSERT_SIZE(BSPSysStripUpdateModifier, 0x1C)