#pragma once

#include "NiExtraData.hpp"
#include "BSFaceGenModel.hpp"

class BSFaceGenModelExtraData : public NiExtraData {
public:
	BSFaceGenModelPtr spModel;

	NIRTTI_ADDRESS(0x65BEEC);
};

ASSERT_SIZE(BSFaceGenModelExtraData, 0x10);