#pragma once

#include "NiTriShapeData.hpp"
#include "NiTArray.hpp"

NiSmartPointer(NiScreenGeometryData);

class NiScreenGeometryData : public NiTriShapeData {
public:
	NiScreenGeometryData();
	virtual ~NiScreenGeometryData();

	class ScreenElement : public NiMemObject {
	public:
		UInt16		m_usNumVerts;
		UInt16		m_usNumTexCoordSets;
		NiPoint2*	m_pkVerts;
		NiColorA*	m_pkColors;
		NiPoint2*	m_pkTexCoords;
	};

	bool													m_bPixelAccurate;
	bool													m_bUnorderedElements;
	UInt32													m_uiActiveVerts;
	NiTPrimitiveArray<NiScreenGeometryData::ScreenElement*> m_kScreenElements;

	CREATE_OBJECT(NiScreenGeometryData, 0xA90B20)
};

ASSERT_SIZE(NiScreenGeometryData, 0x70);