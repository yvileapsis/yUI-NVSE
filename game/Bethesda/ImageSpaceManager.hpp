#pragma once

#include "ImageSpaceBaseData.hpp"
#include "ImageSpaceEffectDepthOfField.hpp"
#include "NiTriShape.hpp"

class ImageSpaceEffect;

class ImageSpaceManager {
public:
	enum EffectID : UInt32 {
		IS_EFFECT_NONE								= 0xFFFFFF,
		IS_EFFECT_BLOOM								= 0,
		IS_EFFECT_HDR								= 1,
		IS_EFFECT_REFRACTION						= 2,
		IS_EFFECT_CINEMATIC							= 3,
		IS_EFFECT_DEPTH_OF_FIELD					= 4,
		IS_EFFECT_DEPTH_RADIAL_BLUR					= 5,
		IS_EFFECT_RADIAL_BLUR						= 6,
		IS_EFFECT_FULLSCREEN_BLUR					= 7,
		IS_EFFECT_GET_HIT							= 8,
		IS_EFFECT_SUNBEAMS							= 9,
		IS_EFFECT_INTERFACE							= 10,
		IS_EFFECT_VATS_SCAN							= 11,
		IS_EFFECT_MOTION_BLUR						= 12,
		IS_EFFECT_VOLUMETRIC_FOG					= 13,
		IS_EFFECT_MAP								= 14,
		IS_EFFECT_MENU_BG							= 15,
		IS_EFFECT_BLUR_3							= 16,
		IS_EFFECT_BLUR_5							= 17,
		IS_EFFECT_BLUR_7							= 18,
		IS_EFFECT_BLUR_9							= 19,
		IS_EFFECT_BLUR_11							= 20,
		IS_EFFECT_BLUR_13							= 21,
		IS_EFFECT_BLUR_15							= 22,
		IS_EFFECT_BRIGHTPASS_BLUR_3					= 23,
		IS_EFFECT_BRIGHTPASS_BLUR_5					= 24,
		IS_EFFECT_BRIGHTPASS_BLUR_7					= 25,
		IS_EFFECT_BRIGHTPASS_BLUR_9					= 26,
		IS_EFFECT_BRIGHTPASS_BLUR_11				= 27,
		IS_EFFECT_BRIGHTPASS_BLUR_13				= 28,
		IS_EFFECT_BRIGHTPASS_BLUR_15				= 29,
		IS_EFFECT_TV								= 30,
		IS_EFFECT_WATER_FFT							= 31,
		IS_EFFECT_WATER_DISPLACEMENT				= 32,
		IS_EFFECT_NOISE								= 33,
		IS_EFFECT_MAX								= 34,
		IS_SHADER_COPY								= 34,
		IS_SHADER_COPY_RENDERTARGET1				= 35,
		IS_SHADER_RESTORE_EDRAM						= 36,
		IS_SHADER_NULL								= 37,
		IS_SHADER_COPY_ALPHA						= 38,
		IS_SHADER_COPY_STENCIL						= 39,
		IS_SHADER_COPY_STENCIL_NO_TEXTURE_NO_DEPTH	= 40,
		IS_SHADER_REFRACTION						= 41,
		IS_SHADER_VOLUMETRIC_FOG					= 42,
		IS_SHADER_BLUR								= 43,
		IS_SHADER_DOUBLE_VISION						= 44,
		IS_SHADER_ALPHA_MULT						= 45,
		IS_SHADER_BLEND								= 46,
		IS_SHADER_TEXTURE_MASK						= 47,
		IS_SHADER_MAP								= 48,
		IS_SHADER_MENU_BG							= 49,
		IS_SHADER_CINEMATIC							= 50,
		IS_SHADER_DEPTH_OF_FIELD					= 51,
		IS_SHADER_DEPTH_OF_FIELD_MASK				= 52,
		IS_SHADER_MOTION_BLUR						= 53,
		IS_SHADER_RADIAL_BLUR						= 54,
		IS_SHADER_RADIAL_BLUR_MEDIUM				= 55,
		IS_SHADER_RADIAL_BLUR_HIGH					= 56,
		IS_SHADER_SUNBEAMS = 57,
		IS_SHADER_HDR_BLEND = 58,
		IS_SHADER_HDR_BLEND_CINEMATIC = 59,
		IS_SHADER_HDR_BLEND_CINEMATIC_ALPHA_MASK = 60,
		IS_SHADER_HDR_BLUR = 61,
		IS_SHADER_BRIGHTPASS_FILTER = 62,
		IS_SHADER_DOWNSAMPLE_16 = 63,
		IS_SHADER_DOWNSAMPLE_9 = 64,
		IS_SHADER_DOWNSAMPLE_4 = 65,
		IS_SHADER_DOWNSAMPLE_4_LUM_CLAMP = 66,
		IS_SHADER_DOWNSAMPLE_4_LIGHT_ADAPT = 67,
		IS_SHADER_DOWNSAMPLE_16_LUM_CLAMP = 68,
		IS_SHADER_DOWNSAMPLE_16_LIGHT_ADAPT = 69,
		IS_SHADER_LIGHT_ADAPT = 70,
		IS_SHADER_LUM_CLAMP = 71,
		IS_SHADER_SCANLINES_BLEND = 72,
		IS_SHADER_SCANLINES_BLEND_1 = 73,
		IS_SHADER_VATS_SCAN = 75,
		IS_SHADER_MENU_UNPACK = 76,
		IS_SHADER_TV = 77,
		IS_SHADER_BLUR_3 = 78,
		IS_SHADER_BLUR_5 = 79,
		IS_SHADER_BLUR_7 = 80,
		IS_SHADER_BLUR_9 = 81,
		IS_SHADER_BLUR_11 = 82,
		IS_SHADER_BLUR_13 = 83,
		IS_SHADER_BLUR_15 = 84,
		IS_SHADER_BRIGHTPASS_BLUR_3 = 85,
		IS_SHADER_BRIGHTPASS_BLUR_5 = 86,
		IS_SHADER_BRIGHTPASS_BLUR_7 = 87,
		IS_SHADER_BRIGHTPASS_BLUR_9 = 88,
		IS_SHADER_BRIGHTPASS_BLUR_11 = 89,
		IS_SHADER_BRIGHTPASS_BLUR_13 = 90,
		IS_SHADER_BRIGHTPASS_BLUR_15 = 91,
		IS_SHADER_WATER_FFT_WATER_SPECTRUM = 93,
		IS_SHADER_WATER_FFT_HORIZONTAL_BUTTERFLY = 94,
		IS_SHADER_WATER_FFT_VERTICAL_BUTTERFLY = 95,
		IS_SHADER_WATER_FFT_HORIZONTAL_SCRAMBLE = 96,
		IS_SHADER_WATER_FFT_VERTICAL_SCRAMBLE = 97,
		IS_SHADER_WATER_FFT_NORMALS = 98,
		IS_SHADER_WATER_FFT_FILTER = 99,
		IS_SHADER_WATER_FFT_DISPLAY_NORMALS = 100,
		IS_SHADER_WATER_FFT_DISPLAY_HIGHW = 101,
		IS_SHADER_WATER_FFT_DISPLAY_AMPLITUTE = 102,
		IS_SHADER_WATER_FFT_WATER_SPECTRUM_1 = 103,
		IS_SHADER_WATER_FFT_DISPLAY_WATER_HEIGHT = 104,
		kWaterDisplaceClearSimulation = 105,
		kWaterDisplaceTexOffset = 106,
		kWaterDisplacementWadingRipple = 107,
		kWaterDisplacementRainRipple = 108,
		kWaterWadingHeightmap = 109,
		kWaterRainHeightmap = 110,
		kWaterBlendHeightmaps = 111,
		kWaterSmoothHeightmap = 112,
		kWaterDisplacementNormals = 113,
		IS_SHADER_NOISE_SCROLL_AND_BLEND = 114,
		IS_SHADER_NOISE_NORMAL_MAP = 115,
	};


