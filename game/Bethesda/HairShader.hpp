#pragma once

#include "ShadowLightShader.hpp"

class NiD3DPass;
class NiD3DVertexShader;
class NiD3DPixelShader;
class NiDX9ShaderDeclaration;

// Unused?
class HairShader : public ShadowLightShader {
public:
	NiD3DPassPtr				spPasses[2];
	NiD3DVertexShaderPtr		spVertexShaders1[7];
	NiD3DPixelShaderPtr			spPixelShaders1[3];
	NiD3DVertexShaderPtr		spVertexShaders2[7];
	NiD3DPixelShaderPtr			spPixelShaders2[3];
	NiDX9ShaderDeclarationPtr	spShaderDeclarationE4;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationE8;
};

ASSERT_SIZE(HairShader, 0xEC)