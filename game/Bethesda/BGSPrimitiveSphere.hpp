#pragma once
#include "BGSPrimitive.hpp"

// 0x34
class BGSPrimitiveSphere : public BGSPrimitive
{
public:
	static BGSPrimitiveSphere* __cdecl CreateObject(float afRadius, NiColorA& apColor);

	void SetRadius(float afRadius);
};
static_assert(sizeof(BGSPrimitiveSphere) == 0x34);