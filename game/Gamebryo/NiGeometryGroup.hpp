#pragma once

#include <d3d9.h>
#include "NiSkinPartition.hpp"

class NiGeometryBufferData;
class NiScreenTexture;
class NiSkinInstance;
class NiGeometryData;
class NiVBChip;

NiSmartPointer(NiGeometryGroup);

class NiGeometryGroup {
public:
	NiGeometryGroup();

	virtual void		Purge();
	virtual void		AddObject(NiScreenTexture* pkScreenTexture);
	virtual void		AddObject0(NiGeometryData* pkData, NiSkinInstance* pkSkinInstance, NiSkinPartition::Partition* pkPartition);
	virtual void		RemoveObject(NiScreenTexture* pkScreenTexture);
	virtual void		RemoveObject0(NiSkinPartition::Partition* pkPartition);
	virtual void		RemoveObject1(NiGeometryData* pkData);
	virtual NiVBChip*	CreateChip(NiGeometryBufferData* pkBuffData, UInt32 uiStream);
	virtual void		ReleaseChip(NiGeometryBufferData* pkBuffData, UInt32 uiStream);
	virtual bool		IsDynamic();
	virtual 			~NiGeometryGroup();

	UInt32				m_uiRefCount;
	LPDIRECT3DDEVICE9	m_pkD3DDevice;
};

ASSERT_SIZE(NiGeometryGroup, 0xC);