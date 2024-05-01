#pragma once

#include "NiObject.hpp"

NiSmartPointer(BSReference);

class BSReference : public NiObject {
public:
	BSReference();
	~BSReference();

	DWORD unk8;

	CREATE_OBJECT(BSReference, 0xC64810)
};

ASSERT_SIZE(BSReference, 0xC)