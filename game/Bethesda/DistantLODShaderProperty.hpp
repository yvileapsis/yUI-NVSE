#pragma once

#include "BSShaderLightingProperty.hpp"

class DistantLODShaderProperty : public BSShaderLightingProperty {
public:
	DWORD dword7C;
	DWORD dword80;
	DWORD dword84;
	DWORD dword88;
};

ASSERT_SIZE(DistantLODShaderProperty, 0x8C)