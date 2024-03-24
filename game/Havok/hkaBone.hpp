#pragma once

#include "hkStringPtr.hpp"

class hkaBone {
public:
	hkStringPtr m_name;
	bool		m_lockTranslation;
};

ASSERT_SIZE(hkaBone, 0x8);