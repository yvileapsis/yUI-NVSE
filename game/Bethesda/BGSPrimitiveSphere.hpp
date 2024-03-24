#pragma once

#include "BGSPrimitive.hpp"

class BGSPrimitiveSphere : public BGSPrimitive {
public:
	static BGSPrimitiveSphere* __cdecl CreateObject(float afRadius, NiColorA& apColor);

	void SetRadius(float afRadius);
};
