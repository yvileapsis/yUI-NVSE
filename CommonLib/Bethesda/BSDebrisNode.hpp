#pragma once

#include "BSRangeNode.hpp"

NiSmartPointer(BSDebrisNode);

class BSDebrisNode : public BSRangeNode {
public:
	BSDebrisNode();
	~BSDebrisNode();

	CREATE_OBJECT(BSDebrisNode, 0xC4AE60)
};

ASSERT_SIZE(BSDebrisNode, 0xB0);