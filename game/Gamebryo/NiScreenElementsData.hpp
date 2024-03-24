#pragma once

#include "NiTriShapeData.hpp"

NiSmartPointer(NiScreenElementsData);

class NiScreenElementsData : public NiTriShapeData {
public:
	NiScreenElementsData();
	virtual ~NiScreenElementsData();

	class Polygon : public NiMemObject {
	public:
		UInt16 m_usNumVertices;
		UInt16 m_usVOffset;
		UInt16 m_usNumTriangles;
		UInt16 m_usIOffset;
	};

	Polygon*	m_akPolygon;
	UInt16*		m_ausPIndexer;
	UInt16		m_usMaxPQuantity;
	UInt16		m_usPGrowBy;
	UInt16		m_usPQuantity;
	UInt16		m_usMaxVQuantity;
	UInt16		m_usVGrowBy;
	UInt16		m_usMaxIQuantity;
	UInt16		m_usIGrowBy;
	bool		m_bBoundNeedsUpdate;

	CREATE_OBJECT(NiScreenElementsData, 0xA87920)
};

ASSERT_SIZE(NiScreenElementsData, 0x70)