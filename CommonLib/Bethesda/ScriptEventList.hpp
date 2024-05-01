#pragma once
#include "ScriptLocal.hpp"
#include "BSSimpleList.hpp"

class Script;
class TESForm;

// 0x14
struct ScriptEventList 
{
	enum
	{
		kEvent_OnAdd					= 1 << 0,
		kEvent_OnEquip					= 1 << 1,		// Called on Item and on Refr
		kEvent_OnActorEquip				= kEvent_OnEquip,	// presumably the game checks the type of the object
		kEvent_OnDrop					= 1 << 2,
		kEvent_OnUnequip				= 1 << 3,
		kEvent_OnActorUnequip			= kEvent_OnUnequip,
		kEvent_OnDeath					= 1 << 4,
		kEvent_OnMurder					= 1 << 5,
		kEvent_OnCombatEnd				= 1 << 6,		// See 0x008A083C
		kEvent_OnHit					= 1 << 7,		// See 0x0089AB12
		kEvent_OnHitWith				= 1 << 8,		// TESObjectWEAP*	0x0089AB2F
		kEvent_OnPackageStart			= 1 << 9,
		kEvent_OnPackageDone			= 1 << 10,
		kEvent_OnPackageChange			= 1 << 11,
		kEvent_OnLoad					= 1 << 12,
		kEvent_OnMagicEffectHit			= 1 << 13,		// EffectSetting* 0x0082326F
		kEvent_OnSell					= 1 << 14,		// 0x0072FE29 and 0x0072FF05, linked to 'Barter Amount Traded' Misc Stat
		kEvent_OnStartCombat			= 1 << 15,

		kEvent_OnOpen					= 1 << 16,		// while opening some container, not all
		kEvent_OnClose					= 1 << 17,		// idem
		kEvent_SayToDone				= 1 << 18,		// in Func0050 0x005791C1 in relation to SayToTopicInfo (OnSayToDone? or OnSayStart/OnSayEnd?)
		kEvent_OnGrab					= 1 << 19,		// 0x0095FACD and 0x009604B0 (same func which is called from PlayerCharacter_func001B and 0021)
		kEvent_OnRelease				= 1 << 20,		// 0x0047ACCA in relation to container
		kEvent_OnDestructionStageChange	= 1 << 21,		// 0x004763E7/0x0047ADEE
		kEvent_OnFire					= 1 << 22,		// 0x008BAFB9 (references to package use item and use weapon are close)

		kEvent_OnTrigger				= 1 << 28,		// 0x005D8D6A	Cmd_EnterTrigger_Execute
		kEvent_OnTriggerEnter			= 1 << 29,		// 0x005D8D50	Cmd_EnterTrigger_Execute
		kEvent_OnTriggerLeave			= 1 << 30,		// 0x0062C946	OnTriggerLeave ?
		kEvent_OnReset					= 1 << 31		// 0x0054E5FB
	};

	struct Event
	{
		TESForm* object;
		UInt32 eventMask;
	};

	struct Struct010
	{
		UInt8 unk00[8];
	};

	Script*						pkScript;			// 00
	UInt32						uiUnk1;				// 04
	BSSimpleList<Event*>*		pkEventList;		// 08
	BSSimpleList<ScriptLocal*>*	pkVarsList;			// 0C
	Struct010*					unk010;				// 10

	ScriptLocal* GetVariable(UInt32 id);
	UInt32 ResetAllVariables();
	ScriptEventList* Copy();
	void					EventCreate(UInt32 eventCode, TESObjectREFR* container);

};
static_assert(sizeof(ScriptEventList) == 0x14);