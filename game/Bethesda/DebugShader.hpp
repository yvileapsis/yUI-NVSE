#pragma once

#include "BSShader.hpp"
#include "BSShaderManager.hpp"
#include "NiRenderedTexture.hpp"
#include "NiRenderedCubeMap.hpp"
#include "NiScreenElements.hpp"

NiSmartPointer(DebugShader);

class DebugShader : public BSShader {
public:
	NiRenderedTexturePtr	spTexture[16];
	NiScreenElementsPtr		spScreenElements;
	NiD3DVertexShaderPtr	spVertexShader;
	NiD3DPixelShaderPtr		spPixelShaders[2];
	UInt32					uiNumTextures;

	static DebugShader* GetShader();
	static void ShowRenderTarget(NiTexture* pTexture);
	static void ShowRenderTarget(NiRenderedCubeMap* pTexture);
	static void ShowRenderTarget(BSRenderedTexture* pTexture);
};

ASSERT_SIZE(DebugShader, 0xC0);