#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiFloatsExtraData);

class NiFloatsExtraData : public NiExtraData {
public:
	NiFloatsExtraData();
	virtual ~NiFloatsExtraData();

	UInt32 m_uiSize;
	float* m_pfValue;

	CREATE_OBJECT(NiFloatsExtraData, 0xA89190);
	NIRTTI_ADDRESS(0x11F5B60);
};

ASSERT_SIZE(NiFloatsExtraData, 0x14)