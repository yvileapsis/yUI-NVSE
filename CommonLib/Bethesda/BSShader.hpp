#pragma once

#include "NiD3DDefaultShader.hpp"
#include "NiDX9ShaderDeclaration.hpp"
#include "BSShaderManager.hpp"
#include "BSShaderProperty.hpp"

class NiD3DPass;

class BSShader : public NiD3DDefaultShader {
public:
	enum ShaderType {
		TYPE_ShadowLightShader		= 1,
		TYPE_HairShader				= 2,
		TYPE_ParallaxShader			= 3,
		TYPE_SkinShader				= 4,
		TYPE_SpeedTreeBranchShader	= 5,
		TYPE_TallGrassShader		= 6,
		TYPE_DistantLODShader		= 7,
		TYPE_SpeedTreeFrondShader	= 8,
		TYPE_SpeedTreeLeafShader	= 9,
		TYPE_NiD3DDefaultShader		= 10,
		TYPE_SkyShader				= 11,
		TYPE_GeometryDecalShader	= 12,
		TYPE_WaterShader			= 13,
		TYPE_ParticleShader			= 14,
		TYPE_BoltShader				= 15,
		TYPE_BeamShader				= 16,
		TYPE_Lighting30Shader		= 17,
		TYPE_PrecipitationShader	= 18,
		TYPE_VolumetricFogShader	= 19,
		TYPE_TileShader				= 20,
		TYPE_BSShaderNoLighting		= 21,
		TYPE_BSShaderBloodSplatter	= 22,
		TYPE_ImagespaceShader		= 23,
		TYPE_BSDistantTreeShader	= 24,
	};

	virtual void SetRenderPass(UInt32 aeType);
	virtual void SetShaders(BSShaderManager::RenderPassType aeType = BSShaderManager::BSSM_EMPTY);
	// Seems to focus on tree shadows in most shaders
	virtual void Func_62(UInt32 aeType);
	// Seems to focus on the fog in most shaders
	virtual void Func_63(UInt32 aeType);
	virtual void RestoreSavedStates(UInt32 aeType);
	virtual void RestoreTechnique(UInt32 uiLastPass);
	// Checks for bUnk64
	virtual void ConfigureTextureStages();
	virtual void ConfigureAllTextureStages();
	virtual void StartMultiPass(NiPropertyState* pShaderProps);
	virtual void StopMultiPass();
	virtual NiDX9ShaderDeclaration* GetShaderDeclaration(NiGeometry*, BSShaderProperty* property);
	virtual void InitShaderConstants();
	// Reloads/ loads shader files and setups passes
	virtual void Reinitialize();
	virtual void ClearAllTextureStages();
	virtual void ClearTextureStages(NiD3DPass* apPass);
	virtual void Func_75();
	virtual void CreateNewPass();
	virtual void Func_77();
	virtual void Func_78(UInt32* apeType, int a2);

	NiD3DPassPtr	spPass;
	void*			pUnk60;
	bool			bUnk64;
	UInt32			iShaderType;

	bool IsShadowLightShader() const { return iShaderType == TYPE_ShadowLightShader; }
	bool IsHairShader() const { return iShaderType == TYPE_HairShader; }
	bool IsParallaxShader() const { return iShaderType == TYPE_ParallaxShader; }
	bool IsSkinShader() const { return iShaderType == TYPE_SkinShader; }
	bool IsSpeedTreeBranchShader() const { return iShaderType == TYPE_SpeedTreeBranchShader; }
	bool IsTallGrassShader() const { return iShaderType == TYPE_TallGrassShader; }
	bool IsDistantLODShader() const { return iShaderType == TYPE_DistantLODShader; }
	bool IsSpeedTreeFrondShader() const { return iShaderType == TYPE_SpeedTreeFrondShader; }
	bool IsSpeedTreeLeafShader() const { return iShaderType == TYPE_SpeedTreeLeafShader; }
	bool IsNiD3DDefaultShader() const { return iShaderType == TYPE_NiD3DDefaultShader; }
	bool IsSkyShader() const { return iShaderType == TYPE_SkyShader; }
	bool IsGeometryDecalShader() const { return iShaderType == TYPE_GeometryDecalShader; }
	bool IsWaterShader() const { return iShaderType == TYPE_WaterShader; }
	bool IsParticleShader() const { return iShaderType == TYPE_ParticleShader; }
	bool IsBoltShader() const { return iShaderType == TYPE_BoltShader; }
	bool IsBeamShader() const { return iShaderType == TYPE_BeamShader; }
	bool IsLighting30Shader() const { return iShaderType == TYPE_Lighting30Shader; }
	bool IsPrecipitationShader() const { return iShaderType == TYPE_PrecipitationShader; }
	bool IsVolumetricFogShader() const { return iShaderType == TYPE_VolumetricFogShader; }
	bool IsTileShader() const { return iShaderType == TYPE_TileShader; }
	bool IsBSShaderNoLighting() const { return iShaderType == TYPE_BSShaderNoLighting; }
	bool IsBSShaderBloodSplatter() const { return iShaderType == TYPE_BSShaderBloodSplatter; }
	bool IsImagespaceShader() const { return iShaderType == TYPE_ImagespaceShader; }
	bool IsBSDistantTreeShader() const { return iShaderType == TYPE_BSDistantTreeShader; }

	static bool __fastcall InitializeEx(BSShader* apThis);
	static void __fastcall SetShadersEx(BSShader* apThis, void*, BSShaderManager::RenderPassType aeType);

	static void CalculateTransformMatrix(NiTransform& akTransform, NiSkinInstance* apSkinInstance, D3DXMATRIX& pOut);
	void SetWorldViewProjTranspose(NiGeometry* apGeometry, const NiTransform& akTransform);

	NiD3DVertexShader* CreateVertexShader(const char* apFilename);
	NiD3DPixelShader* CreatePixelShader(const char* apFilename);

	static NiD3DVertexShader* __fastcall CreateVertexShaderEx(BSShader* apThis, void*, const char* apPath, D3DXMACRO* apMacro, const char* apShaderVersion, const char* apFilename);
	static NiD3DPixelShader* __fastcall CreatePixelShaderEx(BSShader* apThis, void*, const char* apPath, D3DXMACRO* apMacro, const char* apShaderVersion, const char* apFilename);

	static void SetLightData(UInt32 auiNumLights, ShadowSceneLight* apLight, float afDimmer);

	void Setup_ADT4_Opt(NiPropertyState& arProperties);

	struct FogProperties {
		struct FogParameters {
			float fDistFar;
			float fDistNear;
			float fPower;
			float fUnknown;
		};

		FogParameters Parameters;
		NiColorA Color;
	};

	static float fLightMult;
	static float fEmissiveMult;
	static float fNoLightingMult;
	static float fSunMult;
	static float fAmbientMult;
};

ASSERT_SIZE(BSShader, 0x6C);