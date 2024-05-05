#pragma once

#include "bhkEntity.hpp"
#include "BSSimpleList.hpp"
#include "hkAabb.hpp"
#include "hkQuaternion.hpp"
#include "hkTransform.hpp"

class NiAVObject;

class bhkRigidBody : public bhkEntity
{
public:
	virtual hkVector4*	  GetPosition(hkVector4& arOut);
	virtual hkQuaternion*   GetRotation(hkQuaternion& arOut);
	virtual void			SetPosition(hkVector4& arPos);
	virtual void			SetRotation(hkQuaternion& arRot);
	virtual bool			SetMotionType(UInt32 motionType);
	virtual void			SetPosRot(hkVector4& arPos, hkQuaternion& arRot);
	virtual hkVector4*	  GetCenterOfMassLocal(hkVector4& arOut);
	virtual hkVector4*	  GetCenterOfMassInWorld(hkVector4& arOut);
	virtual hkTransform*	GetTransform(hkTransform* arOut);
	virtual void			GetAabb(hkAabb& arOut);
	virtual void			SetHavokAssertState(bool abEnabled);

	BSSimpleList<bhkRefObjectPtr> kActionConstraints;

	CREATE_OBJECT(bhkRigidBody, 0xC8ED70);
	NIRTTI_ADDRESS(0x12681C0);

	static bhkRigidBody* FindHeaviestBody(NiAVObject* apObject);

	void Fix();
	void UnFix(bool abAddRemainingConstraints);

	static void ToggleConstraints(NiAVObject* apObject, bool abFix, bool abAddRemainingConstraints);
};

ASSERT_SIZE(bhkRigidBody, 0x1C);