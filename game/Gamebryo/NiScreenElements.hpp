#pragma once

#include "NiTriShape.hpp"

NiSmartPointer(NiScreenElements);

class NiScreenElements : public NiTriShape {
public:
	NiScreenElements();
	virtual ~NiScreenElements();

	CREATE_OBJECT(NiScreenElements, 0xA877A0)
};

ASSERT_SIZE(NiScreenElements, 0xC4)