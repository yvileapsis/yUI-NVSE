#pragma once

#include "TESForm.hpp"

class BGSTextureSet;
class TESGrass;

class TESLandTexture : public TESForm {
public:
	TESLandTexture();
	~TESLandTexture();

	BGSTextureSet* pTextureSet;		// 18
	UInt8			ucMaterialType;		// 1C
	UInt8			ucFriction;			// 1D
	UInt8			ucRestitution;		// 1E
	char			cSpecularExponent;	// 1F
	BSSimpleList<TESGrass*>	TextureGrassList;			// 20

	SInt32	GetGrassIndex(TESGrass* grass);
};

ASSERT_SIZE(TESLandTexture, 0x28);