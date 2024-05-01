#pragma once

#include "ShadowLightShader.hpp"

class NiD3DVertexShader;
class NiD3DPixelShader;

class SkinShader : public ShadowLightShader {
public:
	virtual void InitPasses();

	NiD3DVertexShaderPtr	spVertexShaders[26];
	NiD3DPixelShaderPtr		spPixelShaders[13];

	static NiD3DPassPtr* const spPasses;

	static SkinShader* Create(NiDX9ShaderDeclaration* apDecl0, NiDX9ShaderDeclaration* apDecl1);
	static SkinShader* __stdcall CreateShader();

	static NiD3DPass* GetPass(UInt32 auID) { return spPasses[auID]; }
	NiD3DVertexShader* GetVertexShader(UInt32 auID) { return spVertexShaders[auID]; };
	NiD3DPixelShader* GetPixelShader(UInt32 auID) { return spPixelShaders[auID]; };

	static bool __fastcall InitializeEx(SkinShader* apThis);
	static void __fastcall SetupTexturesEx(SkinShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall RecreateRendererDataEx(SkinShader* apThis);
	static void __fastcall SetRenderPassEx(SkinShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall Func_62Ex(SkinShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static NiDX9ShaderDeclaration* __fastcall GetShaderDeclarationEx(SkinShader* apThis, void*, NiGeometry* apGeometry, BSShaderProperty* apShaderProperty);
	static void __fastcall InitVertexShadersEx(SkinShader* apThis);
	static void __fastcall InitPixelShadersEx(SkinShader* apThis);
	static bool __fastcall CreatePassesEx(SkinShader* apThis);
	static void __fastcall InitPassesEx(SkinShader* apThis);

	static SkinShader* GetShader();
	static NiD3DPassPtr* GetPasses();
	static NiD3DPass* PickPass(BSShaderManager::RenderPassType aeRenderPass);
};

ASSERT_SIZE(SkinShader, 0x128)