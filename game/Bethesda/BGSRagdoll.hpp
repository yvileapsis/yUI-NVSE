#pragma once

#include "TESForm.hpp"
#include "TESModel.hpp"
#include "BSSimpleArray.hpp"

class BGSBodyPartData;

struct FootIK_DATA {
	struct FootIK_Leg_DATA {
		SInt16	short0;
		SInt16	short2;
		SInt16	short4;
		SInt8	char6;
		float	float8;
		float	floatC;
		float	float10;
		float	float14;
		float	float18;
		float	float1C;
		UInt32	unk20[6];
	};
};

struct LookIK_DATA {
	struct LookIK_Head_DATA {
		SInt16	short0;
		SInt16	short2;
		UInt32	unk[6];
		float	float1C;
		float	float20;
		float	float24;
	};

};

struct GrabIK_DATA {
	struct GrabIK_Arm_DATA {
		SInt16 unk0;
		SInt16 unk2;
		SInt16 unk4;
	};
};

class BGSRagdoll : public TESForm, public TESModel {
public:
	struct Feedback {
		struct FeedbackData {
			float fBlendAmount;
			float fHierarchyGain;
			float fPositionGain;
			float fVelocityGain;
			float fAccelerationGain;
			float fSnapGain;
			float fVelocityDampening;
			float float1C;
			float float20;
			float float24;
			float float28;
			float float2C;
			float float30;
			DWORD dword34;
			DWORD dword38;
		};

		bool					bEnabled;
		FeedbackData			kFeedbackData;
		BSSimpleArray<UInt16>	kFeedbackDynamicBones;
	};


	struct FootIK {
		struct FootIKData {
			float float0;
			float float4;
			float float8;
			float floatC;
			float float10;
			float float14;
			float float18;
		};

		bool											bEnabled;
		FootIKData										kData;
		BSSimpleArray<const char*>						kStringArray;
		BSSimpleArray<FootIK_DATA::FootIK_Leg_DATA>		kDataArray;
	};

	struct LookIK {
		bool											bEnabled;
		BSSimpleArray<const char*>						kStringArray;
		BSSimpleArray<LookIK_DATA::LookIK_Head_DATA>	kDataArray;
	};


	struct GrabIK {
		bool										bEnabled;
		BSSimpleArray<const char*>					kStringArray;
		BSSimpleArray<GrabIK_DATA::GrabIK_Arm_DATA>	kDataArray;
	};

	struct PoseMatching {
		struct PoseMatchingData {
			UInt16	kBones[3];
			UInt8	ucFlags;
			float	fMotorsStrength;
			float	fPoseActivationDelayTime;
			float	fMatchErrorAllowance;
			float	fDisplacementToDisable;
		};

		bool				bEnabled;
		BSStringT			strDeathPose;
		PoseMatchingData	kData;
	};



	UInt32				unk30;
	UInt32				unk34;
	UInt32				unk38;
	UInt32				unk3C;
	Feedback			kFeedback;
	FootIK				kFootIK;
	LookIK				kLookIK;
	GrabIK				kGrabIK;
	PoseMatching		kPoseMatching;
	UInt32				unk13C;
	BGSBodyPartData*	pBodyPartData;
	TESObjectREFR*		pActor;
};

ASSERT_SIZE(BGSRagdoll, 0x148);