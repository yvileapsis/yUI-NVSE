#pragma once

#include <hkUnaryActionCinfo.hpp>
#include <hkVector4.hpp>

ALIGN16 class hkOrientHingedBodyActionCinfo : public hkUnaryActionCinfo {
public:
	hkVector4	HingeAxisLS;
	hkVector4	ForwardLS;
	float		fStrength;
	float		fDamping;
};

ASSERT_SIZE(hkOrientHingedBodyActionCinfo, 0x40);