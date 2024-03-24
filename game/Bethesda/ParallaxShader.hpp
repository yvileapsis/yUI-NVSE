#pragma once

#include "ShadowLightShader.hpp"

class NiD3DVertexShader;

class ParallaxShader : public ShadowLightShader {
public:
	virtual void InitPasses();
	virtual void InitPasses_AD();
	virtual void InitPasses_ADT();
	virtual void InitPasses_ADTS();
	virtual void InitPasses_Diffuse();
	virtual void InitPasses_Texture();
	virtual void InitPasses_Specular();

	NiD3DVertexShaderPtr	spVertexShaders[20];
	NiD3DPixelShaderPtr		spPixelShaders[33];

	static ParallaxShader* Create(NiDX9ShaderDeclaration* apDecl);
	static ParallaxShader* __stdcall CreateShader();

	static NiD3DPassPtr* const spPasses;

	static bool __fastcall InitializeEx(ParallaxShader* apThis);
	static void __fastcall SetupTexturesEx(ParallaxShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall RecreateRendererDataEx(ParallaxShader* apThis);
	static void __fastcall SetRenderPassEx(ParallaxShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall Func_62Ex(ParallaxShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static NiDX9ShaderDeclaration* __fastcall GetShaderDeclarationEx(ParallaxShader* apThis, void*, NiGeometry* apGeometry, BSShaderProperty* apShaderProp);
	static void __fastcall ReinitializeEx(ParallaxShader* apThis);
	static void __fastcall ClearAllTextureStagesEx(ParallaxShader* apThis);
	static void __fastcall InitVertexShadersEx(ParallaxShader* apThis);
	static void __fastcall InitPixelShadersEx(ParallaxShader* apThis);
	static bool __fastcall CreatePassesEx(ParallaxShader* apThis);
};

ASSERT_SIZE(ParallaxShader, 0x160);