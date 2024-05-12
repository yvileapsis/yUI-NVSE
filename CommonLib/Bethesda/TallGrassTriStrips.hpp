#pragma once

#include "NiTriStrips.hpp"

class TallGrassTriStrips : public NiTriStrips {
public:
	virtual void		RenderGrass(NiDX9Renderer* apRenderer);
	virtual void		RenderGrassAlt(NiDX9Renderer* apRenderer);
	virtual void		SetBounds(NiBound aBound);
	virtual void		SetCount(UInt16 ausCount);
	virtual NiBound*	GetLocalBound();

	UInt16 usInstanceCount;
	NiBound LocalBound;

	static TallGrassTriStrips* Create(NiGeometryData* apModelData);

	static void __fastcall RenderGrassEx(TallGrassTriStrips* apThis, void*, NiDX9Renderer* apRenderer);
	static void __fastcall RenderGrassAltEx(TallGrassTriStrips* apThis, void*, NiDX9Renderer* apRenderer);
};

ASSERT_SIZE(TallGrassTriStrips, 0xD8);