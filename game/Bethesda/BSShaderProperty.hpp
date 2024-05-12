#pragma once

#include "BSShaderManager.hpp"
#include "NiShadeProperty.hpp"
#include "stack"

class BSShaderAccumulator;
class NiSourceTexture;
class Lighting30ShaderProperty;
class ShadowSceneLight;
class BGSTextureUseMap;

NiSmartPointer(BSShaderProperty);

enum Flags {
	BSSP_SPECULAR						= 0,
	BSSP_SKINNED						= 1,
	BSSP_LOW_DETAIL						= 2,
	BSSP_VERTEX_ALPHA					= 3,
	BSSP_MOTION_BLUR					= 4,
	BSSP_SINGLE_PASS					= 5,
	BSSP_EMPTY							= 6,
	BSSP_ENVIRONMENT_MAPPING			= 7,
	BSSP_ALPHA_TEXTURE					= 8,
	BSSP_ZPREPASS						= 9,
	BSSP_FACEGEN						= 10,
	BSSP_PARALLAX						= 11,
	BSSP_MODEL_SPACE_NORMALS			= 12,
	BSSP_NON_PROJECTIVE_SHADOWS			= 13,
	BSSP_LANDSCAPE						= 14,
	BSSP_REFRACTION						= 15,
	BSSP_FIRE_REFRACTION				= 16,
	BSSP_EYE_ENVIRONMENT_MAPPING		= 17,
	BSSP_HAIR							= 18,
	BSSP_DYNAMIC_ALPHA					= 19,
	BSSP_LOCAL_MAP_HIDE_SECRET			= 20,
	BSSP_WINDOW_ENVIRONMENT_MAPPING		= 21,
	BSSP_TREE_BILLBOARD					= 22,
	BSSP_SHADOW_FRUSTUM					= 23,
	BSSP_MULTIPLE_TEXTURES				= 24,
	BSSP_REMAPPABLE_TEXTURES			= 25,
	BSSP_DECAL							= 26,
	BSSP_DYNAMIC_DECAL					= 27,
	BSSP_PARALLAX_OCCLUSION				= 28,
	BSSP_EXTERNAL_EMITTANCE				= 29,
	BSSP_SHADOW_MAP						= 30,
	BSSP_ZBUFFER_TEST					= 31,
	BSSP_ZBUFFER_WRITE					= 32,
	BSSP_LOD_LANDSCAPE					= 33,
	BSSP_LOD_BUILDING					= 34,
	BSSP_NO_FADE						= 35,
	BSSP_REFRACTION_TINT				= 36,
	BSSP_VERTEX_COLORS					= 37,
	BSSP_1ST_PERSON						= 38,
	BSSP_1ST_LIGHT_IS_POINTLIGHT		= 39,
	BSSP_2ND_LIGHT						= 40,
	BSSP_3RD_LIGHT						= 41,
	BSSP_VERTEX_LIGHTING				= 42,
	BSSP_UNIFORM_SCALE					= 43,
	BSSP_FIT_SLOPE						= 44,
	BSSP_BILLBOARD_ENV_FADE				= 45,
	BSSP_NO_LOD_LAND_BLEND				= 46,
	BSSP_ENVMAP_LIGHT_FADE				= 47,
	BSSP_WIRE_FRAME						= 48,
	BSSP_VATS_SELECTION					= 49,
	BSSP_SHOW_IN_LOCAL_MAP				= 50,
	BSSP_PREMULT_ALPHA					= 51,
	BSSP_SKIP_NORMAL_MAPS				= 52,
	BSSP_ALPHA_DECAL					= 53,
	BSSP_NO_TRANSPARENCY_MULTISAMPLING	= 54,
	BSSP_STINGER_PROP					= 55,
	BSSP_UNK3							= 56,
	BSSP_UNK4							= 57,
	BSSP_UNK5							= 58,
	BSSP_UNK6							= 59,
	BSSP_UNK7							= 60,
	BSSP_UNK8							= 61,
	BSSP_UNK9							= 62,
	BSSP_REALTIME_CUBEMAP				= 63,
	BSSP_MAX_FLAGS						= 64
};


class BSShaderProperty : public NiShadeProperty {
public:
	BSShaderProperty();
	virtual ~BSShaderProperty();

