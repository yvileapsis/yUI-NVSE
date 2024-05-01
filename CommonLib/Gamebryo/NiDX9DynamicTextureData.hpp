#pragma once

#include "NiDX9TextureData.hpp"

class NiDX9DynamicTextureData : public NiDX9TextureData {
public:
	NiDX9DynamicTextureData();
	virtual ~NiDX9DynamicTextureData();

	LPDIRECT3DBASETEXTURE9	m_pkD3DLockableSysMemTexture;
	bool				m_bTextureLocked;

	NiNewRTTI(NiDX9DynamicTextureData, NiDX9TextureData);
};

ASSERT_SIZE(NiDX9DynamicTextureData, 0x74);