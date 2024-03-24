#pragma once

#include "NiDX9OnscreenBufferData.hpp"

class NiDX9ImplicitBufferData : public NiDX9OnscreenBufferData {
public:
	NiDX9ImplicitBufferData();
	virtual ~NiDX9ImplicitBufferData();

	LPDIRECT3DDEVICE9 m_pkD3DDevice9;

	static bool __fastcall DisplayFrameEx(NiDX9ImplicitBufferData* apThis);
};
ASSERT_SIZE(NiDX9ImplicitBufferData, 0x5C)