#pragma once

#include "BSShader.hpp"
#include "NiPoint4.hpp"
#include "BSShaderNoLightingProperty.hpp"

class BSShaderNoLighting : public BSShader {
public:
	NiD3DPassPtr				spPasses[30];
	NiD3DVertexShaderPtr		spVertexShaders[28];
	NiD3DPixelShaderPtr			spPixelShaders[7];
	NiShaderConstantMapEntry*	pSubtex;
	NiShaderConstantMapEntry*	pWorldTranspose;
	NiShaderConstantMapEntry*	pWorldViewTranspose;
	NiShaderConstantMapEntry*	pEyeDir;
	NiShaderConstantMapEntry*	pBoneMatrix3;
	NiDX9ShaderDeclarationPtr	spShaderDeclarations[12];

	static BSShaderNoLighting* Create(NiDX9ShaderDeclaration* apDecl0, NiDX9ShaderDeclaration* apDecl1, NiDX9ShaderDeclaration* apDecl2, NiDX9ShaderDeclaration* apDecl3,
		NiDX9ShaderDeclaration* apDecl4, NiDX9ShaderDeclaration* apDecl5, NiDX9ShaderDeclaration* apDecl6, NiDX9ShaderDeclaration* apDecl7, NiDX9ShaderDeclaration* apDecl8,
		NiDX9ShaderDeclaration* apDecl9, NiDX9ShaderDeclaration* apDecl10, NiDX9ShaderDeclaration* apDecl11);
	static BSShaderNoLighting* __stdcall CreateShader();
	void InitShaders();
	bool InitPasses();

	static void __fastcall SetupTexturesEx(BSShaderNoLighting* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall UpdateConstantsEx(BSShaderNoLighting* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall PostRenderSetupEx(BSShaderNoLighting* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetRenderPassEx(BSShaderNoLighting* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static NiDX9ShaderDeclaration* __fastcall GetShaderDeclarationEx(BSShaderNoLighting* apThis, void*, NiGeometry* apGeometry, BSShaderNoLightingProperty* apShaderProperty);
	static void __fastcall InitShaderConstantsEx(BSShaderNoLighting* apThis);
	static void __fastcall ReinitializeEx(BSShaderNoLighting* apThis);
	static void __fastcall ClearAllTextureStagesEx(BSShaderNoLighting* apThis);

	NiD3DPass* GetPass(UInt32 auID) const { return spPasses[auID]; }
	NiD3DVertexShader* GetVertexShader(UInt32 auID) const { return spVertexShaders[auID]; };
	NiD3DPixelShader* GetPixelShader(UInt32 auID) const { return spPixelShaders[auID]; };
	NiDX9ShaderDeclaration* GetStoredShaderDeclaration(UInt32 auID) const { return spShaderDeclarations[auID]; };

	void SetTextureTransform(NiPropertyState* apProperties);
	void SetFogParams();
	void UpdateMaterialColor(BSShaderNoLightingProperty* apShaderProp, NiMaterialProperty* apMaterialProp);


	static BSShaderNoLighting* GetShader();
	static NiD3DPass* PickPass(BSShaderManager::RenderPassType aeRenderPass, BSShaderNoLighting* apShader = nullptr);

	struct VertexConstants {
		static FogProperties* const Fog;
		static BSShaderNoLightingProperty::FalloffData* const Falloff;

		static D3DMATRIX* const kTextureTransform;

		static float* GetSubTex();
	};

	struct PixelConstants {
		static NiColorA* const MatColor;
		static NiPoint4* const MatAlpha;
		static FogProperties* const WaterFog;
	};
};

ASSERT_SIZE(BSShaderNoLighting, 0x1B4);