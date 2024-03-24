#pragma once

#include "BSRenderedTexture.hpp"
#include "NiColor.hpp"
#include "NiPoint3.hpp"
#include "NiPoint4.hpp"
#include "NiSourceTexture.hpp"
#include "TESAttackDamageForm.hpp"
#include "TESForm.hpp"
#include "TESTexture.hpp"

class TESSound;
class SpellItem;
class BGSPlaceableWater;

struct WaterShaderData {
	float		float0;
	float		float4;
	float		float8;
	float		floatC;
	float		fSunSpecularPower;
	float		fReflectivityAmount;
	float		fFresnelAmount;
	float		float1C;
	float		fFogNear;
	float		fFogFar;
	DWORD		uiShallowColor;
	DWORD		uiDeepColor;
	DWORD		uiReflectionColor;
	float		float34;
	float		fRainForce;
	float		fRainVelocity;
	float		fRainFalloff;
	float		fRainDampener;
	float		fDisplacementStartingSize;
	float		fDisplacementForce;
	float		fDisplacementVelocity;
	float		fDisplacementFalloff;
	float		fDisplacementDampener;
	float		fRainStartingSize;
	float		fNoiseScale;
	float		fWindDirections[3];
	float		fWindSpeeds[3];
	float		fDepthFalloffStart;
	float		fDepthFalloffEnd;
	float		fFogScale;
	float		fNoiseUVScale;
	float		fUnderwaterFogScale;
	float		fUnderwaterFogNear;
	float		fUnderwaterFogFar;
	float		fDistortionAmount;
	float		fShininess;
	float		fHDRMult;
	float		fLightRadius;
	float		fLightBrightness;
	float		fUVScales[3];
	float		fAmplitudeScales[3];
};

ASSERT_SIZE(WaterShaderData, 0xC4);

class TESWaterForm : public TESForm, TESFullName, TESAttackDamageForm {
public:
	TESWaterForm();
	~TESWaterForm();

	bool					bNeedUpdate;
	BSRenderedTexturePtr	spNormalNoiseTexture;
	NiPoint4				kTexScrolls[3];
	TESTexture				kNoiseTexture;
	char					cAlpha;
	char					cFlags;
	BSStringT				strUnk074;
	TESSound*				pWaterSound;
	TESWaterForm*			pWaterForm;
	WaterShaderData			kShaderData;
	TESWaterForm*			pWaterWeatherControl[3];
	int						iCurrentTextureSelect01;
	int						iCurrentTextureSelect02;
	UInt32					uiFrequencyX;
	UInt32					uiFrequencyY;
	int						iOctaves;
	float					fAmplitude;
	float					fLacunarity;
	float					fBias;
	float					fGain;
	SpellItem*				pConsumeSpell;
	NiSourceTexturePtr		spNoiseTexture;
	BGSPlaceableWater*		PlaceableAutoWater;
	BGSPlaceableWater*		pPlaceableAutoWater;
	UInt32					radiation;
	UInt32					unk18C;
	bool					bResetNoiseTextures;

	const char* GetNoiseTextureName();
	UInt32 GetNoiseTextureNameLength();
	BGSPlaceableWater* GetPlaceableLODWater();
};

ASSERT_SIZE(TESWaterForm, 0x194);