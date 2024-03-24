#pragma once

#include "hkVector4.hpp"
#include "hkpWorldRayCastInput.hpp"
#include "hkpWorldRayCastOutput.hpp"

class hkpRayHitCollector;
class SpecificItemCollector;

class bhkPickData : public hkpWorldRayCastInput, public hkpWorldRayCastOutput {
public:
	hkVector4				hkvector4f90;
	char*					pCache;
	SpecificItemCollector*	pSpecificItemCollector;
	hkpRayHitCollector*		pRayHitCollector;
	BYTE					byteAC;

	static bhkPickData* Create(bhkPickData* apThis);

	void NiSetFrom(const NiPoint3& arPoint);
	void NiSetTo(const NiPoint3& arPoint);

	void SetHitCollector(hkpRayHitCollector* apRayHitCollector);
	void SetSpecificItemCollector(SpecificItemCollector* apSpecificItemCollector);
};

ASSERT_SIZE(bhkPickData, 0xB0);