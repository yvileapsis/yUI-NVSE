#pragma once

#include "NiTexture.hpp"
#include "NiPalette.hpp"

NiSmartPointer(NiPersistentSrcTextureRendererData);

class NiPersistentSrcTextureRendererData : public NiTexture::RendererData {
public:
	NiPersistentSrcTextureRendererData();
	virtual ~NiPersistentSrcTextureRendererData();

	virtual void AllocateData(UInt32 uiLevels, UInt32 uiFaces, UInt32 uiBytes);
	virtual void FreeData();

	enum PlatformID {
		NI_ANY = 0x0,
		NI_XENON = 0x1,
		NI_PS3 = 0x2,
		NI_DX9 = 0x3,
		NI_NUM_PLATFORM_IDS = 0x4,
	};

	UInt8*		m_pucPixels;
	UInt32*		m_puiWidth;
	UInt32*		m_puiHeight;
	UInt32*		m_puiOffsetInBytes;
	UInt32		m_uiPadOffsetInBytes;
	UInt32		m_uiMipmapLevels;
	UInt32		m_uiPixelStride;
	UInt32		m_uiFaces;
	PlatformID	m_eTargetPlatform;
	UInt8*		m_pucPristinePixels;
	UInt32		m_uiPristineMaxOffsetInBytes;
	UInt32		m_uiPristinePadOffsetInBytes;
	NiPalette*	m_spPalette;

	CREATE_OBJECT(NiPersistentSrcTextureRendererData, 0xA982D0)
};

ASSERT_SIZE(NiPersistentSrcTextureRendererData, 0x94)