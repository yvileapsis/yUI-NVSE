#pragma once

#include "NiProperty.hpp"

NiSmartPointer(NiDitherProperty);

class NiDitherProperty : public NiProperty {
public:
	NiDitherProperty();
	virtual ~NiDitherProperty();

	UInt16 m_usflags;

	CREATE_OBJECT(NiDitherProperty, 0xA9A640)
};

ASSERT_SIZE(NiDitherProperty, 0x1C)