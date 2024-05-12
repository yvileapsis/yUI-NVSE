#pragma once

#include "TESForm.hpp"
#include "TESTexture.hpp"
#include "NiPoint3.hpp"
#include "d3d9types.h"

class BGSDebris;

// 0x170
class TESEffectShader : public TESForm
{
public:
	TESEffectShader();
	~TESEffectShader();

	class EffectShaderData
	{
	public:
		UInt8 ucFlags;
		D3DBLEND eTextureBlendModeSource;
		D3DBLENDOP eTextureBlendOperation;
		D3DCMPFUNC eTextureZTestFunction;
		UInt32 uiFillColor1;
		float fFillAlphaFadeInTime;
		float fFillAlphaFullTime;
		float fFillAlphaFadeOutTime;
		float fFillAlphaPersistentPercent;
		float fFillAlphaPulseAmplitude;
		float fFillAlphaPulseFrequency;
		float fFillTextureUAnimSpeed;
		float fFillTextureVAnimSpeed;
		float fEdgeExponentValue;
		UInt32 uiEdgeColor;
		float fEdgeAlphaFadeInTime;
		float fEdgeAlphaFullTime;
		float fEdgeAlphaFadeOutTime;
		float fEdgeAlphaPersistentPercent;
		float fEdgeAlphaPulseAmplitude;
		float fEdgeAlphaPulseFrequency;
		float fFillAlphaFullPercent;
		float fEdgeAlphaFullPercent;
		D3DBLEND eTextureBlendModeDest;
		UInt32 particleSourceBlendMode;
		UInt32 particleBlendOp;
		UInt32 particleZTestFunc;
		UInt32 particleDestBlendMode;
		float particleBirthRampUpTime;
		float particleBirthFullTime;
		float particleBirthRampDownTime;
		float particleBirthFullRatio;
		float particleBirthPersistRatio;
		float particleLifetime;
		float particleLifetimeVar;
		float particleInitSpeedAlongNormal;
		float particleAccelAlongNormal;
		NiPoint3 initialVelocity;
		NiPoint3 acceleration;
		float scaleKey1;
		float scaleKey2;
		float scaleKey1Time;
		float scaleKey2Time;
		UInt32 colorKey1RGB;
		UInt32 colorKey2RGB;
		UInt32 colorKey3RGB;
		float colorKey1Alpha;
		float colorKey2Alpha;
		float colorKey3Alpha;
		float colorKey1Time;
		float colorKey2Time;
		float colorKey3Time;
		float particleInitSpeedAlongNormalVar;
		float particleInitRotDeg;
		float particleInitRotDegVar;
		float particleRotSpeedDegPerSec;
		float particleRotSpeedDegPerSecVar;
		BGSDebris* addonModels;
		float holesStartTime;
		float holesEndTime;
		float holesStartVal;
		float holesEndVal;
		float edgeWidthAlphaUnits;
		UInt32 edgeColorRGB;
		float explosionWindSpeed;
		UInt32 textureCountU;
		UInt32 textureCountV;
		float addonFadeInTime;
		float addonFadeOutTime;
		float addonScaleStart;
		float addonScaleEnd;
		float addonScaleInTime;
		float addonScaleOutTime;
	};

	EffectShaderData Data;
	TESTexture fillTexture;
	TESTexture particleShaderTexture;
	TESTexture holesTexture;
};