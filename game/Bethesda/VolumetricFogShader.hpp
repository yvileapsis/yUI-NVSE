#pragma once

#include "BSShader.hpp"
#include "NiPoint4.hpp"

// Unused
class VolumetricFogShader : public BSShader {
public:
	virtual void InitShaders();
	virtual void InitVertexShaders();
	virtual void InitPixelShaders();
	virtual bool InitPasses();

	NiD3DPassPtr			spPasses[3];
	NiD3DVertexShaderPtr	spVertexShaders[2];
	NiD3DPixelShaderPtr		spPixelShaders[2];
	NiPoint4				fvars0;
	NiPoint4				FogProps;

	static VolumetricFogShader* Create();
	static VolumetricFogShader* __stdcall CreateShader();
	static VolumetricFogShader* GetShader();

	NiD3DPass* GetPass(UInt32 auID) { return spPasses[auID]; }
	NiD3DVertexShader* GetVertexShader(UInt32 auID) { return spVertexShaders[auID]; };
	NiD3DPixelShader* GetPixelShader(UInt32 auID) { return spPixelShaders[auID]; };

	static NiD3DPass* PickPass(BSShaderManager::RenderPassType aeRenderPass);

	static bool __fastcall InitializeEx(VolumetricFogShader* apThis);
	static void __fastcall SetupTexturesEx(VolumetricFogShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall UpdateConstantsEx(VolumetricFogShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetRenderPassEx(VolumetricFogShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall InitShaderConstantsEx(VolumetricFogShader* apThis);
	static void __fastcall ReinitializeEx(VolumetricFogShader* apThis);
	static void __fastcall ClearAllTextureStagesEx(VolumetricFogShader* apThis);
	static void __fastcall InitVertexShadersEx(VolumetricFogShader* apThis);
	static void __fastcall InitPixelShadersEx(VolumetricFogShader* apThis);
	static bool __fastcall InitPassesEx(VolumetricFogShader* apThis);
};

ASSERT_SIZE(VolumetricFogShader, 0xA8)