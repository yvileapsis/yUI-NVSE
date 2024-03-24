#pragma once

#include "NiIntegerExtraData.hpp"

NiSmartPointer(BSDecalPlacementVectorExtraData);

class BSDecalPlacementVectorExtraData : public NiIntegerExtraData {
public:
	BSDecalPlacementVectorExtraData();
	~BSDecalPlacementVectorExtraData();

	UInt16 unk10;
	UInt32 unk14[8];
	UInt32 unk34;
	UInt32 unk38;
	UInt32 unk3C;

	CREATE_OBJECT(BSDecalPlacementVectorExtraData, 0xC64530)
};

ASSERT_SIZE(BSDecalPlacementVectorExtraData, 0x40)