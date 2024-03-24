#pragma once

#include "BSShader.hpp"

class NiD3DPass;
class NiD3DVertexShader;
class NiD3DPixelShader;

class BSShaderBloodSplatter : public BSShader {
public:
	NiD3DPass* passes[2];
	NiD3DVertexShader* vertexShaders[2];
	NiD3DPixelShader* pixelShaders[2];

	static void ComputeScreenSpaceLightLoc(NiCamera* apCamera) { CdeclCall(0xB803B0, apCamera); }
};