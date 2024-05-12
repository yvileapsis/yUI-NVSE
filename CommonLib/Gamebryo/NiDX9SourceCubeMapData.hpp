#pragma once

#include "NiDX9SourceTextureData.hpp"

class NiDX9SourceCubeMapData : public NiDX9SourceTextureData {
public:
	NiDX9SourceCubeMapData();
	virtual ~NiDX9SourceCubeMapData();

	UInt32 m_uiSourceRevID;

	NiNewRTTI(NiDX9SourceCubeMapData, NiDX9SourceTextureData);
};