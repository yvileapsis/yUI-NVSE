#pragma once

#include "BSNiNode.hpp"

NiSmartPointer(BSRangeNode);

class BSRangeNode : public BSNiNode {
public:
	BSRangeNode();
	~BSRangeNode();

	bool bEnabled;
	char cCurrent;
	char cMin;
	char cMax;

	CREATE_OBJECT(BSRangeNode, 0xC45440);
};

ASSERT_SIZE(BSRangeNode, 0xB0)