	BSRenderedTexture*						pMainTarget;
	NiTPrimitiveArray<ImageSpaceEffect*>	kEffects;
	ImageSpaceBaseData						kBaseData;
	ImageSpaceBaseData*						pOverrideBaseData;
	ImageSpaceBaseData*						pActiveImageSpaceParams;
	float									unk0B4[76];
	bool									bIsReady;
	UInt32									iActiveEffectsCount;
	UInt32									iLastEffect;
	NiTriShapePtr							spScreenGeometry;
	NiGeometryDataPtr						spScreenTriShapeData;
	NiTriShapePtr							spDynamicScreenTriShape;
	bool									bUseDynamicResolution;
	NiTriShapePtr							spPartialScreenTriShape;
	ImageSpaceTexture						kDepthTexture;
	NiColorA								kColor214;
	NiColorA								kTintColor;
	float									fUnk234;
	NiColorA								kColor238;
	NiColorA								kFadeColor;
	float									fUnk258;
	float									fUnk25C;
	float									fBlurRadius;
	float									fUnk264;
	float									fDoubleVisionStrength;
	float									fStrength;
	float									fRampUp;
	float									fStart;
	float									fRampDown;
	float									fDownStart;
	float									fRadialBlur280;
	float									fRadialBlur284;
	float									fRadialBlur288;
	float									fRadialBlur28C;
	float									fRadialBlur290;
	NiPoint2								kCenter;
	NiPoint2								kRadialBlurCenter;
	float									fStrength2;
	float									fUnk2A8;
	float									fUnk2AC;
	float									fUnk2B0;
	float									fUnk2B4;
	float									fUnk2B8;
	float									fUnk2BC;
	float									fUnk2C0;
	UInt32									unk2C4;
	UInt32									uiMode;
	bool									bUseMaskedDOF;
	UInt8									byte2CD;
	float									fUnk2D0;
	float									fUnk2D4;
	float									fUnk2D8;
	float									fUnk2DC;
	float									fUnk2E0;
	UInt32									unk2E4;
	UInt32									unk2E8;
	UInt32									unk2EC;
	float									fMotionBlur;
	float									fMotionBlurStrength;
	NiColorA								kRefractionTint;
	NiViewport								kViewport;
	BSRenderedTexture*						pSwapTarget;

