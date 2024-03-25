#pragma once

#include "NiNode.hpp"
#include "BSSimpleList.hpp"

class TESObjectREFR;

NiSmartPointer(BSFadeNode);

enum ENUM_LOD_MULT : UInt32 {
	LOD_MULT_NONE = 0x0,
	LOD_MULT_OBJECTS = 0x1,
	LOD_MULT_ITEMS = 0x2,
	LOD_MULT_ACTORS = 0x3,
	LOD_MULT_TREES = 0x4,
	LOD_MULT_LANDSCAPE = 0x5,
	LOD_MULT_DISTANTLOD = 0x6,
	LOD_MULT_ACTOR_BODY_PART = 0x7,
	LOD_MULT_RENDEREDMENU = 0x8,
	LOD_MULT_INVISIBLE = 0x9,
	LOD_MULT_SKY_CELL = 0xA,
	LOD_MULT_TOTAL = 0xB,
};

class BSFadeNode : public NiNode {
public:
	BSFadeNode();
	~BSFadeNode();

	float				fNearDistSqr;
	float				fFarDistSqr;
	float				fLastFade;
	float				fCurrentFade;
	float				fBoundRadius;
	float				fTimeSinceUpdate;
	ENUM_LOD_MULT		eMultType;
	UInt32				uiFrameCounter;
	TESObjectREFR*		pLinkedObj;
	UInt32				unkD0;
	BSSimpleList<void*> unkD4;
	BSSimpleList<void*> unkDC;

	CREATE_OBJECT(BSFadeNode, 0xB4EAA0);

	static void SetFadeEnabled(bool bEnabled) { *(bool*)0x11AD7B4 = bEnabled; }
	static bool IsFadeEnabled() { return *(bool*)0x11AD7B4; }
	static void SetFadeOutMultiplier(ENUM_LOD_MULT aeType, float afMult);
	static float GetFadeOutMultiplier(ENUM_LOD_MULT aeType);

	static void __fastcall UpdateSelectedDownwardPassEx(BSFadeNode* apThis, void*, const NiUpdateData& arData, UInt32 auiFlags);
};

ASSERT_SIZE(BSFadeNode, 0xE4);