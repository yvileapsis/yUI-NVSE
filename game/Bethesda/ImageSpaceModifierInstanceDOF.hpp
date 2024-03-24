#pragma once

#include "ImageSpaceModifierInstanceTemp.hpp"

class ImageSpaceModifierInstanceDOF : public ImageSpaceModifierInstanceTemp {
public:
	ImageSpaceModifierInstanceDOF();
	virtual ~ImageSpaceModifierInstanceDOF();

	float	fDistance;
	float	fRange;
	float	fMotionBlur;
	UInt32	eMode;

	NiNewRTTI(ImageSpaceModifierInstanceDOF, ImageSpaceModifierInstanceTemp);

	void SetDistance(float afDistance) { fDistance = afDistance; }
	float GetDistance() const { return fDistance; }

	void SetRange(float afRange) { fRange = afRange; }
	float GetRange() const { return fRange; }

	void SetMotionBlur(float afMotionBlur) { fMotionBlur = afMotionBlur; }
	float GetMotionBlur() const { return fMotionBlur; }

	void SetMode(UInt32 aeMode) { eMode = aeMode; }
	UInt32 GetMode() const { return eMode; }

	static ImageSpaceModifierInstanceDOF* Trigger(float afDistance, float afRange, int aMode, float afStrength, float afMotionBlur, float afDuration);
};

ASSERT_SIZE(ImageSpaceModifierInstanceDOF, 0x30);