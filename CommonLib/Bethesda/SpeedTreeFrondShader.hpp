#pragma once

#include "BSShader.hpp"
#include "NiPoint4.hpp"

class SpeedTreeFrondShader : public BSShader {
public:
	NiD3DVertexShaderPtr	spVertexShaders[4];
	NiD3DPixelShaderPtr		spPixelShaders[2];
	NiD3DPassPtr			spShaderPass;
	NiPoint4				LightPos;
	NiPoint4				LightVector;
};

ASSERT_SIZE(SpeedTreeFrondShader, 0xA8)