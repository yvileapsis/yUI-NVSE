#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiStringsExtraData);

class NiStringsExtraData : public NiExtraData {
public:
	NiStringsExtraData();
	virtual ~NiStringsExtraData();

	UInt32 m_uiSize;
	char** m_ppcValue;

	CREATE_OBJECT(NiStringsExtraData, 0xA94930);
	NIRTTI_ADDRESS(0x11F5EBC);
};

ASSERT_SIZE(NiStringsExtraData, 0x14);