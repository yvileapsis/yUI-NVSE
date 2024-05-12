#pragma once

#include "NiGeometryData.hpp"
#include "NiQuaternion.hpp"

NiSmartPointer(NiParticlesData);

class NiParticlesData : public NiGeometryData {
public:
	NiParticlesData();
	virtual ~NiParticlesData();

	virtual void RemoveParticle(UInt16 usParticle);

	UInt8			byte40;
	DWORD			dword4C;
	UInt16			word50;
	float*			m_pfRadii;
	UInt16			m_usActive;
	float*			m_pfSizes;
	NiQuaternion*	m_pkRotations;
	float*			m_pfRotationAngles;
	NiPoint3*		m_pkRotationAxes;
	UInt8*			m_pucTextureIndices;
	DWORD			m_pkSubTextureOffsets;
	UInt8			m_ucSubTextureOffsetCount;

	CREATE_OBJECT(NiParticlesData, 0xA96600)
};

ASSERT_SIZE(NiParticlesData, 0x70);