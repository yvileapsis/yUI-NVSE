#pragma once

#include "hkSerializableCinfo.hpp"

class hkActionCinfo : public hkSerializableCinfo {
public:
	UInt32 m_userData;
};

ASSERT_SIZE(hkActionCinfo, 0x4);