#pragma once

#include "Defines.hpp"
#include "NiTArray.hpp"

class NiVBBlock;

class NiVBSet {
public:
	UInt32 m_uiIndex;
	UInt32 m_uiFVF;
	UInt32 m_uiFlags;
	UInt32 m_uiBlockSize;
	D3DDevicePtr m_pkD3DDevice;
	UInt32 m_uiUsage;
	D3DPOOL m_ePool;
	NiTPrimitiveArray<NiVBBlock*> m_apkBlocks;
	NiVBBlock* m_pkBlockList;
	UInt32 m_uiFreeSpaceThreshold;
};