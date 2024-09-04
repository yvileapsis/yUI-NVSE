#pragma once

#include "hkArray.hpp"

class NiPoint3;
class NiAVObject;
class NiNode;
class NiRTTI;

class bhkNiCollisionObject;
class bhkCollisionObject;
class bhkPCollisionObject;
class bhkConstraint;

class hkSimplePropertyValue;
class hkReferencedObject;
class hkpWorldObject;
class hkpCollidable;
class hkaSkeleton;

class Actor;
class TESObjectREFR;

namespace bhkUtilFunctions {

	typedef int(__cdecl* BoneCompareFunc)(const char*, const char*);

	void					AddAngularVelocity(NiAVObject* apObject, NiPoint3& arVelocity, bool abReset);
	void					AddVelocity(NiAVObject* apObject, NiPoint3& arVelocity, bool abReset);
	void					FindBonesAndConstraints(const NiAVObject* apObject, hkArray<hkReferencedObject*>& arBones, hkArray<bhkConstraint*>& arConstraints);
	bhkNiCollisionObject*	FindCollisionObject(const NiAVObject* apObject);
	NiAVObject*				GetAVObjectFromCollidable(const hkpCollidable* apCollidable);
	TESObjectREFR*			GetReferenceFromCollidable(const hkpCollidable* apCollidable);
	NiPoint3*				GetActorVelocity(NiPoint3& arResult, const Actor* apActor);
	UInt16					GetBoneFromName(const hkaSkeleton* apSkeleton, const char* apName, BoneCompareFunc apFunc = _stricmp);

	bhkNiCollisionObject*	GetbhkNiCollisionObject(const hkpWorldObject* apObject);
	bhkNiCollisionObject*	GetbhkNiCollisionObject(const NiAVObject* apObject);
	bhkCollisionObject*		GetbhkCollisionObject(const hkpWorldObject* apObject);
	bhkCollisionObject*		GetbhkCollisionObject(const NiAVObject* apObject);

	double					GetHeirGain(const NiNode* apNode, const char* apBoneName = nullptr);
	hkSimplePropertyValue*	GetPropertyFromRTTI(hkSimplePropertyValue& arResult, const hkpWorldObject* apObject, const NiRTTI* apRTTI);
	UInt32					GetTriangleCountOfShape(void* apShape);
	bhkPCollisionObject*	IsbhkPCollisionObject(const hkpWorldObject* apObject);
	void					PerformSync(NiAVObject* apObject, UInt32 aeControllerUpdate);
	void					RemoveHavokFromSceneGraph(NiAVObject* apObject);
	void					SetBoneNamesRecurse(NiAVObject* apObject);
}