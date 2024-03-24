#pragma once

#include "bhkWorld.hpp"

ALIGN16 class bhkWorldM : public bhkWorld {
public:
	hkVector4 kWorldTotalSize;
	hkVector4 kBorderSize;
	hkVector4 kBroadPhaseSize;
};

ASSERT_SIZE(bhkWorldM, 0xD0);