#pragma once
#include "TESForm.hpp"
#include "TESModel.hpp"
#include "TESImageSpaceModifiableForm.hpp"
#include "NiNode.hpp"

class TESImageSpaceModifier;
class NiRefObject;

// 0x78
class BGSCameraShot :
	public TESForm,
	public TESModel,
	public TESImageSpaceModifiableForm
{
	BGSCameraShot();
	~BGSCameraShot();

	enum Location;

	enum Action;

	enum EnumTarget : SInt32
	{
		kTargetAttacker = 0x0,
		kTargetProjectile = 0x1,
		kTargetTarget = 0x2,
	};

	enum EnumFlags : SInt32
	{
		kPositionFollowsLocation = 0x1,
		kPositionFollowsTarget = 0x2,
		kDontFollowBone = 0x4,
		kFirstPersonCamera = 0x8,
		kNoTracer = 0x10,
		kStartAtTimeZero = 0x20,
	};


	Action			eAction;
	Location		eLocation;
	EnumTarget		eTarget;
	EnumFlags		eFlags;
	Float32			unk048;
	Float32			unk04C;
	Float32			fGlobalTimeMult;
	Float32			unk054;
	Float32			unk058;
	Float32			unk05C;
	NiAVObjectPtr	spLocationNode;
	NiAVObjectPtr	spTargetNode;
	TESObjectREFR*	pReference;
	NiNodePtr		spRootNode;
	NiAVObjectPtr	spImagespaceTarget;
	UInt8			byte074;
	UInt8			byte075;
	UInt8			byte076;
	UInt8			gap077;
};
static_assert(sizeof(BGSCameraShot) == 0x78);