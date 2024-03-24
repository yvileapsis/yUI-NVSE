#pragma once

#include "NiTArray.hpp"
#include "NiVBSet.hpp"

class NiVBChip;

class NiVBBlock {
public:
	NiVBSet*						m_pkSet;
	UInt32							m_uiIndex;
	LPDIRECT3DVERTEXBUFFER9			m_pkVB;
	UInt32							m_uiSize;
	UInt32							m_uiFVF;
	UInt32							m_uiFlags;
	UInt32							m_uiUsage;
	D3DPOOL							m_ePool;
	UInt32							m_uiCurrOffset;
	UInt32							m_uiFreeBytes;
	UInt32							m_uiTotalFreeBytes;
	NiTPrimitiveArray<NiVBChip*>	m_apkChips;
	NiVBBlock*						m_pkNextBlock;
	NiVBBlock*						m_pkPrevBlock;
};