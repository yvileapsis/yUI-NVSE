#pragma once

#include "NiNode.hpp"

class BSTempNode : public NiNode {
public:
	BSTempNode();
	virtual ~BSTempNode();

	float fTime;

	NIRTTI_ADDRESS(0x1202E00);

	static BSTempNode* Create(float afTime);
};

ASSERT_SIZE(BSTempNode, 0xB0);