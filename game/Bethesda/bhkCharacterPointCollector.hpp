#pragma once

#include "hkpAllCdPointCollector.hpp"

NiSmartPointer(hkpWorldObject);

ALIGN16 class bhkCharacterPointCollector : public hkpAllCdPointCollector {
public:
	bhkCharacterPointCollector* pStartCollector;
	hkArray<hkpWorldObjectPtr>	contactBodies;
	hkArray<UInt32>				arr3B0;
	hkArray<float>				arr3BC;
};

ASSERT_SIZE(bhkCharacterPointCollector, 0x3D0);