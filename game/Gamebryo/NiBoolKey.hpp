#pragma once

#include "NiAnimationKey.hpp"
#include "NiSmartPointer.hpp"

NiSmartPointer(NiBoolKey);

class NiBoolKey : public NiAnimationKey {
public:
	bool m_bBool;
};

ASSERT_SIZE(NiBoolKey, 0x8);