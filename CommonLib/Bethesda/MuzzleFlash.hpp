#pragma once

#include "NiSmartPointer.hpp"

class NiNode;
class NiPointLight;
class BGSProjectile;
class Actor;
class TESObjectWEAP;
class TESObjectREFR;

struct MuzzleFlash {
	MuzzleFlash(TESObjectREFR* apReference, Actor* apSourceActor);

	bool					bEnabled;
	bool					bMPSEnabled;
	bool					bUpdateLight;
	float					fEnableTimer;
	float					fDurationTimer;
	NiPointer<NiNode>		spNode;
	NiPointer<NiPointLight> spLight;
	BGSProjectile*			pProjectile;
	TESObjectWEAP*			pSourceWeapon;
	Actor*					pSourceActor;

	void Enable();
};

ASSERT_SIZE(MuzzleFlash, 0x20);