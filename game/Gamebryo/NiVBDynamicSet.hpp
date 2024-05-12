#pragma once

class NiVBDynamicSet {
public:
	UInt32 m_uiIndex;
	UInt32 m_uiFVF;
	UInt32 m_uiBlockSize;
	LPDIRECT3DDEVICE9 m_pkD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pkVB;
	UInt32 m_uiCurrOffset;
};