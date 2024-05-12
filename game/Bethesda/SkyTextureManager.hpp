#pragma once

class NiSourceTexture;

class SkyTextureManager {
public:
	static void RequestTexture(NiSourceTexture* apTexture, const char* apTexturePath);
};