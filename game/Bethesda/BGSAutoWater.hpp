#pragma once
#include "BSSimpleArray.hpp"
#include "NiColor.hpp"
#include "NiNode.hpp"
#include "NiPoint2.hpp"
#include "NiPoint3.hpp"
#include "NiTriShape.hpp"

// 0x40
class BGSAutoWater
{
public:
	BSSimpleArray<NiPoint3>*	pkPointArray;
	BSSimpleArray<UInt16>*		pkShortArray;
	BSSimpleArray<UInt16>*		pkShortArray1;
	NiPoint3			*		pkVertex;
	UInt32						uiTriangleList;
	NiPoint3*					pkNormal;
	NiPoint2*					pkTexture;
	NiColorA*					pkColor;
	DWORD						dword20;
	UInt32						uiVertCount;
	UInt32						uiTriangleCount;
	NiPointer<NiNode>			spRootNode;
	NiPointer<NiTriShape>		spShape;
	NiPoint3					kPoint;
};
static_assert(sizeof(BGSAutoWater) == 0x40);