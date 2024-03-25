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
static_assert(sizeof(ActorPackage) == 0x18);