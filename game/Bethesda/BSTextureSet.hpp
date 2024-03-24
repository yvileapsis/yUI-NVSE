#pragma once

#include "NiObject.hpp"

class NiTexture;

class BSTextureSet : public NiObject {
public:
	BSTextureSet();
	virtual ~BSTextureSet();

	enum TextureType {
		BSTS_TEX_DIFFUSE = 0x0,
		BSTS_TEX_NORMAL = 0x1,
		BSTS_TEX_GLOW = 0x2,
		BSTS_TEX_HEIGHT = 0x3,
		BSTS_TEX_ENV = 0x4,
		BSTS_TEX_ENV_MASK = 0x5,
	};

	virtual char* GetTexturePath(TextureType eTextureIndex);
	virtual void GetTexture(TextureType eTextureIndex, NiTexture*& ppTexture);
	virtual void SetTexturePath(TextureType eTextureIndex, const char* apPath);
};