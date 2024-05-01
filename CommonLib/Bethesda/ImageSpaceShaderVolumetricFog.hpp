#pragma once

#include "ImageSpaceShader.hpp"

class ImageSpaceShaderVolumetricFog : public ImageSpaceShader {
public:
	BSRenderedTexture* GetTexture();
	void FreeTexture();
};
