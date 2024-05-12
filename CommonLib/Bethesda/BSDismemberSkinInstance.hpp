#pragma once

#include "NiSkinInstance.hpp"

class DismemberPartition;

NiSmartPointer(BSDismemberSkinInstance);

class BSDismemberSkinInstance : public NiSkinInstance {
public:
	BSDismemberSkinInstance();
	~BSDismemberSkinInstance();

	UInt32				uiPartitionNumber;
	DismemberPartition* pPartitions;
	bool				bIsRenderable;  //In Load this is made in OR with every partition->Enabled flag

	CREATE_OBJECT(BSDismemberSkinInstance, 0xA6C8A0);
	NIRTTI_ADDRESS(0x11F49D8);
};

ASSERT_SIZE(BSDismemberSkinInstance, 0x40);