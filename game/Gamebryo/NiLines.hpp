#pragma once

#include "NiGeometry.hpp"

NiSmartPointer(NiLines);

class NiLines : public NiGeometry {
public:
	NiLines();
	virtual ~NiLines();

	CREATE_OBJECT(NiLines, 0xA74750);
	static NiLines *Create(UInt16 usVertices, NiPoint3* pkVertex, NiColorA* pkColor, NiPoint2* pkTexture, UInt16 usNumTextureSets, UInt32 eNBTMethod, bool* pkFlags);
};

ASSERT_SIZE(NiLines, 0xC4)