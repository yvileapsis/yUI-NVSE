#pragma once

#include "NiNode.hpp"
class BSNiNode : public NiNode {
public:
	virtual ~BSNiNode();
	virtual void ReparentSkinInstances(NiNode* apNode, NiAVObject* apParent);
};