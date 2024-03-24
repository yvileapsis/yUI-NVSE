#pragma once

#include "ImageSpaceEffect.hpp"

class ImageSpaceEffectHDR : public ImageSpaceEffect {
public:
	struct HDRParams
	{
		float fEyeAdaptSpeed;
		float fNumBlurPasses;
		float Unk08;
		float fEmissiveHDRMult;
		float fTargetLUM;
		float fUpperLUMClamp;
		float fBrightScale;
		float fBrightClamp;
		float Unk20;
		float Unk24;
		float Unk28;
	};

	UInt32		unk058;
	UInt32		unk05C;
	UInt32		unk060;
	UInt32		unk064;
	bool		bIsLowRes;
	HDRParams*	pHDRParams;

	static float* const fLumRamp;

	static float* const fSkinDimmer;
	static float* const fGrassDimmer;
	static float* const fTreeDimmer;
	static float* const fSunlightDimmer;
};
