#pragma once

#include "bhkCharacterListener.hpp"
#include "bhkCharacterProxy.hpp"
#include "BSSimpleList.hpp"
#include "hkpCharacterContext.hpp"
#include "hkpCharacterProxyListener.hpp"
#include "hkpSurfaceInfo.hpp"
#include "hkStepInfo.hpp"

class Projectile;
class bhkCharControllerShape;
class bhkCharacterControllerCinfo;

NiSmartPointer(hkpRigidBody);
NiSmartPointer(bhkCapsuleShape);
NiSmartPointer(bhkCachingShapePhantom);
NiSmartPointer(bhkCharacterController);

ALIGN16 class bhkCharacterController :
	public bhkCharacterProxy,
	public hkpCharacterContext,
	public bhkCharacterListener,
	public hkpSurfaceInfo
{
public:
	enum CharacterState : UInt32 {
		ON_GROUND	= 0,
		JUMPING		= 1,
		IN_AIR		= 2,
		CLIMBING	= 3,
		FLYING		= 4,
		SWIMMING	= 5,
		PROJECTILE	= 6,
		COUNT		= 11,
	};

	enum CharacterSize : UInt32 {
		NORMAL	= 0,
		SIZED	= 1,
		SIZING	= 2,
	};

	enum Flags {
		DEBUG_GEOMETRY	= 0x8000,
		COMPLEX_SCENE	= 0x100000,
		UNK_200000		= 0x200000,
	};

	virtual void InitWithParams(bhkCharacterControllerCinfo* apInfo);
	virtual void Move();
	virtual void Unk_51();
	virtual void SetDebugDisplay(void* apUnk = nullptr);


	hkVector4					kUpVec;
	hkVector4					kForwardVec;
	hkStepInfo					kStepInfo;
	hkVector4					kOutVelocity;
	hkVector4					kVelocityMod;
	hkVector4					kDirection;
	CharacterState				eWantState;
	float						fVelocityTime;
	float						fRotMod;
	float						fRotModTime;
	float						fCalculatePitchTimer;
	float						fAcrobatics;
	float						fCenter;
	float						fWaterHeight;
	float						fJumpHeight;
	float						fFallStartHeight;
	float						fFallTime;
	float						fGravity;
	float						fPitchAngle;
	float						fRollAngle;
	float						fPitchMult;
	float						fScale;
	float						fSwimFloatHeight;
	float						fActorHeight;
	float						fSpeedPct;
	UInt32						unk56C;
	hkVector4					kRotCenter;
	hkVector4					kPushDelta;
	UInt32						iPushCount;
	bhkCachingShapePhantomPtr	spChrPhantom;
	UInt32						unk598;
	UInt32						eShapeType;
	UInt32						eSizedShapeType;
	bhkCapsuleShapePtr			spShapes[2];
	UInt32						unk5AC;
	UInt32						unk5B0;
	UInt32						unk5B4;
	UInt32						unk5B8;
	UInt32						unk5BC;
	UInt32						unk5C0;
	UInt32						unk5C4;
	UInt32						unk5C8;
	UInt32						unk5CC;
	UInt32						unk5D0;
	UInt32						unk5D4;
	UInt32						unk5D8;
	UInt32						unk5DC;
	UInt32						unk5E0;
	UInt32						unk5E4;
	UInt32						unk5E8;
	UInt32						unk5EC;
	float						fRadius;
	float						fHeight;
	float						fDestRadius;
	float						fDistFromPlayersEyes;
	CharacterSize				eSize;
	UInt32						iPriority;
	UInt32						unk608;
	hkpRigidBodyPtr				spSupportBody;
	bool						bFakeSupport;
	UInt32						unk614;
	UInt32						unk618;
	UInt32						unk61C;
	hkVector4					kFakeSupportStart;
	UInt32						spBumpedBody;
	float						fBumpedForce;
	UInt32						spBumpedCharCollisionObject;
	hkArray<hkpCdPoint>			arr63C;
	BSSimpleList<UInt32>		damageImpacts;
	Projectile*					pProjectile; // Projectile

	NiAVObject* GetNiObject() const { return ThisCall<NiAVObject*>(0x810660, const_cast<bhkCharacterController*>(this)); };

	NiNode* GetCollisionDebugNode();
	void RemoveCollisionDebug();
};

ASSERT_SIZE(bhkCharacterController, 0x660);