	static __forceinline ImageSpaceManager* GetSingleton() {
		return *(ImageSpaceManager**)0x11F91AC;
	}

	ImageSpaceEffect* GetEffect(UInt32 effectID) const {
		return kEffects.GetAt(effectID);
	}

	// Source Texture
	void RenderEffect(ImageSpaceEffect* apEffect, NiDX9Renderer* apRenderer, NiSourceTexture* apSourceTarget, BSRenderedTexture* apDestTarget, ImageSpaceEffectParam* apParam, bool abEndFrame);
	void RenderEffect(ImageSpaceManager::EffectID aeID, NiDX9Renderer* apRenderer, NiSourceTexture* apSourceTarget, BSRenderedTexture* apDestTarget, ImageSpaceShaderParam* apParam, bool abEndFrame);

	// Rendered Texture
	void RenderEffect(ImageSpaceEffect* apEffect, NiDX9Renderer* apRenderer, BSRenderedTexture* apSourceTarget, BSRenderedTexture* apDestTarget, ImageSpaceEffectParam* apParam, bool abEndFrame);
	void RenderEffect(ImageSpaceManager::EffectID aeID, NiDX9Renderer* apRenderer, BSRenderedTexture* apSourceTarget, BSRenderedTexture* apDestTarget, ImageSpaceEffectParam* apParam, bool abEndFrame);

	void RenderEffect(ImageSpaceManager::EffectID aeID, NiDX9Renderer* apRenderer, BSRenderedTexture* apDestTarget, ImageSpaceEffectParam* apParam, bool abEndFrame);

	void RenderEffectRange(NiDX9Renderer* apRenderer, BSRenderedTexture*& aprSourceTarget, BSRenderedTexture*& aprDestTarget);

	static __forceinline bool AreEOFShaderEnabled() {
		return *(bool*)0x11AD884;
	}

	void PrepareEffectRange();
};

ASSERT_SIZE(ImageSpaceManager, 0x31C)