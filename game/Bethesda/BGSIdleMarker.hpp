#pragma once

#include "TESBoundObject.hpp"
#include "BGSIdleCollection.hpp"

class BGSIdleMarker : public TESBoundObject, public BGSIdleCollection {
public:
};

ASSERT_SIZE(BGSIdleMarker, 0x40);