#pragma once

class TESPackage;
class TESPackageData;
class TESObjectREFR;

struct ActorPackage {
	TESPackage*		pPackage;
	TESPackageData* pData;
	TESObjectREFR*	pTarget;
	SInt32			iCurrentProcedureIndex;
	float			fPackageStartTime;
	Bitfield32		uiFlags;
};

ASSERT_SIZE(ActorPackage, 0x18);