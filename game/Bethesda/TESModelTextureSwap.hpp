#pragma once

#include "TESModel.hpp"
#include "BSSimpleList.hpp"

class TESModelTextureSwap : public TESModel {
public:
	TESModelTextureSwap();
	~TESModelTextureSwap();

	struct Texture {
		UInt32	uiTextureID;			// 00
		UInt32	uiIndex3D;			// 04
		char	cTextureName[0x80];	// 08
	};	// there seem to be an array (length 6) post 0x88


	BSSimpleList<Texture*> kTextureList;	// 018
};

ASSERT_SIZE(TESModelTextureSwap, 0x20);