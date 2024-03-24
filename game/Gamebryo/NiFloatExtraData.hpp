#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiFloatExtraData);

class NiFloatExtraData : public NiExtraData {
public:
	NiFloatExtraData();
	virtual ~NiFloatExtraData();

	float m_fValue;

	CREATE_OBJECT(NiFloatExtraData, 0xA9A480);
	NIRTTI_ADDRESS(0x11F5F40);
};

ASSERT_SIZE(NiFloatExtraData, 0x10)