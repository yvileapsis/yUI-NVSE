#pragma once

#include "bhkWorldObject.hpp"

class bhkPhantom : public bhkWorldObject {
public:
	bool unk14;
};

ASSERT_SIZE(bhkPhantom, 0x18);