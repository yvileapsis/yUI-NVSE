#pragma once

#include "NiTriStripsData.hpp"

NiSmartPointer(NiTriStripsDynamicData);

class NiTriStripsDynamicData : public NiTriStripsData {
public:
	NiTriStripsDynamicData();
	~NiTriStripsDynamicData();

	UInt16 m_usActiveVertices;
	UInt16 m_usActiveTriangles;

	CREATE_OBJECT(NiTriStripsDynamicData, 0xA923F0)
};

ASSERT_SIZE(NiTriStripsDynamicData, 0x54)