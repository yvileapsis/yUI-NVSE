#pragma once
#include "TESModel.hpp"
#include "BSSimpleList.hpp"

// 0x20
class TESModelTextureSwap : public TESModel {
public:
	TESModelTextureSwap();

	~TESModelTextureSwap() override;

	struct Texture
	{
		UInt32	uiTextureID;			// 00
		UInt32	uiIndex3D;				// 04
		char	cTextureName[0x80];		// 08
	};	// there seem to be an array (length 6) post 0x88

	BSSimpleList<Texture*> kTextureList;	// 018
};
static_assert(sizeof(TESModelTextureSwap) == 0x20);