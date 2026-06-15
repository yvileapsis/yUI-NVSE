#pragma once

class Actor;
class TESForm;
class TESModel;
class TESObjectARMO;
class TESObjectWEAP;
class TESRace;
class NiNode;
class NiAVObject;

struct BipedAnim {
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
		TESModel*	pPart;
		NiAVObject* pPartClone;
		bool		bSkinned;
	};

	NiNode*			pRoot;
	OptionalBone	kBones[5];
	VB01Data		kObjects[20];
	VB01Data		kBufferedObjects[20];
	float			fWeaponOffset;
	TESObjectREFR*	pRequester;

	MEMBER_FN_PREFIX(BipedAnim);
#if RUNTIME
	DEFINE_MEMBER_FN(Destroy, bool, 0x00418E00, BipedAnim*, bool);	// Destoy and optionally free the structure/class
#endif
};