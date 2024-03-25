#pragma once
#include "TESBoundObject.hpp"

// 0x30
class TESBoundAnimObject : public TESBoundObject
{
public:
	TESBoundAnimObject();
	~TESBoundAnimObject() override;
};
static_assert(sizeof(TESBoundAnimObject) == 0x30);