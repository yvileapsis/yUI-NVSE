#pragma once

#include "IOTask.hpp"
#include "NiNode.hpp"

class TESObjectREFR;

class AttachDistant3DTask : public IOTask {
public:
	TESObjectREFR*	pRef;
	NiNodePtr		spNode;
};

STATIC_ASSERT(sizeof(AttachDistant3DTask) == 0x20);