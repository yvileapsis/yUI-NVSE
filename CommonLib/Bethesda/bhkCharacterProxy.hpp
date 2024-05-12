#pragma once

#include "bhkSerializable.hpp"
#include "bhkCharacterPointCollector.hpp"

class bhkCharacterProxy : public bhkSerializable {
public:
	bhkCharacterPointCollector pointCollector;
};

ASSERT_SIZE(bhkCharacterProxy, 0x3E0);