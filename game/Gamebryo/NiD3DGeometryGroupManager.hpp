#pragma once

#include "NiGeometryGroupManager.hpp"
#include "NiUnsharedGeometryGroup.hpp"
#include "NiTSet.hpp"
#include "NiDX9VertexBufferManager.hpp"

class NiD3DGeometryGroupManager : public NiGeometryGroupManager {
public:
	NiTSet<NiGeometryGroup>		m_kGroups;
	LPDIRECT3DDEVICE9			m_pkD3DDevice;
	NiDX9VertexBufferManager*	m_pkVBManager;
	bool						m_bHWSkinning;

	static bool AddObjectToGroup(NiGeometryGroup* pkGroup, NiGeometryData* pkData, NiSkinInstance* pkSkinInstance, bool bHWSkinning, UInt32 uiBonesPerPartition, UInt32 uiBonesPerVertex);
};