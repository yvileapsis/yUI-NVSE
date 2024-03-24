#pragma once

#include "NiTriShapeData.hpp"

NiSmartPointer(NiTriShapeDynamicData);

class NiTriShapeDynamicData : public NiTriShapeData {
public:
	NiTriShapeDynamicData();
	~NiTriShapeDynamicData();

	UInt16 m_usActiveVertices;
	UInt16 m_usActiveTriangles;

	CREATE_OBJECT(NiTriShapeDynamicData, 0xA81760)
};

ASSERT_SIZE(NiTriShapeDynamicData, 0x5C)