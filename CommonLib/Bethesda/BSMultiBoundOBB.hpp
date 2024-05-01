#pragma once

#include "BSMultiBoundAABB.hpp"
#include "NiMatrix3.hpp"
#include "NiFrustumPlanes.hpp"

NiSmartPointer(BSMultiBoundOBB);

class BSMultiBoundOBB : public BSMultiBoundAABB {
public:
	BSMultiBoundOBB();
	~BSMultiBoundOBB();

	NiMatrix3 Axis;

	CREATE_OBJECT(BSMultiBoundOBB, 0xC36500);

	void GetVertices(BoundVertices* apPoints);
	static void __fastcall GetVerticesEx(BSMultiBoundOBB* apThis, void*, BoundVertices* apPoints);
	static bool __fastcall CompletelyWithinFrustumEx(BSMultiBoundOBB* apThis, void*, NiFrustumPlanes* apPlanes);


};

ASSERT_SIZE(BSMultiBoundOBB, 0x48)