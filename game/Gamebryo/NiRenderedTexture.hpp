#pragma once

#include "NiTexture.hpp"
#include <d3d9types.h>
#include "Ni2DBuffer.hpp"

class NiDX9Renderer;

NiSmartPointer(NiRenderedTexture);

class NiRenderedTexture : public NiTexture {
public:
	NiRenderedTexture();
	virtual ~NiRenderedTexture();

	virtual Ni2DBuffer* GetBuffer();

	Ni2DBufferPtr						m_spBuffer;
	unsigned int						uiBackgroundColor;
	int									unk38;
	Ni2DBuffer::MultiSamplePreference	eMSAAPref;
	bool								bIsRenderTarget;
	bool								bUseCustomFormat;
	D3DFORMAT							eCustomFormat;

	static NiRenderedTexture* CreateObject();

	static NiRenderedTexture* Create(Ni2DBuffer* pkBuffer, NiDX9Renderer* pkRenderer);
};

ASSERT_SIZE(NiRenderedTexture, 0x48);