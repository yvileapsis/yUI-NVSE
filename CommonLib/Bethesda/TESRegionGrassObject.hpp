#pragma once
#include "TESRegionObjectBase.hpp"

class TESGrass;
class TESLandTexture;

class TESRegionGrassObject : public TESRegionObjectBase {
public:
	TESGrass* pObject;
	TESLandTexture* pParent;
};
static_assert(sizeof(TESRegionGrassObject) == 0xC);