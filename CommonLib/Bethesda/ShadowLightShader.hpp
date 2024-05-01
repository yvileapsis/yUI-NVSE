#pragma once

#include "BSShader.hpp"
#include "NiPoint4.hpp"
#include "NiShaderConstantMapEntry.hpp"
#include "BSShaderPPLightingProperty.hpp"

class NiD3DShaderConstantMap;
class BSFogProperty;

class ShadowLightShader : public BSShader {
public:
	virtual void SetupShadowTexture(BSShaderManager::RenderPassType aeRenderPass, BSShaderPPLightingProperty* apShaderProp);
	virtual void Func_80(); // nullsub
	virtual void InitShaders();
	virtual void InitVertexShaders();
	virtual void InitPixelShaders();
	virtual void CreatePasses();
	virtual void SetToggles(NiGeometry* apGeometry, BSFogProperty* apFogProp, NiMaterialProperty* apMatProp);

	NiDX9ShaderDeclarationPtr	spShaderDeclarationGlowParallaxSkin;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationSkinned;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationModelSpaceNormals;
	NiDX9ShaderDeclarationPtr	spShaderDeclarationLandscape;
	NiD3DShaderConstantMapPtr	spTexEffectPixelConstantMap;
	NiD3DShaderConstantMapPtr	spTexEffectVertexConstantMap;
	NiD3DShaderConstantMapPtr	spPixelConstantMap2;
	NiD3DShaderConstantMapPtr	spVertexConstantMap2;


	__forceinline void SetTexEffectPixelConstantMap(NiD3DShaderConstantMap* apMap) { spTexEffectPixelConstantMap = apMap; }
	__forceinline NiD3DShaderConstantMap* GetTexEffectPixelConstantMap() { return spTexEffectPixelConstantMap; }

	__forceinline void SetTexEffectVertexConstantMap(NiD3DShaderConstantMap* apMap) { spTexEffectVertexConstantMap = apMap; }
	__forceinline NiD3DShaderConstantMap* GetTexEffectVertexConstantMap() { return spTexEffectVertexConstantMap; }

	static void SetCustomConstant(UInt32 index, float x, float y, float z, float w);
	static NiPoint4* GetCustomConstant(UInt32 index);

	static __forceinline void SetCameraWorldTranslate(float afX, float afY, float afZ, float afW) {
		NiPoint4* pPos = (NiPoint4*)0x11FA2A0;
		pPos->x = afX;
		pPos->y = afY;
		pPos->z = afZ;
		pPos->w = afW;
	};

	static bool* const bLODNoiseEnabled;
	static bool* const bLODNoiseAniso;
	static float* const fLODNoiseMipBias;
	static UInt32* const Unk_11FCC80;
	static UInt32* const Unk_11FC0A0;
	static UInt32* const Unk_11FB4C0;


	void SetClamp(NiD3DTextureStage* apTextureStage, BSShaderPPLightingProperty* apShaderProp);
	void SetDiffuseMap(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, UInt32 auiTextureNumber);
	void SetNormalMap(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, UInt32 auiTextureNumber);
	void SetGlowMap(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, UInt32 auiTextureNumber, bool abFallbackTex = true);
	void SetParallaxMap(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, UInt32 auiTextureNumber, bool abFallbackTex = true);
	void SetEnvMask(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, UInt32 auiTextureNumber);
	void SetCubeMap(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, UInt32 auiTextureNumber);
	void SetEffectMap(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber);
	void SetLandscapeTexture(BSShaderPPLightingProperty* apShaderProp, UInt32 auiStageNumber, bool abSkipNormals);
	void SetShadowMap(UInt32 auiStageNumber);
	void SetCanopyShadowTexture(UInt32 auiStageNumber);
	void SetCanopyShadowMask(UInt32 auiStageNumber);
	void SetLODNoiseMap(UInt32 auiStageNumber);

