#pragma once

#include "hkpCollidable.hpp"
#include "hkArray.hpp"

class hkpLinkedCollidable : public hkpCollidable {
	hkArray<void> m_collisionEntries;
};

ASSERT_SIZE(hkpLinkedCollidable, 0x5C);