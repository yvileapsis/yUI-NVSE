#pragma once

class Actor;

class ActorValueOwner {
public:
	ActorValueOwner();
	~ActorValueOwner();

	// GetBaseActorValue (used from Eval) result in EAX
	virtual SInt32	GetBaseActorValueInt(UInt32 avCode);
	// GetBaseActorValue internal, result in st
	virtual Float32	GetBaseActorValue(UInt32 avCode);
	// GetActorValue internal, result in EAX
	virtual SInt32	GetActorValueInternal(UInt32 avCode);
	// GetActorValue (used from Eval) result in EAX
	virtual Float32	GetActorValue(UInt32 avCode);
	// GetBaseActorValue04 (internal) result in st
	virtual Float32	GetBaseSubAV(UInt32 avCode);
	// GetDamageActorValue or GetModifiedActorValue
	// called from Fn_08, result in st, added to Fn_01
	virtual Float32	GetSubAVDamage(UInt32 avCode);
	virtual Float32	GetSubAVMod(UInt32 avCode);
	// Manipulate GetPermanentActorValue, maybe convert to integer.
	virtual SInt32	GetPermanentActorValueInt(UInt32 avCode);
	// GetPermanentActorValue (used from Eval) result in EAX
	virtual Float32	GetPermanentActorValue(UInt32 avCode);
	// GetActorBase (= this - 0x100) or GetActorBase (= this - 0x0A4)
	virtual Actor*	GetActor();
	// GetLevel (from ActorBase)
	virtual UInt16	GetLevel();

	// SkillsCurrentValue[14] at index 20
};
static_assert(sizeof(ActorValueOwner) == 0x4);