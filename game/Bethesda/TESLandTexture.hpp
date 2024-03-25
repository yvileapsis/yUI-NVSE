#pragma once
#include "TESForm.hpp"

class BGSTextureSet;
class TESGrass;

// 28
class TESLandTexture : public TESForm {
public:
	TESLandTexture();
	~TESLandTexture();

	BGSTextureSet*			pkTextureSet;		// 18
	UInt8					ucMaterialType;		// 1C
	UInt8					ucFriction;			// 1D
	UInt8					ucRestitution;		// 1E
	char					cSpecularExponent;	// 1F
	BSSimpleList<TESGrass*>	kTextureGrassList;	// 20

	SInt32	GetGrassIndex(TESGrass* grass);
};
static_assert(sizeof(TESLandTexture) == 0x28);