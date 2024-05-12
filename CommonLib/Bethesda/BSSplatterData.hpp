#pragma once

#include "NiPoint3.hpp"

struct BSSplatterData {
	float	fAge;
	float	fDuration;
	float	fAlpha;
	UInt32	uiCount;
};

struct BSSplatterBonusData : public BSSplatterData {
	bool		bUseGravity;
	bool		bUseWind;
	bool		bUseRotation;
	NiPoint3	kGravityVector;
	float		fRotationSpeed;
	float		fRotationDirection;
};