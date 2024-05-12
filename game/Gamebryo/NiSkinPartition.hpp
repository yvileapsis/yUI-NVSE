#pragma once

#include "NiObject.hpp"

NiSmartPointer(NiSkinPartition);

class NiGeometryBufferData;

class NiSkinPartition : public NiObject {
public:
	NiSkinPartition();
	virtual ~NiSkinPartition();

	class Partition {
	public:
		virtual ~Partition();

		UInt16*					m_pusBones;
		float*					m_pfWeights;
		UInt16*					m_pusStripLengths;
		UInt16*					m_pusVertexMap;
		UInt8*					m_pucBonePalette;
		UInt16*					m_pusTriList;
		UInt16					m_usVertices;
		UInt16					m_usTriangles;
		UInt16					m_usBones;
		UInt16					m_usStrips;
		UInt16					m_usBonesPerVertex;
		NiGeometryBufferData*	m_pkBuffData;
	};

	UInt32		m_uiPartitions;
	Partition*	m_pkPartitions;

	CREATE_OBJECT(NiSkinPartition, 0xA81FB0)
};

ASSERT_SIZE(NiSkinPartition, 0x10);