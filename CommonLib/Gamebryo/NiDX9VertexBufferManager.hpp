#pragma once

#include "d3d9.h"
#include "NiCriticalSection.hpp"
#include "NiDX9Resource.hpp"
#include "NiGeometryData.hpp"
#include "NiRefObject.hpp"
#include "NiSkinInstance.hpp"
#include "NiTPointerMap.hpp"

class NiVBChip;

class NiDX9VertexBufferManager : public NiRefObject {
public:
	NiDX9VertexBufferManager();
	virtual ~NiDX9VertexBufferManager();

	class NiDX9VBResource : public NiDX9Resource {
	public:
		UInt32					  m_uiLength;
		UInt32					  m_uiFVF;
		UInt32					  m_uiRevID;
		LPDIRECT3DVERTEXBUFFER9	 m_pkVB;
		D3DVERTEXBUFFER_DESC		m_kVBDesc;
	};
	class NiDX9VBInfo : public NiMemObject {
	public:
		UInt32			  m_uiVBID;
		UInt8			   m_ucIndex;
		NiDX9VBResource*	m_pkVBRes[5];
	};

	LPDIRECT3DDEVICE9						m_pkD3DDevice9;
	NiTPointerMap<UInt32, NiDX9VBInfo*>		m_kDefaultResources;
	NiTPointerMap<UInt32, NiDX9VBInfo*>		m_kManagedResources;
	NiTPointerMap<UInt32, NiDX9VBInfo*>		m_kSystemMemResources;
	UInt32									m_uiFrameID;
	void*									m_pvTempBuffer;
	UInt32									m_uiTempBufferSize;
	void*									m_pvLockedBuffer;
	UInt32									m_uiLockedBufferSize;
	DWORD									unk50[12];
	NiCriticalSection						m_kVBCriticalSection;
	DWORD									unkA0[24];

	static void* __fastcall LockVBEx(NiDX9VertexBufferManager* apThis, void*, LPDIRECT3DVERTEXBUFFER9 apVB, UInt32 auiOffset, UInt32 auiSize, UInt32 auiLockFlags);
	static bool __fastcall UnlockVBEx(NiDX9VertexBufferManager* apThis, void*, LPDIRECT3DVERTEXBUFFER9 apVB);

	NiVBChip* PackUntransformedVB(NiGeometryData* apData, NiSkinInstance* apSkin, UInt16 ausDirtyFlags, NiVBChip* apOldChip, void* apvLockedBuff);
};

ASSERT_SIZE(NiDX9VertexBufferManager, 0x100);