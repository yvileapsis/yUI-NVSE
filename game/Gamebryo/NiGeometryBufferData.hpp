#pragma once

#include "NiVBChip.hpp"

class NiGeometryGroup;

class NiGeometryBufferData {
public:
	NiGeometryBufferData();

	virtual				~NiGeometryBufferData();
	virtual void		DeleteThis();

	UInt32						m_uiFlags;
	NiGeometryGroup*			m_pkGeometryGroup;
	UInt32						m_uiFVF;
	void*						m_hDeclaration;
	bool						m_bSoftwareVP;
	UInt32						m_uiVertCount;
	UInt32						m_uiMaxVertCount;
	UInt32						m_uiStreamCount;
	UInt32*						m_puiVertexStride;
	NiVBChip**					m_ppkVBChip;
	UInt32						m_uiIndexCount;
	UInt32						m_uiIBSize;
	LPDIRECT3DINDEXBUFFER9		m_pkIB;
	UInt32						m_uiBaseVertexIndex;
	D3DPRIMITIVETYPE			m_eType;
	UInt32						m_uiTriCount;
	UInt32						m_uiMaxTriCount;
	UInt32						m_uiNumArrays;
	const UInt16*				m_pusArrayLengths;
	const UInt16*				m_pusIndexArray;

	bool IsVBChipValid() const;
	NiVBChip* GetVBChip(UInt32 auiIndex) const;
	void SetStreamCount(UInt32 auiStreamCount);
	void RemoveIB();

	UInt32 GetNumArrays() const { return m_uiNumArrays; }
	const UInt16* GetIndexArray() const { return m_pusIndexArray; }
	const UInt16* GetArrayLengths() const { return m_pusArrayLengths; }
	const UInt32  GetVertexStride(UInt32 uiStream) const { return uiStream < m_uiStreamCount ? m_puiVertexStride[uiStream] : 0; }

	static bool __fastcall IsVBChipValidEx(NiGeometryBufferData* apThis);
};

ASSERT_SIZE(NiGeometryBufferData, 0x54);