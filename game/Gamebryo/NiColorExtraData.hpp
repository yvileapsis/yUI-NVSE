#pragma once

#include "NiExtraData.hpp"
#include "NiColor.hpp"

NiSmartPointer(NiColorExtraData);

class NiColorExtraData : public NiExtraData {
public:
	NiColorExtraData();
	virtual ~NiColorExtraData();

	NiColorA m_kColorA;

	CREATE_OBJECT(NiColorExtraData, 0xA89490);
	NIRTTI_ADDRESS(0x11F5B68);
};

ASSERT_SIZE(NiColorExtraData, 0x1C)