	enum BSShaderFlags {
		Specular					= 0x1,
		Skinned						= 0x2,
		LowDetail					= 0x4,
		Vertex_Alpha				= 0x8,
		Motion_Blur					= 0x10,
		Single_Pass					= 0x20,
		Empty						= 0x40,
		Environment_Mapping			= 0x80,
		Alpha_Texture				= 0x100,
		Z_Prepass					= 0x200,
		FaceGen						= 0x400,
		Parallax_Shader				= 0x800,
		Model_Space_Normals			= 0x1000,
		Non_Projective_Shadows		= 0x2000,
		Landscape					= 0x4000,
		Refraction					= 0x8000,
		Fire_Refraction				= 0x10000,
		Eye_Environment_Mapping		= 0x20000,
		Hair						= 0x40000,
		Dynamic_Alpha				= 0x80000,
		Localmap_Hide_Secret		= 0x100000,
		Window_Environment_Mapping	= 0x200000,
		Tree_Billboard				= 0x400000,
		Shadow_Frustum				= 0x800000,
		Multiple_Textures			= 0x1000000,
		Remappable_Textures			= 0x2000000,
		Decal_Single_Pass			= 0x4000000,
		Dynamic_Decal_Single_Pass	= 0x8000000,
		Parallax_Occlusion			= 0x10000000,
		External_Emittance			= 0x20000000,
		Shadow_Map					= 0x40000000,
		ZBuffer_Test				= 0x80000000,
	};

	enum BSShaderFlags2 {
		ZBuffer_Write					= 0x1,
		LOD_Landscape					= 0x2,
		LOD_Building					= 0x4,
		No_Fade							= 0x8,
		Refraction_Tint					= 0x10,
		Vertex_Colors					= 0x20,
		_1st_person						= 0x40,
		_1st_Light_is_Point_Light		= 0x80,
		_2nd_Light						= 0x100,
		_3rd_Light						= 0x200,
		Vertex_Lighting					= 0x400,
		Uniform_Scale					= 0x800,
		Fit_Slope						= 0x1000,
		Billboard_and_Envmap_Light_Fade = 0x2000,
		No_LOD_Land_Blend				= 0x4000,
		Envmap_Light_Fade				= 0x8000,
		Wireframe						= 0x10000,
		VATS_Selection					= 0x20000,
		Show_in_Local_Map				= 0x40000,
		Premult_Alpha					= 0x80000,
		Skip_Normal_Maps				= 0x100000,
		Alpha_Decal						= 0x200000,
		No_Transparency_Multisampling	= 0x400000,
		stinger_prop					= 0x800000,
		Unknown3						= 0x1000000,
		Unknown4						= 0x2000000,
		Unknown5						= 0x4000000,
		Unknown6						= 0x8000000,
		Unknown7						= 0x10000000,
		Unknown8						= 0x20000000,
		Unknown9						= 0x40000000,
		Wall_RealTimeEnv				= 0x80000000,
	};

	class RenderPass : public BSMemObject {
	public:
		enum AccumMode : UInt8 {
			ACCUM_NONE				= 0,
			ACCUM_ALPHA_BLEND		= 1,
			ACCUM_UNK_2				= 2,
			ACCUM_PARTICLE			= 3,
			ACCUM_DECAL_SINGLE		= 4,
			ACCUM_DYN_DECAL_SINGLE	= 5,
			ACCUM_REFRACTION		= 6,
			ACCUM_REFRACTION_CLEAR	= 7,
			ACCUM_UNK_8				= 8,
			ACCUM_UNK_9				= 9,
			ACCUM_LOD				= 10,
			ACCUM_UNK_11			= 11,
			ACCUM_UNK_12			= 12,
			ACCUM_UNK_13			= 13,
			ACCUM_ALPHA_FADE		= 14,
		};


		RenderPass(NiGeometry* apGeometry, BSShaderManager::RenderPassType aePassEnum, bool abEnable, UInt8 aucNumLights, ...);
		~RenderPass();

