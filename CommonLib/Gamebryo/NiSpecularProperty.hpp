#pragma once

#include "NiProperty.hpp"

NiSmartPointer(NiSpecularProperty);

class NiSpecularProperty : public NiProperty {
public:
	NiSpecularProperty();
	~NiSpecularProperty();

	UInt16 m_usflags;

	CREATE_OBJECT(NiSpecularProperty, 0xA95160)
};

ASSERT_SIZE(NiSpecularProperty, 0x1C);