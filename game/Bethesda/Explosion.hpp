#pragma once

#include "MobileObject.hpp"

class NiPointLight;
class NonActorMagicCaster;
class BSSoundHandle;

class Explosion : public MobileObject {
public:
  float						unk088;
  float						unk08C;
  float						fInnerRadius;
  float						fOuterRadius;
  float						fImageSpaceRadius;
  float						fDamageMult;
  NiRefObject*				object0A0;
  BSSimpleList<void*>		kTargetList;
  BSSoundHandle*			pSoundHandles[6];
  NiPointer<NiPointLight>	spLight;
  TESObjectREFR*			pOwnerRef;
  int						gapCC;
  NiPointer<NiObject>		spObject0D0;
  UInt32					unk0D4;
  bool						gapD8;
  bool						gapD9;
  NonActorMagicCaster*		pCaster;
  int						gapE0;
  int						gapE4;
  NiPoint3					kClosestPoint;
  NiPoint3					kClosestPointNormal;
  float						fCalculatedDamage;

  static NiTPointerList<TESObjectREFR*>* GetWaterReflectedRefs();
};

ASSERT_SIZE(Explosion, 0x104);