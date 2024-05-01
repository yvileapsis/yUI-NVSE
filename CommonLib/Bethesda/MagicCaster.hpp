#pragma once

class MagicItem;
class MagicItemForm;
class MagicTarget;
class SpellItem;
class TESForm;
class ActiveEffect;
class EffectItem;

// 0xC
class MagicCaster {
public:
	MagicCaster();
	~MagicCaster();

	enum EnumState
	{
		kCastState_Inactive = 0,
		kCastState_Aim = 1,
		kCastState_Cast = 2,
		kCastState_ReleaseCast = 3,
		kCastState_FindTargets = 4,
		kCastState_SpellDisabled = 5,
		kCastState_AlreadyCasting = 6,
		kCastState_CannotCast = 7
	};

	virtual void			AddAbility(SpellItem* splItem, bool arg2);												// 000
	virtual void			AddAddiction(SpellItem* splItem, bool arg2);											// 004
	virtual void			AddEffect(SpellItem* splItem, bool arg2);												// 008
	virtual void			CastSpell(MagicItem* spell, bool arg2, MagicTarget* target, float arg4, bool arg5);		// 00C
	virtual void			AddDisease(SpellItem* splItem, MagicTarget* target, bool arg3);							// 010
	virtual void			AddFormEffects(MagicItem* magItem, MagicItemForm* itemForm, bool arg3);					// 014
	virtual MagicTarget*	PickMagicTarget();																		/*018*/
	virtual void			Unk_07();																				/*01C*/
	virtual void			Unk_08();																				/*020*/
	virtual void			NullSub(UInt32 arg1, UInt32 arg2);														/*024*/
	virtual bool			Unk_0A(UInt32 arg1, float* arg2, UInt32* arg3, UInt32 arg4);							/*028*/
	virtual Actor*			GetActor();																				/*02C*/
	virtual NiNode*			GetMagicNode();																			/*030*/
	virtual MagicItem*		GetMagicItem();																			/*034*/
	virtual bool			ApplyEffectToTarget(ActiveEffect* activeEffect);										/*038*/
	virtual float			Unk_0F(UInt8 arg1, float arg2);															/*03C*/
	virtual void			SetMagicItem(MagicItem* spell);															/*040*/
	virtual MagicTarget*	GetMagicTarget();																		/*044*/
	virtual void			SetMagicTarget(MagicTarget* magicTarget);												/*048*/
	virtual ActiveEffect*	CreateActiveEffect(MagicItem* magicItem, EffectItem* effItem, TESForm* itemForm);		// 04C

	void*		unk004;	// 004
	EnumState	eState;	// 008
};
static_assert(sizeof(MagicCaster) == 0xC);