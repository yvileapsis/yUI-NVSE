#pragma once

#include "BSMultiBoundShape.hpp"
#include "NiSphere.hpp"

NiSmartPointer(BSMultiBoundSphere);

class BSMultiBoundSphere : public BSMultiBoundShape {
public:
	BSMultiBoundSphere();
	~BSMultiBoundSphere();

	NiSphere Sphere;

	CREATE_OBJECT(BSMultiBoundSphere, 0xC4FDB0)
};

ASSERT_SIZE(BSMultiBoundSphere, 0x1C)