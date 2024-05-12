#pragma once

class Actor;
class TESForm;
class TESModelTextureSwap;
class TESObjectARMO;
class TESObjectWEAP;
class TESRace;
class NiNode;

struct BipedAnim {	// somehow descend from NiNodeArray
	enum eOptionalBoneType {
		kOptionalBone_Bip01Head = 0,
		kOptionalBone_Weapon = 1,
		kOptionalBone_Bip01LForeTwist = 2,
		kOptionalBone_Bip01Spine2 = 3,
		kOptionalBone_Bip01Neck1 = 4,
	};

	// 008
	struct OptionalBone
	{
		bool	exists;
		NiNode* bone;
	};

	// 010
	struct VB01Data
	{
		union
		{
			TESForm* item;
			TESObjectARMO* armor;
			TESObjectWEAP* weapon;
			TESRace* race;
		};
		TESModelTextureSwap* modelTexture;
		NiNode* boneNode;
		UInt32 unk00C;
	};

	NiNode* pRoot;
	OptionalBone bones[5];
	VB01Data slotData[20];
	VB01Data unk016C[20];
	UInt32 unk2AC;
	Actor* actor;

	MEMBER_FN_PREFIX(BipedAnim);
#if RUNTIME
	DEFINE_MEMBER_FN(Destroy, bool, 0x00418E00, BipedAnim*, bool);	// Destoy and optionally free the structure/class
#endif
};