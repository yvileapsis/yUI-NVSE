#pragma once

#include "NiAVObject.hpp"
#include "NiSkinPartition.hpp"
#include "NiSkinData.hpp"

NiSmartPointer(NiSkinInstance);

class NiSkinInstance : public NiObject {
public:
	NiSkinInstance();
	virtual ~NiSkinInstance();

	NiSkinDataPtr m_spSkinData;
	NiSkinPartitionPtr m_spSkinPartition;
	NiAVObject* m_pkRootParent;
	NiAVObject** m_ppkBones;
	UInt32 m_uiFrameID;
	UInt32 m_uiNumMatrices;
	UInt32 m_uiNumRegisters;
	UInt32 m_uiAllocatedSize;
	void* m_pvBoneMatrices;
	void* unk2C;
	void* m_pvSkinToWorldWorldToSkinMatrix;

	CREATE_OBJECT(NiSkinInstance, 0xA866F0);

	bool IsPartitionEnabled(UInt32 partitionIndex);

	void UpdateModelBound(NiBound& arBound);

	__forceinline NiSkinPartition* GetSkinPartition() const { return m_spSkinPartition.m_pObject; };
	void SetSkinPartition(NiSkinPartition* pkSkinPartition) { m_spSkinPartition = pkSkinPartition; };

	__forceinline NiSkinData* GetSkinData() const { return m_spSkinData.m_pObject; };
	void SetSkinData(NiSkinData* pkSkinData) { m_spSkinData = pkSkinData; }
};

ASSERT_SIZE(NiSkinInstance, 0x34);