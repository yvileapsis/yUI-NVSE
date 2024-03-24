#pragma once

#include "hkpCdPointCollector.hpp"
#include "hkInplaceArray.hpp"
#include "hkpRootCdPoint.hpp"
#include "NiPoint4.hpp"

class hkCdBody;

class hkpAllCdPointCollector : public hkpCdPointCollector {
public:
	virtual void addCdPointAlt(const hkpCdPoint& event);

	ALIGN16 struct UnkData {
		NiPoint4	point00;
		NiPoint4	point10;
		hkCdBody*	cdBody1;
		UInt8		body1Key;
		UInt8		obj1Flag;
		NiPoint4	point28;
		NiPoint4	point38;
		hkCdBody*	cdBody2;
		UInt8		body2Key;
		UInt8		obj2Flag;
		NiPoint4	point50;
		NiPoint4	point60;
	};

	hkInplaceArray<UnkData, 8> m_hits;
};

ASSERT_SIZE(hkpAllCdPointCollector, 0x3A0);
ASSERT_SIZE(hkpAllCdPointCollector::UnkData, 0x70);