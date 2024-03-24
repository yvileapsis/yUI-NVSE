#pragma once

#include "BSSimpleArray.hpp"
#include "NiNode.hpp"
#include "NiTriShape.hpp"

class BGSAutoWater {
public:
	BSSimpleArray<NiPoint3>*	pPointArray;
	BSSimpleArray<UInt16>*		pShortArray;
	BSSimpleArray<UInt16>*		pShortArray1;
	NiPoint3*					pVertex;
	UInt32						uiTriangleList;
	NiPoint3*					pNormal;
	NiPoint2*					pTexture;
	NiColorA*					pColor;
	DWORD						dword20;
	UInt32						uiVertCount;
	UInt32						uiTriangleCount;
	NiNodePtr					spRootNode;
	NiTriShapePtr				spShape;
	NiPoint3					kPoint;
};

ASSERT_SIZE(BGSAutoWater, 0x40);