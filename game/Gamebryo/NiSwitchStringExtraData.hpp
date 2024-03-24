#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiSwitchStringExtraData);

class NiSwitchStringExtraData : public NiExtraData {
public:
	NiSwitchStringExtraData();
	~NiSwitchStringExtraData();

	UInt32 m_uiSize;
	char** m_ppcValue;
	SInt32 m_iIndex;

	CREATE_OBJECT(NiSwitchStringExtraData, 0xA93870)
};

ASSERT_SIZE(NiSwitchStringExtraData, 0x18);