#pragma once

#include "NiTriShape.hpp"

class TallGrassTriShape : public NiTriShape {
public:
	virtual void		RenderGrass(NiDX9Renderer* apRenderer);
	virtual void		RenderGrassAlt(NiDX9Renderer* apRenderer);
	virtual void		SetBounds(NiBound aBound);
	virtual void		SetCount(UInt16 ausCount);
	virtual NiBound*	GetLocalBound();

	UInt16 usInstanceCount;
	NiBound LocalBound;

	static TallGrassTriShape* Create(NiGeometryData* apModelData);

	static void __fastcall RenderGrassEx(TallGrassTriShape* apThis, void*, NiDX9Renderer* apRenderer);
	static void __fastcall RenderGrassAltEx(TallGrassTriShape* apThis, void*, NiDX9Renderer* apRenderer);
};

ASSERT_SIZE(TallGrassTriShape, 0xD8);