#pragma once

#include "BSMultiBoundShape.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(BSMultiBoundCapsule);

class BSMultiBoundCapsule : public BSMultiBoundShape {
public:
	BSMultiBoundCapsule();
	~BSMultiBoundCapsule();

	NiPoint3 PointA;
	NiPoint3 PointB;
	float fRadius;

	CREATE_OBJECT(BSMultiBoundCapsule, 0xC610A0)
};

ASSERT_SIZE(BSMultiBoundCapsule, 0x28)