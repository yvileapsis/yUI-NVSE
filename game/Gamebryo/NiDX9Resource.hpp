#pragma once

#include <d3d9.h>

class NiDX9Resource {
public:
	enum Type {
		TYPE_TEXTURE = 0x0,
		TYPE_TEXTURE_CUBE = 0x1,
		TYPE_TEXTURE_VOLUME = 0x2,
		TYPE_VERTEXBUFFER = 0x3,
		TYPE_INDEXBUFFER = 0x4,
		TYPE_INVALID = 0xFFFF,
		TYPE_COUNT = 0x10000,
	};

	UInt32					m_uiResID;
	Type					m_eType;
	D3DPOOL					m_ePool;
	UInt32					m_uiUsage;
	LPDIRECT3DRESOURCE9		m_pkD3DResource;
};