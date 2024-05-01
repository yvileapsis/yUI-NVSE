#pragma once

#include "NiExtraData.hpp"

NiSmartPointer(NiVectorExtraData);

class NiVectorExtraData : public NiExtraData {
public:
	NiVectorExtraData();
	~NiVectorExtraData();

	float m_afVector[4];

	CREATE_OBJECT(NiVectorExtraData, 0xA91FE0)
};

ASSERT_SIZE(NiVectorExtraData, 0x1C)