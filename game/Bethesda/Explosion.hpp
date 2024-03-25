#pragma once
#include "MobileObject.hpp"
#include "NiTPointerList.hpp"

class NiPointLight;
class NonActorMagicCaster;
class BSSoundHandle;

class Explosion : public MobileObject
{
public:
	Explosion();
	~Explosion();

	Float32					unk088;
	Float32					unk08C;		// init to 3.0
	Float32					fInnerRadius;
	Float32					fOuterRadius;
	Float32					fImageSpaceRadius;
	Float32					fDamageMult;
	NiRefObject*			object0A0;
	BSSimpleList<UInt32>	kTargetList;
	BSSoundHandle*			pSoundHandles[6];
	NiPointLight*			spLight;
	TESObjectREFR*			pOwnerRef;
	NiRefObject*			object0CC;
	NiRefObject*			spObject0D0;
	UInt32					unk0D4;
	UInt8					dontApplyExplosionIMOD;
	UInt8					isUnderwater;
	UInt8					byte0DA;
	UInt8					byte0DB;
	NonActorMagicCaster*	pkCaster;		
	TESObjectWEAP*			pkWeapon;				// 0E0
	UInt32					unk0E4;					// 0E4
	NiPoint3				kClosestPoint;			// 0E8
	NiPoint3				kClosestPointNormal;	// 0F4
	Float32					fCalculatedDamage;

  static NiTPointerList<TESObjectREFR*>* GetWaterReflectedRefs();
};
static_assert(sizeof(Explosion) == 0x104);