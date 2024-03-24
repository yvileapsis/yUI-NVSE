#pragma once

#include "hkVector4.hpp"

class hkaRaycastInterface {
public:
	hkaRaycastInterface();

	virtual			~hkaRaycastInterface();
	virtual bool*	castRay(bool*, hkVector4*, hkVector4*, UInt32, float, hkVector4*);
	virtual bool*	castRayAlt(bool*, hkVector4*, hkVector4*, UInt32, hkVector4*);
};

ASSERT_SIZE(hkaRaycastInterface, 0x4);