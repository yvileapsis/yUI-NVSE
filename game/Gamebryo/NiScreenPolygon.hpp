#pragma once

#include "NiObject.hpp"
#include "NiPropertyState.hpp"
#include "NiPoint2.hpp"
#include "NiPoint3.hpp"
#include "NiColor.hpp"

NiSmartPointer(NiScreenPolygon);

class NiScreenPolygon : public NiObject {
public:
	NiScreenPolygon();
	virtual ~NiScreenPolygon();

	NiPropertyState*	m_spPropertyState;
	UInt16				m_usVertices;
	NiPoint3*			m_pkVertex;
	NiPoint2*			m_pkTexture;
	NiColorA*			m_pkColor;

	CREATE_OBJECT(NiScreenPolygon, 0xA8FFC0)
};

ASSERT_SIZE(NiScreenPolygon, 0x1C)