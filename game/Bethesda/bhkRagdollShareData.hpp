#pragma once

#include "NiObject.hpp"
#include "hkaSkeleton.hpp"

class hkaSkeletonMapper;
class BGSRagdoll;

class bhkRagdollShareData : public NiObject {
public:
	hkaSkeleton*		pRagdollSkeleton;
	hkaSkeleton*		pFinalSkeleton;
	hkaSkeletonMapper*	pSkeletonMapper10;
	hkaSkeletonMapper*	pSkeletonMapper14;
	struct Feedback*	pFeedback;
	hkaSkeleton*		pSkeleton1C;
};

ASSERT_SIZE(bhkRagdollShareData, 0x20);