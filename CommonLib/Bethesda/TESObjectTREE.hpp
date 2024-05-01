#pragma once
#include "TESBoundObject.hpp"
#include "TESModelTree.hpp"
#include "TESIconTree.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "NiTArray.hpp"
#include "NiPoint2.hpp"

class TESObjectTREE :
	public TESBoundObject,
	public TESModelTree,
	public TESIconTree,
	public BGSDestructibleObjectForm
{
public:
	struct Data
	{
		Float32		fLeafCurvature;
		Float32		fLeafAngleMin;
		Float32		fLeafAngleMax;
		Float32		fBranchDimming;
		Float32		fLeafDimming;
		SInt32		iShadowRadius;
		Float32		fRockSpeed;
		Float32		fRustleSpeed;
	};

	NiTPrimitiveArray<UInt32>	kSpeedTreeSeeds;
	Data						kTreeData;
	NiPoint2					kBillboardDimensions;
};
static_assert(sizeof(TESObjectTREE) == 0x94);