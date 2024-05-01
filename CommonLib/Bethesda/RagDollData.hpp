#pragma once

#include "NiPoint3.hpp"

struct RagDollBone {
	UInt8		ucBoneID;
	NiPoint3	kBonePos;
	NiPoint3	kBoneRot;
};

struct RagDollData {
	UInt8			ucBoneCount;
	RagDollBone*	pBoneArray;
	NiPoint3		kBip01Rot;
};

ASSERT_SIZE(RagDollBone, 0x1C);
ASSERT_SIZE(RagDollData, 0x14);
