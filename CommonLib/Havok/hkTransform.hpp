#pragma once

#include "hkVector4.hpp"
#include "hkMatrix3x4.hpp"

class hkTransform {
public:
	hkMatrix3x4 rotation;
	hkVector4	translation;
};

ASSERT_SIZE(hkTransform, 0x40)