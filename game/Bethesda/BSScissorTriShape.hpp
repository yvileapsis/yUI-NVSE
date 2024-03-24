#pragma once

#include "NiTriShape.hpp"

class BSScissorTriShape : public NiTriShape {
public:
	RECT rectangle;


	static BSScissorTriShape* CreateObject();
	static void __fastcall RenderImmediateEx(BSScissorTriShape* apThis, void*, NiDX9Renderer* apRenderer);
	static void __fastcall RenderImmediateAltEx(BSScissorTriShape* apThis, void*, NiDX9Renderer* apRenderer);
};

ASSERT_SIZE(BSScissorTriShape, 0xD4);