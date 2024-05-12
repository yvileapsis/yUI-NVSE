#pragma once
#include "NiFloatInterpolator.hpp"
#include "NiColorInterpolator.hpp"
#include "TESForm.hpp"
#include "NiPoint2.hpp"

class TESSound;

class TESImageSpaceModifier : public TESForm {
public:
	struct NiFloatKeyPair {
		NiFloatKey kKeys[2];
	};

	struct NiColorKeyPair {
		NiColorKey kKey[2];
	};

	struct ImageSpaceModifierData {
		bool		bAnimatable;
		float		fDuration;
		UInt32		uiKeySizes[21][2];
		UInt32		uiTintColorKeySize;
		UInt32		uiBlurKeySize;
		UInt32		uiDoubleKeySize;
		UInt32		uiRadialBlurStrengthKeySize;
		UInt32		uiRadialBlurRampUpKeySize;
		UInt32		uiRadialBlurStartKeySize;
		bool		bUseTargetForRadialBlur;
		NiPoint2	kRadialBlurCenter;
		UInt32		uiDepthOfFieldStrengthKeySize;
		UInt32		uiDepthOfFieldDistanceKeySize;
		UInt32		uiDepthOfFieldRangeKeySize;
		bool		bUseTargetForDepthOfField;
		char		ucDepthOfFieldMode;
		UInt32		uiRadialBlurRampDownKeySize;
		UInt32		uiRadialBlurDownStartKeySize;
		UInt32		uiFadeColorKeySize;
		UInt32		uiMotionBlurStrengthKeySize;
	};


	TESSound*				pOutroSound;
	TESSound*				pIntroSound;
	ImageSpaceModifierData	kData;
	NiFloatInterpolator		kInterpolators[21][2];
	NiFloatInterpolator		kBlurInterpolator;
	NiFloatInterpolator		kDoubleInterpolator;
	NiColorInterpolator		kTintColorInterpolator;
	NiColorInterpolator		kFadeColorInterpolator;
	NiFloatInterpolator		kRadialBlurStrengthInterpolator;
	NiFloatInterpolator		kRadialBlurRampUpInterpolator;
	NiFloatInterpolator		kRadialBlurStartInterpolator;
	NiFloatInterpolator		kRadialBlurRampDownInterpolator;
	NiFloatInterpolator		kDepthOfFieldStrengthInterpolator;
	NiFloatInterpolator		kDepthOfFieldDistanceInterpolator;
	NiFloatInterpolator		kDepthOfFieldRangeInterpolator;
	NiFloatInterpolator		kDepthOfFieldVignetteRadiusInterpolator;
	NiFloatInterpolator		kDepthOfFieldVignetteStrengthInterpolator;
	NiFloatKeyPair*			pFloatKeys[22][2];
	NiColorKeyPair*			pColorKeys[2];
	NiFloatKeyPair*			pFloatKeys70C[9];
};
static_assert(sizeof(TESImageSpaceModifier) == 0x730);