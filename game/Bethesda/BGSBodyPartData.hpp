#pragma once

#include "TESForm.hpp"
#include "TESModel.hpp"
#include "BGSPreloadable.hpp"

class BGSBodyPart;
class BGSRagdoll;

class BGSBodyPartData : public TESForm {
public:
	BGSBodyPartData();
	~BGSBodyPartData();

	enum
	{
		eBodyPart_Torso = 0,
		eBodyPart_Head1,
		eBodyPart_Head2,
		eBodyPart_LeftArm1,
		eBodyPart_LeftArm2,
		eBodyPart_RightArm1,
		eBodyPart_RightArm2,
		eBodyPart_LeftLeg1,
		eBodyPart_LeftLeg2,
		eBodyPart_LeftLeg3,
		eBodyPart_RightLeg1,
		eBodyPart_RightLeg2,
		eBodyPart_RightLeg3,
		eBodyPart_Brain,
		eBodyPart_Weapon,
	};

	TESModel		model;				// 018
	BGSPreloadable	preloadable;		// 030
	BGSBodyPart* bodyParts[15];		// 034
	BGSRagdoll* ragDoll;			// 070
};

ASSERT_SIZE(BGSBodyPartData, 0x74);