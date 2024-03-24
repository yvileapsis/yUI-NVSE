#pragma once

#include "NiRefObject.hpp"
#include "NiTStringMap.hpp"
#include "NiDX9Resource.hpp"

class NiDX9IndexBufferManager : public NiRefObject {
public:
	NiDX9IndexBufferManager();
	virtual ~NiDX9IndexBufferManager();

	class NiDX9IBResource : public NiDX9Resource {
	public:
		UInt32					m_uiLength;
		D3DFORMAT				m_eFormat;
		UInt32					m_uiRevID;
		LPDIRECT3DINDEXBUFFER9	m_pkIB;
		D3DINDEXBUFFER_DESC		m_kIBDesc;
	};
	class NiDX9IBInfo
	{
	public:
		UInt32				m_uiIBID;
		UInt8				m_ucIndex;
		NiDX9IBResource*	m_apkIBRes[5];
	};

	LPDIRECT3DDEVICE9						m_pkD3DDevice9;
	LPDIRECT3DINDEXBUFFER9					m_pkTempPointConnIB;
	UInt32									m_uiTempPointConnLen;
	LPDIRECT3DINDEXBUFFER9					m_pkTempLineConnIB;
	UInt32									m_uiTempLineConnLen;
	NiTPointerMap<UInt32, NiDX9IBInfo*>		m_kDefaultResources;
	NiTPointerMap<UInt32, NiDX9IBInfo*>		m_kManagedResources;
	NiTPointerMap<UInt32, NiDX9IBInfo*>		m_kSystemMemResources;

	LPDIRECT3DINDEXBUFFER9 PackBuffer(const UInt16* pusTriList, UInt32 uiLength, UInt32 uiMaxLength, LPDIRECT3DINDEXBUFFER9 pkOldIB, UInt32& uiIBSize, D3DPOOL ePool, UInt32 uiUsage);
	LPDIRECT3DINDEXBUFFER9 CreateIndexBuffer(UInt32 uiLength, UInt32 uiUsage, D3DFORMAT eFormat, D3DPOOL ePool, D3DINDEXBUFFER_DESC* pkDesc = NULL);
};

ASSERT_SIZE(NiDX9IndexBufferManager, 0x4C);