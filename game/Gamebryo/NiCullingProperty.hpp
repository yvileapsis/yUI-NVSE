#pragma once

#include "NiProperty.hpp"

NiSmartPointer(NiCullingProperty);

class  NiCullingProperty : public NiProperty {
public:
	UInt16 m_usFlags;

	CREATE_OBJECT(NiCullingProperty, 0xA8F580)
};

ASSERT_SIZE(NiCullingProperty, 0x1C)