	void SetLandToggles(BSShaderPPLightingProperty* apShaderProp, UInt32 auiSpecID);
	void UpdateSkinWorldTranspose(NiSkinInstance* apSkinInstance);
	void UpdateTime(BSShaderPPLightingProperty* apShaderProp);
	void UpdateBoundCenter(NiGeometry* apGeometry);
	void UpdateHighDetailRange(NiGeometry* apGeometry);
	void UpdateRefractionStrength(BSShaderPPLightingProperty* apShaderProp);
	void UpdateLODLandParams(BSShaderManager::RenderPassType aeRenderPassType, BSShaderPPLightingProperty* apShaderProp, NiGeometry* apGeometry);
	void UpdateLODTextureParams(BSShaderPPLightingProperty* apShaderProp);
	void Unk_B79410(BSShaderManager::RenderPassType aeRenderPassType, BSShaderProperty::RenderPass* apRenderPass);
	void UpdateWaterFogConstants();
	void UpdateLights(BSShaderPPLightingProperty* apShaderProp, BSShaderProperty::RenderPass* apRenderPass, D3DXMATRIX aMatrix, NiTransform& akTransform, BSShaderManager::RenderPassType aeRenderPassType, NiSkinInstance* apSkinInstance);
	void UpdateAmbientColor(BSShaderPPLightingProperty* apShaderProp, BSShaderProperty::RenderPass* apRenderPass, NiGeometry* apGeometry, NiMaterialProperty* apMatProp);
	void UpdateWorldTranspose(BSShaderProperty::RenderPass* apRenderPass, BSShaderManager::RenderPassType aeRenderPassType, NiSkinInstance* apSkinInstance, NiTransform& akTransform) const;
	void UpdateShadowProj(NiPropertyState* apProperties, NiSkinInstance* apSkinInstance, NiTransform& akTransform);
	void UpdateEyePosition(BSShaderManager::RenderPassType aeRenderPassType, NiTransform& akTransform, D3DXMATRIX aMatrix);
	void UpdateToggles(BSShaderManager::RenderPassType aeRenderPassType, NiGeometry* apGeo, BSShaderPPLightingProperty* apShaderProp, NiMaterialProperty* apMatProp, BSShaderProperty::RenderPass* apRenderPass, NiAlphaProperty* apAlphaProp);
	void SetEmittanceColorForGlowMapMeshes(NiMaterialProperty* apMatProp);
	void SetAmbientColorForNonGlowMapMeshes(NiMaterialProperty* apMatProp);

	static void SetLightData(UInt32 auiLight, ShadowSceneLight* apSceneLight, float afDimmer);

