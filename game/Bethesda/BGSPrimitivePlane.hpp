#pragma once

#include "BGSPrimitive.hpp"

class BGSPrimitivePlane : public BGSPrimitive {
public:
	virtual bool CreateOcclusionPlane(NiPoint3*);
	virtual BSMultiBoundShape* CreatePortal(NiPoint3*);

	DWORD dword4;
	NiRefObject* pnirefobject24;
	NiRefObject* pnirefobject2C;

	static BGSPrimitivePlane* __cdecl CreateObject(NiPoint2* apRadius, NiColorA* apColor);
	static void __fastcall BuildEx(BGSPrimitivePlane* apThis);
};
