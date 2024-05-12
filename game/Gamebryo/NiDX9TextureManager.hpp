#pragma once

#include <d3d9.h>
#include "NiRefObject.hpp"

class NiDX9Renderer;
class NiTexture;

NiSmartPointer(NiDX9TextureManager);

class NiDX9TextureManager : public NiRefObject {
public:
	NiDX9TextureManager();
	virtual ~NiDX9TextureManager();

	virtual LPDIRECT3DTEXTURE9		GetTexture();
	virtual LPDIRECT3DBASETEXTURE9	PrepareTextureForRendering(NiTexture* pkNewTexIm, bool& bChanged, bool& bMipmap, bool& bNonPow2);
	virtual void					PrecacheTexture(NiTexture* pkIm, bool bForceLoad, bool bLocked);
	virtual UInt32					GetFormatFromTexture();

	LPDIRECT3DDEVICE9	m_pkD3DDevice9;
	NiDX9Renderer*		m_pkRenderer;

	static LPDIRECT3DBASETEXTURE9 __fastcall PrepareTextureForRenderingEx(NiDX9TextureManager* apThis, void*, NiTexture* apNewTexIm, bool& abChanged, bool& abMipmap, bool& abNonPow2);
};