	static bool __fastcall InitializeEx(ShadowLightShader* apThis);
	static void __fastcall RecreateRendererDataEx(ShadowLightShader* apThis);
	static void __fastcall SetRenderPassEx(ShadowLightShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall Func_62Ex(ShadowLightShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall Func_63Ex(ShadowLightShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass);
	static void __fastcall UpdateConstantsEx(ShadowLightShader* apThis, void*, NiPropertyState* apProperties);
	static void __fastcall SetupTexturesEx(ShadowLightShader* apThis, void*, NiPropertyState* apProperties);
	static NiDX9ShaderDeclaration* __fastcall GetShaderDeclarationEx(ShadowLightShader* apThis, void*, NiGeometry* apGeometry, BSShaderProperty* apShaderProp);
	static void __fastcall InitShaderConstantsEx(ShadowLightShader* apThis);
	static void __fastcall ReinitializeEx(ShadowLightShader* apThis);
	static void __fastcall ClearAllTextureStagesEx(ShadowLightShader* apThis);
	static void __fastcall SetupShadowTextureEx(ShadowLightShader* apThis, void*, BSShaderManager::RenderPassType aeRenderPass, BSShaderPPLightingProperty* apShaderProp);
	static void __fastcall InitShadersEx(ShadowLightShader* apThis);
	static void __fastcall InitVertexShadersEx(ShadowLightShader* apThis);
	static void __fastcall InitPixelShadersEx(ShadowLightShader* apThis);
	static bool __fastcall CreatePassesEx(ShadowLightShader* apThis);
	static void __fastcall SetTogglesEx(ShadowLightShader* apThis, void*, NiGeometry* apGeometry, BSFogProperty* apFogProp, NiMaterialProperty* apMatProp);

	void RenderTexEffect(NiGeometry* apGeometry, BSShaderPPLightingProperty* apShaderProp, BSShaderProperty::RenderPass* apRenderPass);
	void RenderZOnlyTexEffect(NiGeometry* apGeometry, BSShaderPPLightingProperty* apShaderProp, BSShaderProperty::RenderPass* apRenderPass);
	void UpdateTexEffectConstants(NiGeometry* apGeometry, BSShaderPPLightingProperty* apShaderProp);

	static NiD3DVertexShader* GetVertexShader(UInt32 auID);
	static void SetVertexShader(UInt32 auID, NiD3DVertexShader* apShader);

	static NiD3DPixelShader* GetPixelShader(UInt32 auID);
	static void SetPixelShader(UInt32 auID, NiD3DPixelShader* apShader);

	static ShadowLightShader*	GetShader();
	static NiD3DPassPtr*		GetAllPasses();
	static NiD3DPass*			PickPass(BSShaderManager::RenderPassType aeRenderPass);

	static float GetCanopyShadowScale();

	static NiPoint4* const LightPosition;
	static NiPoint4* const LightRadius;

	static std::vector<NiNode*> vSpotLightNodes;

	// For 11FCC80?
	enum PassDescriptors {
		kPD_UNK_0 = 0,
		kPD_UNK_1 = 1 << 0,
		kPD_UNK_2 = 1 << 1,
		kPD_UNK_3 = 1 << 2,
		kPD_UNK_4 = 1 << 3,
		kPD_UNK_5 = 1 << 4,
		kPD_UNK_6 = 1 << 5,
		kPD_UNK_7 = 1 << 6,
		kPD_UNK_8 = 1 << 7,
		kPD_UNK_9 = 1 << 8,
		kPD_UNK_10 = 1 << 9,
		kPD_UNK_11 = 1 << 10,
		kPD_SKINNED = 1 << 11,
		kPD_UNK_13 = 1 << 12,
		kPD_UNK_14 = 1 << 13,
		kPD_UNK_15 = 1 << 14,
		kPD_UNK_16 = 1 << 15,
	};

	enum VertexEntries {
		kVX_WorldViewProjTranspose = 0,
		kVX_WorldTranspose = 1,
		kVX_SkinWorldViewProjTranspose = 2,
		kVX_ShadowProj = 3,
		kVX_ShadowProjData = 4,
		kVX_ShadowProjTransform = 5,
		kVX_LightData = 6,
		kVX_FogParam_ShadowVolumeFatness = 7,
		kVX_FogColor_ShadowVolumeExtrudeDistance = 8,
		kVX_EyePosition = 9,
		kVX_BoneMatrix3 = 10,
		kVX_PrevBoneMatrix3 = 11,
		kVX_SkinWorldTranspose = 12,
		kVX_WorldViewTranspose = 13,
		kVX_SkinWorldViewTranspose = 14,
		kVX_Time = 15,
		kVX_HighDetailRange = 16,
		kVX_BoundCenter = 17,
		kVX_DepthData = 18,
		kVX_PrevWorldViewPT = 19,
		kVX_PrevWorldViewT = 20,
		kVX_LODLandParams = 21,
	};

	enum PixelEntries {
		kPX_EyeDir = 0,
		kPX_AmbientColor = 1,
		kPX_LightColors = 2,
		kPX_EmittanceColor = 3,
		kPX_Toggles = 4,
		kPX_EyePosition = 5,
		kPX_LightDirection = 6,
		kPX_LightPosition0 = 7,
		kPX_LightPosition1 = 8,
		kPX_LightPosition2 = 9,
		kPX_LightPosition3 = 10,
		kPX_HairTint = 15,
		kPX_RefractionParams = 16,
		kPX_FogParams = 17,
		kPX_FogColor = 18,
		kPX_FogPlane = 19,
		kPX_LODTEXPARAMS = 20,
		kPX_STBBColorConstants = 21,
	};

	union Toggles {
		struct {
			float bUseVertexColors;
			float fNormalStrength;
			float fEnvMapScale;
			float fHasMask;
		};

		struct {
			float bUseVertexColors;
			float bUseFog;
			float fSpecularity;
			float fAlphaTestRef;
		};
	};

	static __forceinline NiPoint4* GetConstant(int index) {
		return &((NiPoint4*)0x11FA0C0)[index];
	}

	struct VertexConstants {
		// WorldViewProjTranspose 0
		// SkinWorldViewProjTranspose 1
		// WorldViewTranspose 4
		// SkinWorldViewTranspose 5

		// 8
		static D3DXMATRIX* const WorldTranspose;


		// 9
		static D3DXMATRIX* const SkinWorldTranspose;

		// 9, 10, map 2
		static NiPoint2* const UVOffset;
		// 11, map 2
		static __forceinline float* const GetZScaler() {
			return (float*)0x11FD864;
		}

		// 12
		static __forceinline NiPoint4* const GetHighDetailRange() {
			return GetConstant(34);
		}

		// 13
		static float* const Time;

		// 14 - params, 15 - color
		static __forceinline BSShader::FogProperties* const GetFogParam() {
			return (BSShader::FogProperties*)0x11FA280;
		}

		// 16
		static NiPoint4* const EyePosition;

		// 17
		static __forceinline NiPoint4* const GetDepthData() {
			return (NiPoint4*)0x11FD954;
		}

		// 18
		static D3DXMATRIX* const ShadowProj;

		// 19
		static NiPoint4* const LODLandParams;

		// 22
		static __forceinline NiPoint4* const GetShadowProjData() {
			return (NiPoint4*)0x11FD874;
		}

		// 23
		static __forceinline NiPoint4* const GetShadowProjTransform() {
			return (NiPoint4*)0x11FD884;
		}

		// 24
		static __forceinline NiPoint4* const GetBoundCenter() {
			return (NiPoint4*)0x11FD8B4;
		}

		// 25; aka LightDirection
		static NiPoint4* const LightData;

		// 35
		static __forceinline D3DMATRIX* const GetPrevWorldViewPT() {
			return (D3DMATRIX*)0x11FED08;
		}

		// 39
		static __forceinline D3DMATRIX* const GetPrevWorldViewT() {
			return (D3DMATRIX*)0x11FED48;
		}

		// 98
		static float* const GetPrevBoneMatrix3() {
			return (float*)0x11FA580;
		}
	};

	struct PixelConstants {
		// 0
		static NiPoint4* const EyeDir;

		// 0, map 2
		static NiColorA* const FillColor;

		// 1
		static NiColorA* const AmbientColor;

		// 1, map 2
		static NiColorA* const RimColor;

		// 2
		static __forceinline NiColorA* const GetEmittanceColor() {
			return reinterpret_cast<NiColorA*>(GetConstant(27));
		}

		// 2, map 2
		static NiPoint4* const TexEffectVars;

		// 3
		static NiColorA* const LightColor;

		// 14 - params, 15 - color
		static FogProperties* const FogParams;

		// 16
		static NiPoint4* const FogPlane;

		// 17
		static NiPoint4* const EyePosition;

		// 18
		static __forceinline NiPoint4* const GetLightPosition() {
			return (NiPoint4*)0x11FD9A8;
		}
		static __forceinline NiPoint4* const GetLightPosition(UInt32 index) {
			ASSERT(index >= 0 && index < 10);
			return &((NiPoint4*)0x11FD9A8)[index];
		}

		// 27
		static __forceinline Toggles* const GetToggles() {
			return reinterpret_cast<Toggles*>(GetConstant(33));
		}

		// 28
		static __forceinline NiColorA* const GetHairTint() {
			return (NiColorA*)0x11FA090;
		}

		// 29
		static __forceinline NiPoint4* const GetRefractionParams() {
			return (NiPoint4*)0x11FD944;
		}

		// 30
		static __forceinline NiPoint4* const GetSTBBColorConstants() {
			return GetConstant(35);
		}

		// 31
		static __forceinline BSShaderPPLightingProperty::LODTexParams* const GetLODTEXPARAMS() {
			return (BSShaderPPLightingProperty::LODTexParams*)0x11FA300;
		}
	};

	__forceinline void SetPixelConstantMapEntry(ShadowLightShader::PixelEntries aeIndex, const char* apEntryName) {
		((NiShaderConstantMapEntryPtr*)0x11FEC30)[aeIndex] = GetPixelConstantMap()->GetEntry(apEntryName);
	};

	static __forceinline void SetPixelConstantMapEntry(ShadowLightShader::PixelEntries aeIndex, NiShaderConstantMapEntry* apEntry) {
		((NiShaderConstantMapEntryPtr*)0x11FEC30)[aeIndex] = apEntry;
	};

	static __forceinline NiShaderConstantMapEntry* GetPixelConstantMapEntry(ShadowLightShader::PixelEntries index) {
		return ((NiShaderConstantMapEntryPtr*)0x11FEC30)[index];
	}

	__forceinline void SetVertexConstantMapEntry(ShadowLightShader::VertexEntries aeIndex, const char* apEntryName) {
		((NiShaderConstantMapEntryPtr*)0x11FEBD8)[aeIndex] = GetVertexConstantMap()->GetEntry(apEntryName);
	};

	static __forceinline void SetVertexConstantMapEntry(ShadowLightShader::VertexEntries aeIndex, NiShaderConstantMapEntry* apEntry) {
		((NiShaderConstantMapEntryPtr*)0x11FEBD8)[aeIndex] = apEntry;
	};

	static __forceinline NiShaderConstantMapEntry* GetVertexConstantMapEntry(ShadowLightShader::VertexEntries index) {
		return ((NiShaderConstantMapEntryPtr*)0x11FEBD8)[index];
	}
};
ASSERT_SIZE(ShadowLightShader, 0x8C)