#pragma once

#include <d3d9.h>
#include "NiVBDynamicSet.hpp"
#include "NiVBBlock.hpp"

class NiVBChip {
public:
	UInt32 m_uiIndex;
	union
	{
		NiVBBlock* m_pkBlock;
		NiVBDynamicSet* m_pkDynamicSet;
	};
	LPDIRECT3DVERTEXBUFFER9 m_pkVB;
	UInt32 m_uiOffset;
	UInt32 m_uiLockFlags;
	UInt32 m_uiSize;
	NiVBChip* m_pkNextChip;
	NiVBChip* m_pkPrevChip;
};

ASSERT_SIZE(NiVBChip, 0x020);