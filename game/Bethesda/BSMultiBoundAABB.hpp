#pragma once

#include "BSMultiBoundShape.hpp"
#include "NiPoint3.hpp"
#include "NiFrustumPlanes.hpp"

NiSmartPointer(BSMultiBoundAABB);

class BSMultiBound;

class BSMultiBoundAABB : public BSMultiBoundShape {
public:
	BSMultiBoundAABB();
	virtual ~BSMultiBoundAABB();

	NiPoint3 Center;
	NiPoint3 HalfExtents;

	CREATE_OBJECT(BSMultiBoundAABB, 0xC381A0);

	void GetVertices(BoundVertices* apVerts);
};

ASSERT_SIZE(BSMultiBoundAABB, 0x24)