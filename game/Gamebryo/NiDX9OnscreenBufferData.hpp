#pragma once

#include "NiDX92DBufferData.hpp"

class NiDX9OnscreenBufferData : public NiDX92DBufferData {
public:
	NiDX9OnscreenBufferData();
	virtual ~NiDX9OnscreenBufferData();

	D3DPRESENT_PARAMETERS m_kD3DPresentParams;
};
ASSERT_SIZE(NiDX9OnscreenBufferData, 0x58)