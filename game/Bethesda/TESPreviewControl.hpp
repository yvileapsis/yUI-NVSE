#pragma once

#include "TESRenderControl.hpp"
#include "BSString.hpp"

class ShadowSceneNode;

class TESPreviewControl : public TESRenderControl {
public:
	BYTE						byte6C;
	DWORD						dword70;
	float						float74;
	BSStringT					strWindowName;
	BYTE						byte80;
	NiNodePtr					spNode84;
	NiPointer<ShadowSceneNode>	spShadowSceneNode;
};

ASSERT_SIZE(TESPreviewControl, 0x8C)