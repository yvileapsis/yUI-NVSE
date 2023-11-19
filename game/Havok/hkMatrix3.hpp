#pragma once

#include "hkVector4.hpp"

class hkMatrix3f {
public:
	hkVector4 m_col0;
	hkVector4 m_col1;
	hkVector4 m_col2;
};

ASSERT_SIZE(hkMatrix3f, 0x30);