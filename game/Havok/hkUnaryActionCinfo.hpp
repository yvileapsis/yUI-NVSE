#pragma once

#include "hkActionCinfo.hpp"

class hkpRigidBody;

class hkUnaryActionCinfo : public hkActionCinfo {
public:
	hkpRigidBody* m_body;
};

ASSERT_SIZE(hkUnaryActionCinfo, 0x8);