		NiGeometry*			pGeometry;
		UInt16				usPassEnum;
		AccumMode			eAccumulationHint;
		bool				bEnabled;
		bool				bNoFog;
		UInt8				ucNumLights;
		UInt8				ucMaxNumLights;
		UInt8				cCurrLandTexture;
		ShadowSceneLight**	ppSceneLights;

		void SetLights(UInt8 aucNumLights, ...);
#if USE_PBR
		void SetLights(std::stack<ShadowSceneLight*>* apLights);
#endif
		static const char* GetPassName(UInt32 uiRenderPass);
		static LPCWSTR GetPassNameWideStr(UInt32 uiRenderPass);
		static const char* GetCurrentPassName();
		static const char* GetCurrentPassShaderType();
		static bool IsShadowPass(UInt32 uiRenderPass);

		const char* GetPassName();
		NiGeometry* GetGeometry();

		__forceinline BSShaderManager::RenderPassType GetPassType() const { return (BSShaderManager::RenderPassType)usPassEnum; }
		__forceinline static UInt32 GetCurrentPassType() { return *(UInt32*)0x11F91E4; }
		__forceinline static RenderPass* GetCurrentPass() { return *(RenderPass**)0x11F91E0; }

		void ClearSceneLights();
		void AllocateSceneLights(UInt8 aucNumLights);

		static void RenderImmediately(NiGeometry* apGeometry, BSShaderManager::RenderPassType aePassEnum);
	};

	class RenderPassArray : public NiTObjectArray<RenderPass*> {
	public:
		RenderPassArray(UInt32 auiSize);
		virtual ~RenderPassArray() override;

		UInt32 uiMaxPasses;

		void AddPass(NiGeometry* apGeometry, BSShaderManager::RenderPassType auiPassEnum, bool abEnable,
			UInt8 aucNumLights = 0, ShadowSceneLight* apSceneLight = nullptr, ShadowSceneLight* apSceneLight1 = nullptr, ShadowSceneLight* apSceneLight2 = nullptr, RenderPass* apRenderPass = nullptr);

		bool IsEmpty() const { return uiMaxPasses == 0; }
		RenderPass* GetLastPass() const { return GetAt(uiMaxPasses - 1); }

		static void __fastcall AddPassEx(RenderPassArray* apThis, void*, NiGeometry* apGeometry, BSShaderManager::RenderPassType auiPassEnum, bool abEnable,
			UInt8 aucNumLights, ShadowSceneLight* apSceneLight, ShadowSceneLight* apSceneLight1, ShadowSceneLight* apSceneLight2, RenderPass* apRenderPass);
	};

