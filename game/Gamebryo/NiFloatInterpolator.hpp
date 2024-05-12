#pragma once

#include "NiKeyBasedInterpolator.hpp"
#include "NiFloatData.hpp"

class NiFloatInterpolator : public NiKeyBasedInterpolator {
public:
	NiFloatInterpolator();
	virtual ~NiFloatInterpolator();

	float			m_fFloatValue;
	NiFloatData*	m_spFloatData;
	UInt32			m_uiLastIdx;
};

ASSERT_SIZE(NiFloatInterpolator, 0x18);