#pragma once

#include "BSShader.hpp"
#include "NiColor.hpp"

class PrecipitationShader : public BSShader {
public:
	NiPoint3 Velocity;
	NiPoint3 MinPos;
	NiPoint3 MaxPos;
	NiPoint3 Params;
	DWORD dword9C;
	NiColor AmbientColor;
};

ASSERT_SIZE(PrecipitationShader, 0xAC)