#pragma once

#include "NiAnimationKey.hpp"
#include "NiColor.hpp"

NiSmartPointer(NiColorKey);

class NiColorKey : public NiAnimationKey {
public:
	NiColorKey();
	~NiColorKey();

	NiColorA m_Color;
};

ASSERT_SIZE(NiColorKey, 0x14)