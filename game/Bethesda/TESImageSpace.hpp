#pragma once
#include "TESForm.hpp"
#include "NiColor.hpp"

// 0x80
class TESImageSpace : public TESForm {
public:
	TESImageSpace();
	~TESImageSpace();

	struct HDRValues {
		union {
			struct {
				float fEyeAdaptSpeed;
				float fBlurRadius;
				float fBlurPasses;
				float fEmissiveMult;
				float fTargetLUM;
				float fUpperLUMClamp;
				float fBrightScale;
				float fBrightClamp;
				float fLUMRampNoTex;
				float fLUMRampMin;
				float fLUMRampMax;
				float fSunlightDimmer;
				float fGrassDimmer;
				float fTreeDimmer;
				float fSkinDimmer;
			};
			float fHDRValues[15];
		};
	};

	struct BloomValues {
		union {
			struct {
				float fBlurRadius;
				struct {
					float fInterior;
					float fExterior;
				} kAlphaMults;
			};
			float fBloomValues[3];
		};
	};

	struct GetHitValues {
		union {
			struct {
				float fBlurRadius;
				float fBlurDamping;
				float fDamping;
			};
			float fGetHitValues[3];
		};
	};

	struct NightEyeValues {
		union {
			struct {
				NiColor kColor;
				float	fBrightness;
			};
			float	fNightEyeValues[4];
		};
	};

	struct CinematicValues {
		union {
			struct {
				float	fSaturation;
				struct {
					float	fAvgLumValue;
					float	fValue;
				} kContrast;
				float	fBrightness;
				struct {
					NiColor kColor;
					float	fValue;
				} kTint;
			};
			float	fCinematicValues[8];
		};
	};

	union {
		struct {
			HDRValues		kHDR;
			BloomValues		kBloom;
			GetHitValues	kGetHit;
			NightEyeValues	kNightEye;
			CinematicValues	kCinematic;
		};
		float		fTraitValues[33]; // 132 bytes
	};
	UInt32		unused9C;
	UInt32		unusedA0;
	UInt32		unusedA4;
	Bitfield32	uiFlags;
	UInt32		unusedAC;
};
static_assert(sizeof(TESImageSpace) == 0xB0);