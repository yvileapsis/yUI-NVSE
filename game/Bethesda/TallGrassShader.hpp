#pragma once

#include "BSShader.hpp"
#include "NiPoint4.hpp"

#include "TallGrassShaderProperty.hpp"

class TallGrassShader : public BSShader {
public:
	virtual void InitShaders();
	virtual void InitPasses();

	NiD3DPassPtr				spPasses[44];
	float						fWindTime;
	int							unk120;
	bool						unk124;
	NiD3DVertexShaderPtr		spVertexShaders[24];
	NiD3DPixelShaderPtr			spPixelShaders[7];
	NiPoint4*					pInstanceData;
	NiShaderConstantMapEntry*	pInstanceDataEntry;
	UInt16						ucInstanceCount;
	NiColorA					kLightColor;
	NiPoint4					kLightData;
	NiColorA					kAlphaTestRef;

	static TallGrassShader* Create(NiDX9ShaderDeclaration* apShaderDeclaration);
	static TallGrassShader* __stdcall CreateShader();

	NiD3DPass*			GetPass(UInt32 auID) { return spPasses[auID]; }
	NiD3DVertexShader*	GetVertexShader(UInt32 auID) { return spVertexShaders[auID]; };
	NiD3DPixelShader*	GetPixelShader(UInt32 auID) { return spPixelShaders[auID]; };

	static void __fastcall InitializeEx(TallGrassShader* apThis);
	static UInt32 __fastcall PreProcessPipelineEx(TallGrassShader* apThis, void*, NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, NiGeometryBufferData* pkRendererData, const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	static UInt32 __fastcall SetupTransformationsEx(TallGrassShader* apThis, void*, NiGeometry* pkGeometry, const NiSkinInstance* pkSkin, const NiSkinPartition::Partition* pkPartition, NiGeometryBufferData* pkRendererData, const NiPropertyState* pkState, const NiDynamicEffectState* pkEffects, const NiTransform& kWorld, const NiBound& kWorldBound);
	static void __fastcall SetupTexturesEx(TallGrassShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall UpdateConstantsEx(TallGrassShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetRenderPassEx(TallGrassShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall InitShaderConstantsEx(TallGrassShader* apThis);
	static void __fastcall ReinitializeEx(TallGrassShader* apThis);
	static void __fastcall ClearAllTextureStagesEx(TallGrassShader* apThis);
	static void __fastcall InitShadersEx(TallGrassShader* apThis);
	static bool __fastcall InitPassesEx(TallGrassShader* apThis);

	void SetupPasses();
	void SetupShadowData(const NiSkinInstance* apSkinInstance, const NiTransform& akTransform);
	void SetupLightData(TallGrassShaderProperty* apShaderProperty, D3DXMATRIX& akWorldMatrix, float afScale, UInt32 auiRenderPassType);
	void UpdateInstanceData(TallGrassShaderProperty* apShaderProperty);

	static TallGrassShader* GetShader();
	static NiD3DPass* PickPass(BSShaderManager::RenderPassType aeRenderPass, TallGrassShader* apShader = nullptr);

	struct VertexConstants {
		struct DrawDistanceSettings {
			float		fGrass_11F9EE0;
			float		fGrass_11F9EE4;
			float		fGrassStartFadeDistance;
			float		fGrassFadeRange;
		};

		struct ConstantGroup {
			NiPoint4				kModelViewProj;
			NiColorA				kDiffuseColor;
			NiPoint4				kScaleMask;
			NiPoint4				kPointLighting;
			NiPoint2				kGrassSway;
			NiPoint2				kGrassWindMagnitude;
			DrawDistanceSettings	kDrawDistances;
			NiColorA				kAmbientColor;
			NiColorA				kDimmer;
			NiPoint4				kShadowProjData;
		};

		static ConstantGroup* const GroupedConstants;

	};
};