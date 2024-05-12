#pragma once
#include "IOTask.hpp"
#include "NiNode.hpp"

class TESObjectREFR;

// 0x20
class AttachDistant3DTask : public IOTask {
public:
	TESObjectREFR*	pRef;
	NiNodePtr		spNode;
};
static_assert(sizeof(AttachDistant3DTask) == 0x20);