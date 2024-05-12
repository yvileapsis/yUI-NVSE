#pragma once

#include "BSNiNode.hpp"

NiSmartPointer(BSValueNode);

class BSValueNode : public BSNiNode {
public:
	BSValueNode();
	~BSValueNode();

	Bitfield8	ucFlags;
	SInt32		iValue;
	NiObjectPtr spReservedObject;

	CREATE_OBJECT(BSValueNode, 0xC43A30);
	NIRTTI_ADDRESS(0x1202DE8);
};

ASSERT_SIZE(BSValueNode, 0xB8)