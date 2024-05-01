#pragma once

#include "NiRefObject.hpp"

NiSmartPointer(TangentSpaceData);

class TangentSpaceData : public NiRefObject {
public:
	bool unk8;
	NiPoint3* pPointsC;
	NiPoint3* pPoints10;
};

ASSERT_SIZE(TangentSpaceData, 0x14);