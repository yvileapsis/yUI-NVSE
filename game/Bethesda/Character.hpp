#pragma once
#include "Actor.hpp"
#include "BipedAnim.hpp"

class TESObjectARMO;

class Character : public Actor {
public:
	Character();
	~Character() override;

	virtual void		Unk_137();
	virtual void		Unk_138();

	BipedAnim*	pkBipedAnim;
	Float32		fTotalArmorDR;
	Float32		fTotalArmorDT;
	UInt8		bIsTrespassing;
	UInt8		bIsGuard;
	UInt16		unk1C2;
	Float32		fFlyInventoryWeight;
};
static_assert(sizeof(Character) == 0x1C8);