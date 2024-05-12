#pragma once

#include "NiIntegersExtraData.hpp"

NiSmartPointer(BSWArray);

class BSWArray : public NiIntegersExtraData {
public:
	BSWArray();
	~BSWArray();

	CREATE_OBJECT(BSWArray, 0xC61510);
};

ASSERT_SIZE(BSWArray, 0x14)