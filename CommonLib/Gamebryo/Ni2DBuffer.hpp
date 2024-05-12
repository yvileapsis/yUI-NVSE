#pragma once

#include "NiObject.hpp"
#include "NiPixelFormat.hpp"
#include "NiRect.hpp"

NiSmartPointer(Ni2DBuffer);

class Ni2DBuffer : public NiObject {
public:
	Ni2DBuffer();
	virtual ~Ni2DBuffer();

	enum CopyFilterPreference {
		COPY_FILTER_NONE = 0x0,
		COPY_FILTER_POINT = 0x1,
		COPY_FILTER_LINEAR = 0x2,
		COPY_FILTER_MAX = 0x3,
	};

	enum MultiSamplePreference {
		MULTISAMPLE_NONE = 0x0,
		MULTISAMPLE_2 = 0x1,
		MULTISAMPLE_4 = 0x2,
		MULTISAMPLE_8 = 0x3,
	};

	virtual void FastCopy(Ni2DBuffer* pkDest, const NiRect<UInt32>* pkSrcRect, UInt32 uiDestX, UInt32 uiDestY);
	virtual void Copy(Ni2DBuffer* pkDest, const NiRect<UInt32>* pkSrcRect, const NiRect<UInt32>* pkDestRect, Ni2DBuffer::CopyFilterPreference ePref);
	virtual void CreateRendererData();

	class RendererData : public NiRefObject {
	public:
		RendererData();
		virtual ~RendererData();

		Ni2DBuffer*				m_pkBuffer;
		const NiPixelFormat*	m_pkPixelFormat;
		SInt16					m_eMSAAPref;
	};

	typedef NiPointer<RendererData> RendererDataPtr;

	UInt32				m_uiWidth;
	UInt32				m_uiHeight;
	RendererDataPtr		m_spRendererData;

	UInt32 GetWidth() const { return m_uiWidth; }
	UInt32 GetHeight() const { return m_uiHeight; }
};

ASSERT_SIZE(Ni2DBuffer, 0x014);