#pragma once

#include "hkaRaycastInterface.hpp"
#include "BSSimpleArray.hpp"
#include "NiFixedString.hpp"
#include "NiAVObject.hpp"
#include "BSPrecisionTimer.hpp"
#include "NiPoint4.hpp"
#include "hkMatrix3.hpp"
#include "hkArray.hpp"
#include "hkQuaternion.hpp"

class hkaRagdollInstance;
class bhkPoseArray;
class hkpWorldObject;
class hkaFootPlacementIkSolver;
class hkpCachingShapePhantom;
class hkaSkeleton;
class hkaPose;

NiSmartPointer(bhkRagdollShareData);

ALIGN16 class bhkRagdollController : public hkaRaycastInterface {
public:
	struct GrabIKData {
		UInt32	unk00;
		float	fGain;
		float	float4;
		bool	bStart;
		bool	bStop;
	};

	struct FootIKData {
		hkaFootPlacementIkSolver* pFootPlacementSolver;
		float fUnk4;
		DWORD unk8;
		DWORD unkC;
		__m128 unk10[3];
		__m128* unk40;
		DWORD unk44;
		DWORD unk48;
		DWORD unk4C;
		DWORD unk50;
		DWORD unk54;
		DWORD unk58;
		DWORD unk5C;
		DWORD unk60;
		DWORD unk64;
		DWORD unk68;
		DWORD unk6C;
		DWORD unk70;
		DWORD unk74;
		DWORD unk78;
		DWORD unk7C;
		DWORD unk80;
		DWORD unk84;
		DWORD unk88;
		DWORD unk8C;
		DWORD unk90;
		DWORD unk94;
		DWORD unk98;
		DWORD unk9C;
		DWORD unkA0;
		DWORD unkA4;
		DWORD unkA8;
		DWORD unkAC;
		DWORD unkB0;
		DWORD unkB4;
		DWORD unkB8;
		DWORD unkBC;
		UInt16 usBone;
		DWORD unkC4;
		DWORD unkC8;
		DWORD unkCC;
	};

	struct RagdollAnimData {
		struct RagdollParams {
			float fHierarchyGain;
			float fVelocityDamping;
			float fAccelerationGain;
			float fVelocityGain;
			float fPositionGain;
			float fPositionMaxLinearVelocity;
			float fPositionMaxAngularVelocity;
			float fSnapGain;
			float fSnapMaxLinearVelocity;
			float fSnapMaxAngularVelocity;
			float fSnapMaxLinearDistance;
			float fSnapMaxAngularDistance;
		};


		hkArray<RagdollParams*> kRagdollParams;
		hkArray<UInt32>			kArray_0C;
		UInt32					uiSomeSize_18;
		UInt32					unk01C;
		UInt32					unk020;
		UInt32					unk024;
		UInt32					unk028;
		hkArray<UInt32>			kArray_2C;
		UInt32					unk038;
		hkArray<UInt32>			kArray_3C;
	};

	struct BoneTransf {
		NiNode*		pBone;
		NiTransform kTransform;
	};

	bool						unk004;
	float						unk008;
	UInt32						unk00C;
	hkMatrix3					matrix010;
	UInt8						byte040;
	UInt8						bGrabRelated_0x41;
	bool						bUseIKSystem;
	bool						bGrabIK_0x43;
	bool						unk044;
	hkaRagdollInstance*			pRagdollInstance;
	RagdollAnimData*			pRagdollAnimData;
	UInt8						byte050;
	UInt8						gap051;
	UInt8						gap052;
	UInt8						gap053;
	void*						pUnk054;
	NiNode*						pBip01Node;
	UInt32						unk05C;
	NiMatrix3					kMatrix060;
	NiPointer<NiNode>			spDebugLinesNode;
	hkaPose*					pPose_0x88;
	UInt8						unk08C[4];
	hkpCachingShapePhantom*		phkobject090;
	hkArray<hkMatrix3>			kMatrices;
	BSSimpleArray<NiAVObject*>	kBones;
	bool						bLookIKSetUp;
	bool						bLookIKEnabled;
	bool						bNoHeadTrack;
	bool						bLookIK_B3;
	bool						unk0B4;
	UInt32						unk0B8;
	UInt32						unk0BC;
	hkQuaternion				kQuaternion_0C0;
	hkQuaternion				kQuaternion_0xD0;
	hkQuaternion				kQuaternion_0xE0;
	UInt32						unk0F0;
	UInt16						unk0F4;
	UInt16						unk0F6;
	UInt32						unk0F8;
	UInt32						unk0FC;
	hkQuaternion 				kQuaternion_0x100;
	hkQuaternion				kQuaternion_0x110;
	hkQuaternion				kQuaternion_0x120;
	bool						unk130;
	UInt32						unk134;
	UInt32						unk138;
	UInt32						unk13C;
	UInt32						unk140;
	UInt16						unk144;
	UInt32						unk148;
	UInt32						unk14C;
	UInt32						unk150;
	UInt32						unk154;
	UInt32						unk158;
	UInt32						unk15C;
	UInt32						unk160;
	UInt32						unk164;
	UInt32						unk168;
	UInt32						unk16C;
	hkQuaternion				kQuaternion_0x170;
	UInt8						byte180;
	UInt8						byte181;
	UInt32						unk184;
	UInt32						unk188;
	UInt32						unk18C;
	bool						byte190;
	UInt8						byte191;
	UInt8						byte192;
	UInt8						byte193;
	UInt32						unk194;
	bool						unk198;
	float						unk19C;
	float						unk1A0;
	UInt8						byte1A4[4];
	float						unk1A8;
	UInt32						unk1AC;
	GrabIKData					kGrabIK_0x1B0[2];
	UInt32						GrabIK_Data_1D0;
	UInt32						unk1D4;
	UInt32						unk1D8;
	UInt32						unk1DC;
	UInt32						unk1E0;
	UInt32						unk1E4;
	UInt32						unk1E8;
	UInt8						unk1EC[4];
	UInt32						unk1F0;
	UInt32						unk1F4;
	UInt32						unk1F8;
	UInt32						unk1FC;
	UInt32						unk200;
	UInt32						unk204;
	UInt32						unk208;
	UInt32						unk20C;
	UInt16						usLeftUpperArmIdx;
	UInt16						usLeftForeArmIdx;
	UInt16						usLeftHandIdx;
	UInt16						usRightUpperArmIdx;
	UInt16						usRightForeArmIdx;
	UInt16						usRightHandIdx;
	bool						bGrabIKSetUp;
	bool						bGrabIKEnabled;
	bool						bGrabIK_0x21E;
	bool						bFootIKSetUp;
	bool						bFootIKEnabled;
	bool						byte221;
	bool						bFootIK_0x222;
	bool						bFootIK_0x223;
	bool						byte224_falseIfLibertyPrime;
	UInt32						unk228;
	hkArray<FootIKData>			hkArray_22C;
	UInt16						unk238;
	UInt16						unk23A;
	float						unk23C;
	bool						byte240_trueIfLibertyPrime;
	UInt8						byte241;
	UInt8						byte242;
	UInt8						gap243;
	float						fUnk244_FootIK;
	UInt32						unk248;
	UInt32						deathPSAName;
	NiPointer<bhkPoseArray>		spPoseArray;
	UInt8						byte254;
	UInt32						unk258;
	UInt32						unk25C;
	BSPrecisionTimer			timer;
	UInt32						unk270;
	UInt32						unk274;
	UInt32						unk278;
	float						unk27C;
	float						unk280;
	bool						unk284;
	UInt32						unk288;
	bool						bFeedbackSet;
	bool						bFeedbackEnabled;
	UInt8						byte28E;
	UInt8						byte28F;
	float						unk290;
	UInt32						unk294;
	UInt32						unk298;
	UInt32						unk29C;
	NiFixedString				strBlockName;
	bhkRagdollShareDataPtr		spRagdollShareData;
	BSPrecisionTimer			kCounter2A8;
	UInt32						uiHeadBoneIndex; // for m_boneToRigidBodyMap in hkaRagdollInstance
	UInt32						unk2BC;

	void CreateDebugNodes();
	void CreateBoneLines();
};

ASSERT_OFFSET(bhkRagdollController, unk130, 0x130);
ASSERT_SIZE(bhkRagdollController, 0x2C0);
ASSERT_SIZE(bhkRagdollController::BoneTransf, 0x38);