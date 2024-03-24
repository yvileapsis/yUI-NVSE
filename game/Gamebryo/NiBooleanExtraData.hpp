#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiBooleanExtraData);

class NiBooleanExtraData : public NiExtraData {
public:
	NiBooleanExtraData();
	virtual ~NiBooleanExtraData();

	bool m_bValue;

	CREATE_OBJECT(NiBooleanExtraData, 0xA9AF90);
	NIRTTI_ADDRESS(0x11F5F5C);
};

ASSERT_SIZE(NiBooleanExtraData, 0x10)