#pragma once

#include "NiDX9RenderedTextureData.hpp"

class NiRenderedCubeMap;

class NiDX9RenderedCubeMapData : public NiDX9RenderedTextureData {
public:
	NiDX9RenderedCubeMapData();
	virtual ~NiDX9RenderedCubeMapData();

	NiNewRTTI(NiDX9RenderedCubeMapData, NiDX9RenderedTextureData);

	static NiPixelFormat* __fastcall CreateSurf(NiDX9RenderedCubeMapData* apThis, void*, NiRenderedCubeMap* pkTexture);
};