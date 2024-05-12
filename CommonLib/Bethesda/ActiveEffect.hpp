#pragma once
#include "BSSimpleList.hpp"

class ActiveEffect;
class BSSoundHandle;
class MagicItem;
class MagicCaster;
class MagicTarget;
class EffectItem;

// 0x48
class ActiveEffect
{
public:

	enum EnumSpellItemTypes
	{
		kSpellType_ActorEffect	= 0x0,
		kSpellType_Disease		= 0x1,
		kSpellType_Power		= 0x2,
		kSpellType_LesserPower	= 0x3,
		kSpellType_Ability		= 0x4,
		kSpellType_Poison		= 0x5,
		kSpellType_6			= 0x6,
		kSpellType_Addiction	= 0xA,
	};

	ActiveEffect();	// args are caster, magicItem, effectItem
	virtual ~ActiveEffect();

	virtual ActiveEffect*	Clone();
	virtual void			Unk_02(UInt32 arg);
	virtual void			Unk_03(UInt32 arg); // nullsub
	virtual void			SaveGame(UInt32 arg);
	virtual void			LoadGame(UInt32 arg);
	virtual void			Unk_06(UInt32 arg);
	virtual void			Unk_07(UInt32 arg);
	virtual void			Unk_08(UInt32 arg);
	virtual bool			UnregisterCaster(MagicCaster* apkCaster);	// returns 1 and clears caster if it matches the parameter, else returns 0
	virtual bool			Unk_0A();
	virtual void			Unk_0B(ActiveEffect* apkTarget);
	virtual bool			Unk_0C(UInt32 arg);
	virtual bool			Unk_0D(UInt32 arg);
	virtual void			Unk_0E(UInt32 arg);		// update/add effect?
	virtual void			Terminate();	// update/add effect?
	virtual void			Unk_10(UInt32 arg);
	virtual void			CopyTo(ActiveEffect* apkTo);
	virtual void			Unk_12();
	virtual void			Unk_13();

	Float32				fTimeElapsed;		// 04
	MagicItem*			pkMagicItem;		// 08
	EffectItem*			pkEffectItem;			// 0C
	bool				bApplied;			// 10
	bool				bTerminated;		// 11 set to 1 when effect is to be removed
	UInt8				ucFlags12;			// 12
	UInt8				pad13;				// 13
	UInt32				unk14;				// 14
	UInt32				unk18;				// 18 - flags
	Float32				fMagnitude;			// 1C - adjusted based on target?
	Float32				fDuration;			// 20 - adjusted based on target?
	MagicTarget*		pkTarget;			// 24
	MagicCaster*		pkCaster;			// 28
	EnumSpellItemTypes	eSpellType;			// 2C e.g. SpellItem::kType_Ability
	BSSoundHandle*		pkSound;			// 30 Sound* in stewie's tweaks
	UInt32				unk34;				// 34
	UInt32				unk38;				// 38
	TESForm*			pkEnchantObject;	// 3C enchanted obj responsible for effect
	BSSimpleList<TESForm*>	kData;		// 40 - in ScriptEffect this is a Script *

	void Remove(bool abRemoveImmediately);
};
static_assert(sizeof(ActiveEffect) == 0x048);