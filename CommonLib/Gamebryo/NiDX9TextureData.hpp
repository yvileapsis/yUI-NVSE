#pragma once
#include "NiPersistentSrcTextureRendererData.hpp"
#include "NiTexture.hpp"

class NiDX9Renderer;
class NiDX9SourceTextureData;
class NiDX9RenderedTextureData;
class NiDX9DynamicTextureData;

class NiDX9TextureData : public NiTexture::RendererData {
public:
	NiDX9TextureData();
	virtual ~NiDX9TextureData();

	virtual UInt32						GetLevels() const;
	virtual LPDIRECT3DBASETEXTURE9		GetD3DTexture() const;
	virtual void						SetD3DTexture(LPDIRECT3DBASETEXTURE9 pkD3DTexture);
	virtual NiDX9RenderedTextureData*	GetAsRenderedTexture() const;
	virtual NiDX9SourceTextureData*		GetAsSourceTexture() const;
	virtual NiDX9DynamicTextureData*	GetAsDynamicTexture() const;
	virtual bool						InitializeFromD3DTexture(LPDIRECT3DBASETEXTURE9 pkD3DTexture);

	NiDX9Renderer*			m_pkRenderer;
	LPDIRECT3DBASETEXTURE9	m_pkD3DTexture;
	UInt32					m_uiLevels;

	NiNewRTTI(NiDX9TextureData, NiTexture::RendererData);

	static NiPixelFormat* FindClosestPixelFormat(const NiTexture::FormatPrefs* kPrefs, NiPixelFormat* const* ppkDestFmts);

// TODO: ask wall
//	bool SaveTexture(const char* path, D3DXIMAGE_FILEFORMAT aeFormat) {
//		return D3DXSaveTextureToFile(path, aeFormat, m_pkD3DTexture, NULL);
//	}
};

ASSERT_SIZE(NiDX9TextureData, 0x6C);