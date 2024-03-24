#pragma once

#include "NiPSysModifier.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(BSParentVelocityModifier);

class BSParentVelocityModifier : public NiPSysModifier {
public:
	BSParentVelocityModifier();
	~BSParentVelocityModifier();

	float float18;
	UInt32 unk1C;
	float fDamping;
	NiPoint3 nipoint324;
	NiPoint3 nipoint330;

	CREATE_OBJECT(BSParentVelocityModifier, 0xC63880)
};

ASSERT_SIZE(BSParentVelocityModifier, 0x3C)