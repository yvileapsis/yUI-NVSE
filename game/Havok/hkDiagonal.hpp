#pragma once

#include "hkVector4.hpp"

class hkDiagonal {
public:
	hkDiagonal();
	hkDiagonal(hkVector4& akPoint0, hkVector4& akPoint1) : pt0(akPoint0), pt1(akPoint1) {}

	hkVector4	pt0;
	hkVector4	pt1;
};

ASSERT_SIZE(hkDiagonal, 0x20)