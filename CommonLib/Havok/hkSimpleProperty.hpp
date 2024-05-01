#pragma once

#include "hkSimplePropertyValue.hpp"

class hkSimpleProperty {
public:
	UInt32					m_key;
	UInt32					m_alignmentPadding;
	hkSimplePropertyValue	m_value;
};

ASSERT_SIZE(hkSimpleProperty, 0x10);