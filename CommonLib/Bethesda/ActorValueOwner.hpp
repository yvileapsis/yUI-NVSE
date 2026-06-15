#pragma once

class TESForm;

class ActorValueOwner {
public:
	ActorValueOwner();
	~ActorValueOwner();

	virtual int32_t		GetBaseActorValueI(uint32_t aeIndex);
	virtual float		GetBaseActorValueF(uint32_t aeIndex);
	virtual int32_t		GetActorValueI(uint32_t aeIndex);
	virtual float		GetActorValueF(uint32_t aeIndex);
	virtual float		GetTemporaryModifier(uint32_t aeIndex);
	virtual float		GetDamageModifier(uint32_t aeIndex);
	virtual float		GetPermanentModifier(uint32_t aeIndex);
	virtual int32_t		GetPermanentActorValueI(uint32_t aeIndex);
	virtual float		GetPermanentActorValueF(uint32_t aeIndex);
	virtual TESForm*	GetAsForm() const;
	virtual uint16_t	GetActorLevel() const;
};
static_assert(sizeof(ActorValueOwner) == 0x4);