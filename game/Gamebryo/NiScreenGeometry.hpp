#pragma once

#include "NiTriShape.hpp"

NiSmartPointer(NiScreenGeometry);

class NiScreenGeometry : public NiTriShape {
public:
	NiScreenGeometry();
	virtual ~NiScreenGeometry();

	CREATE_OBJECT(NiScreenGeometry, 0xA8FF80)
};

ASSERT_SIZE(NiScreenGeometry, 0xC4)