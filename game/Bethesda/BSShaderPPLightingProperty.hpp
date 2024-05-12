#pragma once

#include "BSShaderLightingProperty.hpp"
#include "BSShaderTextureSet.hpp"
#include "TangentSpaceData.hpp"
#include "NiBinaryExtraData.hpp"

class NiSourceTexture;
class BSShaderAccumulator;
class Lighting30ShaderProperty;

NiSmartPointer(BSShaderPPLightingProperty);

class BSShaderPPLightingProperty : public BSShaderLightingProperty {
public:
	BSShaderPPLightingProperty();
	virtual ~BSShaderPPLightingProperty();

	virtual void							CopyTo3(Lighting30ShaderProperty* apTarget);
	virtual void							CopyProperties(BSShaderProperty* apTarget);
	virtual NiAdditionalGeometryData*		SetupLandscape(NiGeometry* apGeometry);
	virtual void							Unk_53();
	virtual void							PickClampMode(UInt32 auiMode);
	virtual NiTexturingProperty::ClampMode	GetClampMode();
	virtual void							SetClampMode(NiTexturingProperty::ClampMode aeClamp);
	virtual UInt16							GetLandscapeTextureCount();
	virtual int								GetLandPassCount();
	virtual void							SetTexture(BSTextureSet::TextureType aeTextureSlot, UInt32 auiTextureNumber, NiTexture* apTexture);
	virtual NiTexture*						GetTexture(BSTextureSet::TextureType aeTextureSlot, UInt32 auiTextureNumber);
	virtual void							LoadTextureSet(BSTextureSet* apTextureSet);
	virtual void							SetDiffuseTexture(int, NiTexture* apTexture);
	virtual void							SetNormalTexture(int, NiTexture* apTexture);
	virtual void							SetGlowMask();
	virtual void							SetSpecular();
	virtual float							GetSpecularFade();
	virtual float							GetEnvMapFade();
	virtual void							AddPassesOld(NiGeometry* apGeometry, UInt32 aeEnabledPasses, UInt16* apusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apShaderAccum, bool abAddPass);
	virtual void							AddPasses(NiGeometry* apGeometry, UInt32 aeEnabledPasses, UInt16* apusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apShaderAccum, bool abAddPass);

	struct SpecularExponents {
		UInt8 ucExponent[10];
	};

	struct SpecularAvailabilities {
		bool bHasSpecular[10];
	};

	NiSmartPointer(TextureEffectData);

	class TextureEffectData : public NiRefObject {
	public:
		TextureEffectData();
		~TextureEffectData();

		NiTexturePtr	spTexture08;
		NiColorA		kNewFillColor;
		NiColorA		kNewEdgeColor;
		NiColorA		kFillColor;
		NiColorA		kEdgeColor;
		float			fAnimU;
		float			fAnimV;
		float			fEdgeFalloff;
		float			fUnk58;
		D3DBLEND		eSourceBlend;
		D3DBLEND		eDestBlend;
		D3DBLENDOP		eBlendOperation;
		D3DCMPFUNC		eCompareFunc;
		NiTexturePtr	spTexEffect;
		DWORD			uiAlphaRef;

		static void AddRecurse(NiAVObject* apObject, const NiColor& akColor, TextureEffectData* apEffect = nullptr);
		static void RemoveRecurse(NiAVObject* apObject);
	};

	struct LODTexParams {
		float fVal0;
		float fVal1;
		float fFadeTime;
		float fTextureScale;
	};

	UInt32									unk07C;
	float									fMorphDistance;
	NiColorA								kHairTint;
	NiPoint4								kLandBlendParams;
	BSShaderTextureSetPtr					spTextureSet;
	UInt16									usLandscapeTextures;
	NiTexturePtr*							ppTextures[6];
	SpecularExponents*						pLandSpecularExponents;
	UInt16									usLandPassCount;
	SpecularAvailabilities*					pLandSpecularStatus;
	TangentSpaceDataPtr						spTangentSpaceData;
	SInt8*									pcTexturePercents;
	NiTexturingProperty::ClampMode			eClampMode;
	TextureEffectDataPtr					spTexEffectData;
	float									fRefractionStrength;
	UInt32									iRefractionFirePeriod;
	float									fParallaxMaxPasses;
	float									fParallaxScale;
	LODTexParams							kLODTextureParams;
	RenderPass*								pDepthPass;

	CREATE_OBJECT(BSShaderPPLightingProperty, 0xB68D50);

	NiTexture* GetDiffuse(UInt32 auID = 0) const { return ppTextures[BSTextureSet::BSTS_TEX_DIFFUSE][auID]; };
	NiTexture* GetNormal(UInt32 auID = 0) const;
	NiTexture* GetGlow(UInt32 auID = 0) const { return ppTextures[BSTextureSet::BSTS_TEX_GLOW][auID]; };
	NiTexture* GetParallax(UInt32 auID = 0) const { return ppTextures[BSTextureSet::BSTS_TEX_HEIGHT][auID]; };
	NiTexture* GetCubeMap(UInt32 auID = 0) const { return ppTextures[BSTextureSet::BSTS_TEX_ENV][auID]; };
	NiTexture* GetEnvMask(UInt32 auID = 0) const { return ppTextures[BSTextureSet::BSTS_TEX_ENV_MASK][auID]; };

	void SetLODTexParams(float afVal0, float afVal1, float afFade);

	static UInt16 __fastcall GetNumberOfPassesEx(BSShaderPPLightingProperty* apThis, void*, NiGeometry* apGeometry);
	Lighting30ShaderProperty* PickShader(NiGeometry* apGeometry);
	static Lighting30ShaderProperty* __fastcall ConvertTo30Shader_Hook(BSShaderPPLightingProperty* apThis, void*, NiGeometry* apGeometry, void*, void*);

	BSShaderProperty::RenderPassArray* AddWaterDepthPass(NiGeometry* apGeometry) const;

	static void __fastcall CountTexturesEx(BSShaderPPLightingProperty* apThis, void*, void(__cdecl* apFunc)(NiTexture*, UInt32, BGSTextureUseMap**), BGSTextureUseMap** apMap);

	static bool __fastcall SetupGeometryEx(BSShaderPPLightingProperty* apThis, void*, NiGeometry* apGeometry);
	static BSShaderProperty::RenderPassArray* __fastcall GetRenderPassesEx(BSShaderPPLightingProperty* apThis, void*, NiGeometry* apGeometry, UInt32 aeEnabledPasses, WORD* pusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
	static void __fastcall GetViewerStringsEx(BSShaderPPLightingProperty* apThis, void*, NiViewerStringsArray* apStrings);
	static void __fastcall AddPassesEx(BSShaderPPLightingProperty* apThis, void*, NiGeometry* apGeometry, UInt32 aeEnabledPasses, WORD* apusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);

};

ASSERT_SIZE(BSShaderPPLightingProperty, 0x104);
ASSERT_SIZE(BSShaderPPLightingProperty::TextureEffectData, 0x74)