	virtual void						CopyTo(BSShaderProperty* apTarget);
	virtual void						CopyData(BSShaderProperty* apTarget);
	virtual void						SetupGeometry(NiGeometry* apGeometry);
	virtual RenderPassArray*			GetRenderPasses(const NiGeometry* apGeometry, const UInt32 auiEnabledPasses, UInt16* apusPassCount, const UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
	virtual UInt16						GetNumberofPasses(NiGeometry* apGeometry);
	virtual RenderPassArray*			GetRenderPassArray4C();
	virtual RenderPass*					SetDepthPass(NiGeometry* apGeometry);
	virtual BSShaderProperty*			PickShader(NiGeometry* apGeometry, UInt32 unk0 = 0, UInt32 unk2 = 0);
	virtual NiSourceTexture*			GetDiffuseTexture() const;
	virtual RenderPassArray*			GetWaterDepthPass(NiGeometry* apGeometry);
	virtual void						CountTextures(void* apCountFunc, BGSTextureUseMap* apTexMap) const;
	virtual void						PrecacheTextures() const;

	UInt32							ulFlags[2];
	float							fAlpha;
	float							fFadeAlpha;
	float							fEnvMapScale;
	float							fLODFade;
	UInt32							iLastRenderPassState;
	RenderPassArray*				pRenderPassArray;				// Mistakenly called pRenderPassList, Oblivion leftover
	RenderPassArray*				pRenderPassArray_depthMap;
	RenderPassArray*				pRenderPassArray_constAlpha;
	RenderPassArray*				pRenderPassArray_localMap;
	RenderPassArray*				pRenderPassArray_unk4C;
	RenderPassArray*				pRenderPassArray_waterDepth;
	RenderPassArray*				pRenderPassArray_silhouette;
	BSShaderManager::ShaderType		uiShaderIndex;
	float							fDepthBias;

	CREATE_OBJECT(BSShaderProperty, 0xBA9250);

	bool GetFlag(UInt32 auiFlag) const;
	void SetFlag(UInt32 auiFlag, bool abEnable);

	bool HasSpecular() const { return (ulFlags[0] & Specular) != 0; }
	bool IsSkinned() const { return (ulFlags[0] & Skinned) != 0; }
	bool IsLowDetail() const { return (ulFlags[0] & LowDetail) != 0; }
	bool HasVertexAlpha() const { return (ulFlags[0] & Vertex_Alpha) != 0; }
	bool HasMotionBlur() const { return (ulFlags[0] & Motion_Blur) != 0; }
	bool IsSinglePass() const { return (ulFlags[0] & Single_Pass) != 0; }
	bool IsEmpty() const { return (ulFlags[0] & Empty) != 0; }
	bool HasEnvironmentMap() const { return (ulFlags[0] & Environment_Mapping) != 0; }
	bool HasAlphaTexture() const { return (ulFlags[0] & Alpha_Texture) != 0; }
	bool IsZPrePass() const { return (ulFlags[0] & Z_Prepass) != 0; }
	bool IsFaceGen() const { return (ulFlags[0] & FaceGen) != 0; }
	bool IsParallax() const { return (ulFlags[0] & Parallax_Shader) != 0; }
	bool HasModelSpaceNormals() const { return (ulFlags[0] & Model_Space_Normals) != 0; }
	bool HasNonProjectiveShadows() const { return (ulFlags[0] & Non_Projective_Shadows) != 0; }
	bool IsLandscape() const { return (ulFlags[0] & Landscape) != 0; }
	bool HasRefraction() const { return (ulFlags[0] & Refraction) != 0; }
	bool HasFireRefraction() const { return (ulFlags[0] & Fire_Refraction) != 0; }
	bool HasEyeEnvironmentMap() const { return (ulFlags[0] & Eye_Environment_Mapping) != 0; }
	bool IsHair() const { return (ulFlags[0] & Hair) != 0; }
	bool HasDynamicAlpha() const { return (ulFlags[0] & Dynamic_Alpha) != 0; }
	bool IsLocalMapHideSecret() const { return (ulFlags[0] & Localmap_Hide_Secret) != 0; }
	bool HasWindowEnvironmentMap() const { return (ulFlags[0] & Window_Environment_Mapping) != 0; }
	bool IsTreeBillboard() const { return (ulFlags[0] & Tree_Billboard) != 0; }
	bool UsesShadowFrustum() const { return (ulFlags[0] & Shadow_Frustum) != 0; }
	bool HasMultipleTextures() const { return (ulFlags[0] & Multiple_Textures) != 0; }
	bool HasRemappableTextures() const { return (ulFlags[0] & Remappable_Textures) != 0; }
	bool IsSinglePassDecal() const { return (ulFlags[0] & Decal_Single_Pass) != 0; }
	bool IsSinglePassDynamicDecal() const { return (ulFlags[0] & Dynamic_Decal_Single_Pass) != 0; }
	bool HasParallaxOcclusion() const { return (ulFlags[0] & Parallax_Occlusion) != 0; }
	bool HasExternalEmittance() const { return (ulFlags[0] & External_Emittance) != 0; }
	bool IsShadowMap() const { return (ulFlags[0] & Shadow_Map) != 0; }
	bool HasZBufferTest() const { return (ulFlags[0] & ZBuffer_Test) != 0; }
	bool HasZBufferWrite() const { return (ulFlags[1] & ZBuffer_Write) != 0; }
	bool IsLODLand() const { return (ulFlags[1] & LOD_Landscape) != 0; }
	bool IsLODBuilding() const { return (ulFlags[1] & LOD_Building) != 0; }
	bool HasNoFade() const { return (ulFlags[1] & No_Fade) != 0; }
	bool HasRefractionTint() const { return (ulFlags[1] & Refraction_Tint) != 0; }
	bool HasVertexColors() const { return (ulFlags[1] & Vertex_Colors) != 0; }
	bool Is1stPerson() const { return (ulFlags[1] & _1st_person) != 0; }
	bool Is1stLightPointlight() const { return (ulFlags[1] & _1st_Light_is_Point_Light) != 0; }
	bool Is2ndLight() const { return (ulFlags[1] & _2nd_Light) != 0; }
	bool Is3rdLight() const { return (ulFlags[1] & _3rd_Light) != 0; }
	bool HasVertexLighting() const { return (ulFlags[1] & Vertex_Lighting) != 0; }
	bool HasUniformScale() const { return (ulFlags[1] & Uniform_Scale) != 0; }
	bool DoesFitSlopes() const { return (ulFlags[1] & Fit_Slope) != 0; }
	bool HasBillboardAndEnvmapLightFade() const { return (ulFlags[1] & Billboard_and_Envmap_Light_Fade) != 0; }
	bool HasNoLODLandBlend() const { return (ulFlags[1] & No_LOD_Land_Blend) != 0; }
	bool HasEnvmapLightFade() const { return (ulFlags[1] & Envmap_Light_Fade) != 0; }
	bool IsWireframe() const { return (ulFlags[1] & Wireframe) != 0; }
	bool IsVATSSelection() const { return (ulFlags[1] & VATS_Selection) != 0; }
	bool IsShownOnMap() const { return (ulFlags[1] & Show_in_Local_Map) != 0; }
	bool HasPremultAlpha() const { return (ulFlags[1] & Premult_Alpha) != 0; }
	bool DoesSkipNormalMaps() const { return (ulFlags[1] & Skip_Normal_Maps) != 0; }
	bool IsAlphaDecal() const { return (ulFlags[1] & Alpha_Decal) != 0; }
	bool HasNoTMSAA() const { return (ulFlags[1] & No_Transparency_Multisampling) != 0; }
	bool IsStingerProperty() const { return (ulFlags[1] & stinger_prop) != 0; }
	bool HasRealtimeCubeMaps() const { return (ulFlags[1] & Wall_RealTimeEnv) != 0; }

	void SetAlpha(float afAlpha);

	static void __fastcall GetViewerStringsEx(BSShaderProperty* apThis, void*, NiViewerStringsArray* apStrings);

	void InitRenderPassArray(SInt32 aiSize);
	void InitRenderPassArray_DepthMap(UInt32 auiSize = 1);
	void InitRenderPassArray_ConstAlpha(UInt32 auiSize = 1);
	void InitRenderPassArray_LocalMap(UInt32 auiSize = 1);
	void InitRenderPassArray_WaterDepth(UInt32 auiSize = 1);
	void InitRenderPassArray_Silhouette(UInt32 auiSize = 1);

	static float* fEnvMapLOD2;
	static float* fEyeEnvMapLOD2;
	static float* fSpecularLODEndFade;

#define ADD_PASS_ARGS NiGeometry* apGeometry, UInt16* apusPassCount, bool abAddPass, bool& abEnablePass
#define ONE_LIGHT ShadowSceneLight* apLight0
#define TWO_LIGHTS ShadowSceneLight* apLight0, ShadowSceneLight* apLight1
#define THREE_LIGHTS ShadowSceneLight* apLight0, ShadowSceneLight* apLight1, ShadowSceneLight* apLight2

	void AddAlphaPass(NiGeometry* apGeometry, bool abSkinned, bool abAlphaTest);
	void AddLocalMapPass(NiGeometry* apGeometry, bool abSkinned, bool abHidden);
	void AddSilhouettePass(NiGeometry* apGeometry, bool abSkinned);
	void AddDepthPass(NiGeometry* apGeometry, bool abSkinned, bool abAlphaTest, BSShaderAccumulator* apAccumulator);
	void AddPass_ZOnly(ADD_PASS_ARGS, bool abSkinned, bool abUseAlphaTest);
	void AddPass_ZOnly_TexEffect(ADD_PASS_ARGS, bool abSkinned);
	void AddPass_Normals(ADD_PASS_ARGS, bool abSkinned, bool abFire);
	void AddPass_NormalsClear(ADD_PASS_ARGS, bool abSkinned, bool abFire);
	
	void AddPass_LandLow(ADD_PASS_ARGS, ONE_LIGHT, bool abSpecular, bool abShadows);

	void AddPass_Ambient(ADD_PASS_ARGS, ONE_LIGHT, bool abSkinned, bool abGlowMap, bool abTreeBranch, bool abAlphaTest);

	void AddPass_Textures(ADD_PASS_ARGS, ONE_LIGHT, bool abSkinned, bool abFaceGen, bool abVertexColors, bool abParallax, bool abTreeBranch, bool abHair);

	void AddPass_ADT(ADD_PASS_ARGS, ONE_LIGHT,	 bool abSkinned, bool abGlowMap, bool abFaceGen, bool abShadows, bool abParallax, bool abModelSpaceNormals, bool abTreeBranch, bool abTreeBillboard, bool abHair);
	void AddPass_ADT2(ADD_PASS_ARGS, TWO_LIGHTS, bool abSkinned, bool abGlowMap, bool abFaceGen, bool abShadows, bool abParallax, bool abTreeBranch, bool abHair);
	
	void AddPass_ADTS(ADD_PASS_ARGS, ONE_LIGHT,	 bool abSkinned, bool abGlowMap, bool abFaceGen, bool abShadows, bool abParallax, bool abModelSpaceNormals, bool abTreeBranch, bool abTreeBillboard, bool abHair);
	void AddPass_ADTS2(ADD_PASS_ARGS, TWO_LIGHTS, bool abSkinned, bool abGlowMap, bool abFaceGen, bool abShadows, bool abParallax, bool abTreeBranch, bool abHair);

	void AddPass_AD2(ADD_PASS_ARGS, TWO_LIGHTS, bool abSkinned, bool abGlowMap, bool abFaceGen, bool abShadows, bool abParallax, bool abTreeBranch, bool abHair);
	void AddPass_AD3(ADD_PASS_ARGS, THREE_LIGHTS, bool abSkinned, bool abGlowMap, bool abFaceGen, bool abShadows, bool abParallax, bool abTreeBranch, bool abHair);

	void AddPass_Opt(ADD_PASS_ARGS, ONE_LIGHT, bool abSkinned, bool abGlowMap, bool abFaceGen, bool abSpecular, bool unk);

	void AddPass_DiffuseDirOrPoint(ADD_PASS_ARGS, ONE_LIGHT, bool abSkinned, bool abTreeBranch, bool abPoint = false);
	void AddPass_DiffusePoint2(ADD_PASS_ARGS, TWO_LIGHTS, bool abSkinned, bool abFaceGen, bool abParallax, bool abTreeBranch, bool abHair);
	void AddPass_DiffusePoint3(ADD_PASS_ARGS, THREE_LIGHTS, bool abSkinned, bool abFaceGen, bool abParallax, bool abTreeBranch, bool abHair);


	void AddPass_SpecularPoint(ADD_PASS_ARGS, ONE_LIGHT, bool abSkinned, bool abParallax, bool abTreeBranch, bool abHair);
	void AddPass_SpecularPoint2(ADD_PASS_ARGS, TWO_LIGHTS, bool abSkinned, bool abParallax, bool abTreeBranch, bool abHair);
	void AddPass_SpecularPoint3(ADD_PASS_ARGS, THREE_LIGHTS, bool abSkinned, bool abParallax, bool abTreeBranch, bool abHair);

	void AddPass_2X_EnvMap(ADD_PASS_ARGS, TWO_LIGHTS, bool abSkinned, bool abEyeEnvMapping, bool abWindowEnvMapping);
	void AddPass2x_TexEffect(ADD_PASS_ARGS, bool abSkinned);
	void AddPass_Fog(ADD_PASS_ARGS, bool abSkinned, bool abTreeBranch, bool abUseAlphaBlend);
#if USE_PBR
	void AddPass_PBR(NiGeometry* apGeometry, UInt16* apusPassCount, bool abAddPass, bool& abEnablePass, bool abSkinned, bool abUseAlphaTest, std::stack<ShadowSceneLight*>* apLights);
#endif
};

ASSERT_SIZE(BSShaderProperty, 0x60);
ASSERT_SIZE(BSShaderProperty::RenderPass, 0x10);
ASSERT_SIZE(BSShaderProperty::RenderPassArray, 0x14);