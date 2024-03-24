#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiBinaryExtraData);

class NiBinaryExtraData : public NiExtraData {
public:
	NiBinaryExtraData();
	virtual ~NiBinaryExtraData();

	UInt8* m_pData;
	UInt32 m_uiSize;

	CREATE_OBJECT(NiBinaryExtraData, 0xA7F8E0)
};

ASSERT_SIZE(NiBinaryExtraData, 0x14)