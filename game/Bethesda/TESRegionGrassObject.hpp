#pragma once

#include "TESRegionObjectBase.hpp"

class TESGrass;
class TESLandTexture;

class TESRegionGrassObject : public TESRegionObjectBase {
public:
	TESGrass* pObject;
	TESLandTexture* pParent;
};

ASSERT_SIZE(TESRegionGrassObject, 0xC)