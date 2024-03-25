#pragma once

#include "NiRenderedTexture.hpp"
#include "NiRenderTargetGroup.hpp"
#include "NiRenderedCubeMap.hpp"
#include "BSString.hpp"
#include "NiDX9TextureData.hpp"

NiSmartPointer(BSRenderedTexture);

class BSRenderedTexture : public NiObject {
public:
	NiRenderTargetGroupPtr	spRenderTargetGroups[6];
	NiRenderTargetGroupPtr	spSomeRT;
	NiObjectPtr				spObject024;
	NiObjectPtr				spObject028;
	SInt32					eType; // BSTextureManager::RenderTargetTypes
	NiRenderedTexturePtr	spRenderedTextures[4];

	NiNewRTTI(BSRenderedTexture, NiObject);

	static UInt32* const uiCurrentRT;
	static NiRenderTargetGroupPtr* const spStoredRenderTargets;

	bool SaveTexture(const char* path, const D3DXIMAGE_FILEFORMAT aeFormat) const;

	static BSRenderedTexture* Create(BSStringT<char>* apName, const UInt32 uiWidth, const UInt32 uiHeight,
		NiTexture::FormatPrefs* kPrefs, Ni2DBuffer::MultiSamplePreference eMSAAPref,
		bool bUseDepthStencil, NiDepthStencilBuffer* pkDSBuffer, UInt32 a7, UInt32 uiBackgroundColor);

	static BSRenderedTexture* Create(const UInt32 uiWidth, const UInt32 uiHeight);

	static BSRenderedTexture* CreateCubeMap(const UInt32 auiSize);

	static void StoreRenderTarget(NiRenderTargetGroup* apTarget);
	static bool ReleaseUnusedRenderTargets();
	static void ReleaseCurrentRenderTarget();
	static void SwitchRenderTarget();
	static NiRenderTargetGroup* GetCurrentStoredRenderTarget();

	NiRenderedTexture* GetTexture(const UInt32 uiIndex) const;

	NiRenderTargetGroup* GetRenderTargetGroup() const;

	LPDIRECT3DBASETEXTURE9 GetD3DTexture(const UInt32 auiIndex, bool abRenderTarget) const;

	void SetOffScreenRenderTargetGroup(UInt32 auiClearMode) const;

	void SetOnScreenRenderTargetGroup(UInt32 auiClearMode) const;

	void SetRenderTargetGroup(UInt32 auiClearMode) const;

	UInt32 GetWidth(UInt32 auiTexture) const;

	UInt32 GetHeight(UInt32 auiTexture) const;

	void ClearTextures();
};

ASSERT_SIZE(BSRenderedTexture, 0x40);