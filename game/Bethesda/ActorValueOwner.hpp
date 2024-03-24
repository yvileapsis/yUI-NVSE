#pragma once

class Actor;

class ActorValueOwner {
public:
	ActorValueOwner();
	~ActorValueOwner();

	virtual UInt32	Fn_00(UInt32 actorValueCode);	// GetBaseActorValue (used from Eval) result in EAX
	virtual float	Fn_01(UInt32 actorValueCode);	// GetBaseActorValue internal, result in st
	virtual UInt32	Fn_02(UInt32 actorValueCode);	// GetActorValue internal, result in EAX
	virtual float	Fn_03(UInt32 actorValueCode);	// GetActorValue (used from Eval) result in EAX
	virtual float	Fn_04(UInt32 actorValueCode);	// GetBaseActorValue04 (internal) result in st
	virtual float	Fn_05(void* arg);
	virtual float	Fn_06(UInt32 actorValueCode);	// GetDamageActorValue or GetModifiedActorValue		called from Fn_08, result in st, added to Fn_01
	virtual UInt32	Fn_07(UInt32 actorValueCode);	// Manipulate GetPermanentActorValue, maybe convert to integer.
	virtual float	Fn_08(UInt32 actorValueCode);	// GetPermanentActorValue (used from Eval) result in EAX
	virtual Actor* Fn_09(void);					// GetActorBase (= this - 0x100) or GetActorBase (= this - 0x0A4)
	virtual UInt16	Fn_0A(void);					// GetLevel (from ActorBase)

	// SkillsCurrentValue[14] at index 20
};

ASSERT_SIZE(ActorValueOwner, 0x04);