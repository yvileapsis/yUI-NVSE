#pragma once

#include "Defines.hpp"

class NiVBDynamicSet {
public:
	UInt32 m_uiIndex;
	UInt32 m_uiFVF;
	UInt32 m_uiBlockSize;
	D3DDevicePtr m_pkD3DDevice;
	D3DVertexBufferPtr m_pkVB;
	UInt32 m_uiCurrOffset;
};