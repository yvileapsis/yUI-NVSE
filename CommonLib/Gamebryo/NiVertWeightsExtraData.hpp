#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiVertWeightsExtraData);

class NiVertWeightsExtraData : public NiExtraData {
public:
	NiVertWeightsExtraData();
	~NiVertWeightsExtraData();

	CREATE_OBJECT(NiVertWeightsExtraData, 0xA89B80)
};

ASSERT_SIZE(NiVertWeightsExtraData, 0xC)