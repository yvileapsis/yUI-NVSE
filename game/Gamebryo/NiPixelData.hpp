#pragma once

#include "NiObject.hpp"
#include "NiPalette.hpp"

NiSmartPointer(NiPixelData);

class NiPixelData : public NiObject {
public:
	NiPixelData();
	virtual ~NiPixelData();

	NiPixelFormat	m_kPixelFormat;
	NiPalettePtr	m_spPalette;
	UInt8*			m_pucPixels;
	UInt32*			m_puiWidth;
	UInt32*			m_puiHeight;
	UInt32*			m_puiOffsetInBytes;
	UInt32			m_uiMipmapLevels;
	UInt32			m_uiPixelStride;
	UInt32			m_uiRevID;
	UInt32			m_uiFaces;
	bool			bIsInternal;

	CREATE_OBJECT(NiPixelData, 0xA7C3F0)
};

ASSERT_SIZE(NiPixelData, 0x74)