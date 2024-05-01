#pragma once

#include "NiDX9TextureData.hpp"
#include "Ni2DBuffer.hpp"

class NiRenderedTexture;

class NiDX9RenderedTextureData : public NiDX9TextureData {
public:
	NiDX9RenderedTextureData();
	virtual ~NiDX9RenderedTextureData();

	LPDIRECT3DSURFACE9 pRenderTarget;

	static const NiPixelFormat* __fastcall CreateSurf(NiDX9RenderedTextureData* apThis, void*, const NiRenderedTexture* pkTexture, Ni2DBuffer::MultiSamplePreference eMSAAPref);
};