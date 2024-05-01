#pragma once
#include "BGSPrimitive.hpp"

// 0x34
class BGSPrimitivePlane : public BGSPrimitive {
public:
	virtual bool CreateOcclusionPlane(NiPoint3*);
	virtual BSMultiBoundShape* CreatePortal(NiPoint3*);

	static BGSPrimitivePlane* __cdecl CreateObject(NiPoint2* apRadius, NiColorA* apColor);
	static void __fastcall BuildEx(BGSPrimitivePlane* apThis);
};
static_assert(sizeof(BGSPrimitivePlane) == 0x34);