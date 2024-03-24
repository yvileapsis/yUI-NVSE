#pragma once

#include "NiTriBasedGeom.hpp"
#include "NiTriStripsData.hpp"

NiSmartPointer(NiTriStrips);

class NiTriStrips : public NiTriBasedGeom {
public:
	NiTriStrips();
	virtual ~NiTriStrips();

	CREATE_OBJECT(NiTriStrips, 0xA71CE0);

	NiTriStripsData* GetModelData();

	static void __fastcall RenderImmediateEx(NiTriStrips* apThis, void*, NiDX9Renderer* apRenderer);
	static void __fastcall RenderImmediateAltEx(NiTriStrips* apThis, void*, NiDX9Renderer* apRenderer);
};

ASSERT_SIZE(NiTriStrips, 0xC4)