#pragma once

#include "BSShader.hpp"
#include "Lighting30ShaderProperty.hpp"
#include "NiPoint4.hpp"

class Lighting30Shader : public BSShader {
public:
	virtual void SetupShadowTexture(UInt32 aeRenderPass, Lighting30ShaderProperty* apShaderProperty);
	virtual void SetupEffectTextures(UInt32 aeRenderPass, Lighting30ShaderProperty* apShaderProperty);
	virtual void Func_81(UInt32 aeRenderPass, Lighting30ShaderProperty* apShaderProp);
	virtual void InitShaders();
	virtual void InitVertexShaders();
	virtual void InitPixelShaders();
	virtual void Func_85();

	NiD3DShaderConstantMap* pixelConstantMapTexEffect;
	NiD3DShaderConstantMap* vertexConstantMap;
	NiD3DShaderConstantMap* pPixelConstantMap74;
	NiD3DShaderConstantMap* pVertexConstantMap78;
	NiDX9ShaderDeclaration* pShaderDeclaration7C;
	NiDX9ShaderDeclaration* pShaderDeclaration80;
	NiDX9ShaderDeclaration* pShaderDeclaration84_VC;
	NiDX9ShaderDeclaration* pShaderDeclaration88;
	NiDX9ShaderDeclaration* pShaderDeclaration8C_S;
	NiDX9ShaderDeclaration* pShaderDeclaration90;
	NiDX9ShaderDeclaration* pShaderDeclaration94_VC_S;
	NiDX9ShaderDeclaration* pShaderDeclaration98;
	DWORD					dword9C;
	DWORD					dwordA0;

	static void DisableFog();

	void SetClamp(NiD3DTextureStage* apTextureStage, Lighting30ShaderProperty* apShaderProp);
	void SetupStandardTextures(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp, bool abUseGlowMap);
	void SetNormalMap(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp);
	void SetNormalMap(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp, UInt32 auiStageNumber);
	void SetHairAnisoMap(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp);
	void SetParallaxMap(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp, UInt32 auiStageNumber);
	void SetEnvMask(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp);
	void SetCubeMap(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp);
	void SetTexEffectMap(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp);
	void SetTexEffectMap_ZOnly(NiD3DPass* apPass, Lighting30ShaderProperty* apShaderProp);
	void SetShadowMaps(NiD3DPass* apPass);

	static void __fastcall SetupEffectTexturesEx(Lighting30Shader* apThis, void*, UInt32 aeRenderPass, Lighting30ShaderProperty* apShaderProperty);
	static void __fastcall SetupTexturesEx(Lighting30Shader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetupShadowTextureEx(Lighting30Shader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass, Lighting30ShaderProperty* apShaderProp);

	static Lighting30Shader* GetShader();
	static NiD3DPass**	GetAllPasses();
	static NiD3DPass*	PickPass(BSShaderManager::RenderPassType aeRenderPass);
};

ASSERT_SIZE(Lighting30Shader, 0xA4);

// Custom
class Lighting30ShaderManager {
public:
	struct PixelConstants {
		static __forceinline NiPoint4* GetParallaxData() {
			return &(*(NiPoint4*)0x1201BA0);
		};

		static __forceinline NiPoint4* GetLightData(UInt32 index) {
			return &((NiPoint4*)0x1201C70)[index];
		}
	};
};