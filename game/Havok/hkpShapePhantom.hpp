#pragma once

#include "hkpPhantom.hpp"

class hkVector4;
class hkpLinearCastInput;
class hkpCdPointCollector;
class hkpCdBodyPairCollector;
class hkpCollisionInput;
class hkTransform;

class hkpShapePhantom : public hkpPhantom {
public:
	hkpShapePhantom();
	virtual ~hkpShapePhantom();

	virtual void setPositionAndLinearCast(const hkVector4& position, const hkpLinearCastInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector);
	virtual void setTransformAndLinearCast(const hkTransform& transform, const hkpLinearCastInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector);
	virtual void getClosestPoints(hkpCdPointCollector& collector, const hkpCollisionInput* input = nullptr);
	virtual void getPenetrations(hkpCdBodyPairCollector& collector, const hkpCollisionInput* input = nullptr);
};

ASSERT_SIZE(hkpShapePhantom, 0xA4);