#pragma once

#include "hkVector4.hpp"

class hkaRagdollRaycastInterface {
public:
	hkaRagdollRaycastInterface();
	virtual ~hkaRagdollRaycastInterface();
	virtual bool castRay(bool* result, const class hkpAabbPhantom* phantomIn, const hkVector4& fromWS, const hkVector4& toWS, hkVector4& hitPointWS, hkVector4& hitNormalWS);
	virtual bool castRayAlt(bool* result, const hkVector4& fromWS, const hkVector4& toWS, float& hitFractionOut, hkVector4& normalWSOut);
};

ASSERT_SIZE(hkaRagdollRaycastInterface, 0x4);