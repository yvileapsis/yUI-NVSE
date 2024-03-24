#pragma once

#include "BSMemObject.hpp"
#include "NiPoint3.hpp"

class BSAudioListener : public BSMemObject {
public:
	BSAudioListener();
	virtual				~BSAudioListener();
	virtual void		Unk_01();
	virtual void		SetPosition(NiPoint3& arPos);
	virtual NiPoint3*	GetPosition();
	virtual void		UpdatePositionAndOrientation();
	virtual void		SetFrontAndTopOrientation(NiPoint3& arTop, NiPoint3& arFront);
	virtual UInt32		GetUnk10();
	virtual UInt32		GetUnk1C();
	virtual void		SetVelocity(NiPoint3& arVelocity);
	virtual void		SetFlt040(float);
	virtual void		SetEnvironmentType(UInt32 aeType);
	virtual UInt32		GetEnvironmentType();
	virtual void		SetRolloffFactor(float afValue);

	NiPoint3	kOriginWorldPos;
	UInt32		unk10;
	UInt32		unk14;
	UInt32		unk18;
	UInt32		unk1C;
	UInt32		unk20;
	UInt32		unk24;
	NiPoint3	kVelocity;
	UInt32		sysTime;
	float		flt38;
	float		fRollOffFactor;
	float		flt40;
	UInt32		uiEnvironmentType;
};

ASSERT_SIZE(BSAudioListener, 0x48)