#pragma once

#include "BSMemObject.hpp"
#include "BSSimpleList.hpp"
#include "NiTexture.hpp"
#include "NiTMap.hpp"

class TEX_USER_DATA {
public:
	NiTexturePtr			spNiTexture;
	UInt32					uiSize;
	BSSimpleList<UInt32>	kList;
};

class BGSTextureUseMap : public BSMemObject {
public:
	NiTMap<NiTexture*, TEX_USER_DATA*> kUserMap;
};