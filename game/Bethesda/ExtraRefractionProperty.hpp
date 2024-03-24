#pragma once

#include "BSExtraData.hpp"

class ExtraRefractionProperty : public BSExtraData {
public:
	EXTRADATATYPE(REFRACTIONPROPERTY);

	float fRefractionAmount;
};

ASSERT_SIZE(ExtraRefractionProperty, 0x10);