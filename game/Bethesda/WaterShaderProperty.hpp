#pragma once

#include "BSShaderProperty.hpp"
#include "BSRenderedTexture.hpp"

NiSmartPointer(WaterShaderProperty);

class WaterShaderProperty : public BSShaderProperty {
public:
	WaterShaderProperty();
	~WaterShaderProperty();

	struct VarAmounts
	{
		float unk;
		float fWaterReflectivityAmt;
		float fWaterOpacity;
		float fWaterDistortionAmt;
	};

	bool						bIsWading;
	bool						bIsLOD;
	bool						bRenderAtWaterLevel;
	bool						bDepth;
	DWORD						dword64;
	DWORD						dword68;
	float						blendRadiusX;
	float						blendRadiusY;
	float						fogPower;
	float						fog78;
	BYTE						byte7C;
	BYTE						byte7D;
	bool						bIsUnderwater;
	bool						bIsUseDefaultWater;
	bool						bReflect;
	bool						bRefract;
	BYTE						UV;
	bool						bHasLights;
	DWORD						uiStencilMask;
	NiColorA					kShallowColor;
	NiColorA					kDeepColor;
	NiColorA					kReflectionColor;
	VarAmounts					Vars;
	float						floatC8;
	float						floatCC;
	float						blendRadiusZ;
	float						floatD4;
	NiPoint4					kDepthData;
	float						floatE8;
	float						floatEC;
	float						floatF0;
	float						floatF4;
	float						floatF8;
	float						floatFC;
	float						fresnelZ;
	float						fresnelW;
	float						float108;
	float						float10C;
	float						float110;
	float						float114;
	float						fWaterFresnelTerm;
	float						fWaterNoise;
	float						fFogAmount;
	float						fTexScale;
	NiTPointerList<NiLight*>	kLights;
	NiSourceTexturePtr			spNoiseTexture;
	BSRenderedTexturePtr		spDistortionTexture;
	BSRenderedTexturePtr		spReflectionTexture;
	BSRenderedTexturePtr		spRefractionTexture;
	BSRenderedTexturePtr		spDepthTexture;
	NiTexturePtr				spWadingTexture;
	RenderPass*					pStencilPass;

	CREATE_OBJECT(WaterShaderProperty, 0xB6AE60);

	RenderPass* CreateStencilPass(NiGeometry* apGeo);
	NiSourceTexture* GetNoiseTexture() const;

	static RenderPassArray* __fastcall GetRenderPassesEx(WaterShaderProperty* apThis, void*, NiGeometry* apGeometry, UInt32 auiEnabledPasses, WORD* pusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
};

ASSERT_SIZE(WaterShaderProperty, 0x150);