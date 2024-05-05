#pragma once

#include "NiCollisionObject.hpp"
#include "NiTransform.hpp"

class bhkWorldObject;

NiSmartPointer(bhkNiCollisionObject);

class bhkNiCollisionObject : public NiCollisionObject {
public:
	bhkNiCollisionObject();
	virtual ~bhkNiCollisionObject();

	virtual NiPoint3*   GetVelocity(NiPoint3* outVel);
	virtual void		hktoNiTransform();
	virtual void		NitohkTransform();
	virtual void		ClearVelocities();
	virtual void		SetMotionType(UInt32 aeType, bhkRigidBody* apBody, bool abUpdateMotion);
	virtual bool		GetKeyFrame();
	virtual void		SetTransform(NiTransform& akTransform);
	virtual bool		GetIncludedInSkeleton();
	virtual void		SetDebugDisplay(bool abToggle);

	enum Flags {
		ACTIVE		  = 1 << 0, // 0x1
		
		NOTIFY		  = 1 << 2, // 0x4
		SET_LOCAL	   = 1 << 3, // 0x8
		DEBUG_DISPLAY   = 1 << 4, // 0x10
		USE_VELOCITY	= 1 << 5, // 0x20
		RESET		   = 1 << 6, // 0x40
		SYNC_ON_UPDATE  = 1 << 7, // 0x80
		UNK_100		 = 1 << 8, // 0x100 BhkBlendCollisionObject related
		UNK_200		 = 1 << 9, // 0x200
		ANIM_TARGETED   = 1 << 10, // 0x400
		DISMEMBER_LIMB  = 1 << 11, // 0x800
	};

	Bitfield16				  usFlags;
	NiPointer<bhkWorldObject>   spWorldObject;
};

ASSERT_SIZE(bhkNiCollisionObject, 0x14);