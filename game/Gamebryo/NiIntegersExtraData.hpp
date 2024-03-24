#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiIntegersExtraData);

class NiIntegersExtraData : public NiExtraData {
public:
	NiIntegersExtraData();
	virtual ~NiIntegersExtraData();

	UInt32	m_uiSize;
	SInt32* m_piValue;

	CREATE_OBJECT(NiIntegersExtraData, 0xA99A10)
};

ASSERT_SIZE(NiIntegersExtraData, 0x14)