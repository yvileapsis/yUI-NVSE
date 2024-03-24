#pragma once

#include "ImageSpaceModifierInstance.hpp"

class TESImageSpaceModifier;

class ImageSpaceModifierInstanceForm : public ImageSpaceModifierInstance {
public:
	TESImageSpaceModifier*	pImageSpaceMod;
	TESImageSpaceModifier*	pLastImageSpaceMod;
	float					fLastStrength;
	NiObjectPtr				spLastTarget;
	float					fTransitionTime;

	NiNewRTTI(ImageSpaceModifierInstanceForm, ImageSpaceModifierInstance);
};