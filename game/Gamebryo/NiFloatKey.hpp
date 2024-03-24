#pragma once

#include "NiAnimationKey.hpp"
#include "NiSmartPointer.hpp"

NiSmartPointer(NiFloatKey);

class NiFloatKey : public NiAnimationKey {
public:
	float m_fValue;
	float ms_fDefault;
};

ASSERT_SIZE(NiFloatKey, 0xC);