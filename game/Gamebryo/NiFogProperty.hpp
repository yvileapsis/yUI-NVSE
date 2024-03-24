#pragma once

#include "NiProperty.hpp"
#include "NiColor.hpp"

NiSmartPointer(NiFogProperty);

class NiFogProperty : public NiProperty {
public:
	NiFogProperty();
	virtual ~NiFogProperty();

	UInt16	m_flags;
	float	m_fDepth;
	NiColor m_kColor;

	CREATE_OBJECT(NiFogProperty, 0xA99FD0)
};

ASSERT_SIZE(NiFogProperty, 0x2C)