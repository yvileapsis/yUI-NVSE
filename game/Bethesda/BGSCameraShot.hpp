#pragma once

#include "TESForm.hpp"
#include "TESModel.hpp"
#include "TESImageSpaceModifiableForm.hpp"
#include "NiNode.hpp"

class TESImageSpaceModifier;
class NiRefObject;

class BGSCameraShot : public TESForm, public TESModel, public TESImageSpaceModifiableForm {
	BGSCameraShot();
	~BGSCameraShot();

	enum Location;

	enum Action;

	enum Target : SInt32
	{
		kTargetAttacker = 0x0,
		kTargetProjectile = 0x1,
		kTargetTarget = 0x2,
	};

	enum Flags : SInt32
	{
		kPositionFollowsLocation = 0x1,
		kPositionFollowsTarget = 0x2,
		kDontFollowBone = 0x4,
		kFirstPersonCamera = 0x8,
		kNoTracer = 0x10,
		kStartAtTimeZero = 0x20,
	};

	Action			kAction;
	Location		kLocation;
	Target			kTarget;
	Flags			uiFlags;
	float			unk048;
	float			unk04C;
	float			fGlobalTimeMult;
	float			unk054;
	float			unk058;
	float			unk05C;
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

ASSERT_SIZE(BGSCameraShot, 0x78);