#pragma once

#include "NiFloatInterpController.hpp"

class BSFrustumFOVController : public NiFloatInterpController {
public:
	BSFrustumFOVController();
	virtual ~BSFrustumFOVController();

	CREATE_OBJECT(BSFrustumFOVController, 0xA42EE0);
	NIRTTI_ADDRESS(0x11F3EC8);
};