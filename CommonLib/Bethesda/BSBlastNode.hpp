#pragma once

#include "BSRangeNode.hpp"

NiSmartPointer(BSBlastNode);

class BSBlastNode : public BSRangeNode {
public:
	BSBlastNode();
	~BSBlastNode();

	CREATE_OBJECT(BSBlastNode, 0xC4AC00);
};

ASSERT_SIZE(BSBlastNode, 0xB0)