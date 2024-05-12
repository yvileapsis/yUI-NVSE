#pragma once

#include "TESBoundObject.hpp"
#include "TESModelTree.hpp"
#include "TESIconTree.hpp"
#include "BGSDestructibleObjectForm.hpp"
#include "NiTArray.hpp"
#include "NiPoint2.hpp"

class TESObjectTREE : public TESBoundObject {
public:
	struct Data
	{
		float fLeafCurvature;
		float fLeafAngleMin;
		float fLeafAngleMax;
		float fBranchDimming;
		float fLeafDimming;
		SInt32 iShadowRadius;
		float fRockSpeed;
		float fRustleSpeed;
	};

	TESModelTree modelTree;
	TESIconTree iconTree;
	BGSDestructibleObjectForm destructibleObjectForm;
	NiTPrimitiveArray<UInt32> speedTreeSeeds;
	TESObjectTREE::Data treeData;
	NiPoint2 billboardDimensions;
};

ASSERT_SIZE(TESObjectTREE, 0x94);