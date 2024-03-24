#pragma once

#include "Actor.hpp"
#include "BipedAnim.hpp"

class TESObjectARMO;

class Character : public Actor {
public:
	Character();
	virtual ~Character();

	virtual void		Unk_137(void);
	virtual void		Unk_138(void);

	BipedAnim*	pBipedAnim;
	float		fTotalArmorDR;
	float		fTotalArmorDT;
	UInt8		bIsTrespassing;
	UInt8		bIsGuard;
	UInt16		unk1C2;
	float		fFlyInventoryWeight;
};

ASSERT_SIZE(Character, 0x1C8);