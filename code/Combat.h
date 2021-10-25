#pragma once
struct CombatActionInfo
{
	char* name;
	float cost;
	UInt32 unk08;
};

CombatActionInfo** combatInfos = (CombatActionInfo * *)0x11A4280; // 27

enum CombatActions
{
	COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE = 0,
	COMBAT_ACTION_ATTACK_RANGED_EXPLOSIVE_F,
	COMBAT_ACTION_ATTACK_RANGED,
	COMBAT_ACTION_ATTACK_RANGED_FROM_COVER,
	COMBAT_ACTION_ATTACK_GRENADE,
	COMBAT_ACTION_ATTACK_GRENADE_FLUSH_TARG,
	COMBAT_ACTION_ATTACK_GRENADE_FROM_COVER,
	COMBAT_ACTION_ATTACK_MELEE,
	COMBAT_ACTION_ATTACK_HAND_TO_HAND,
	COMBAT_ACTION_MOVE,
	COMBAT_ACTION_MOVE_AND_SWITCH_TO_MELEE,
	COMBAT_ACTION_MOVE_AND_ATTACK_RANGED,
	COMBAT_ACTION_MOVE_AND_ATTACK_RANGED_EX,
	COMBAT_ACTION_MOVE_AND_ATTACK_GRENADE,
	COMBAT_ACTION_DRAW_WEAPON,
	COMBAT_ACTION_SWITCH_WEAPON,
	COMBAT_ACTION_AVOID_THREAT,
	COMBAT_ACTION_SEARCH,
	COMBAT_ACTION_INVESTIGATE,
	COMBAT_ACTION_DODGE,
	COMBAT_ACTION_IGNORE_BLOCKED_TARGET,
	COMBAT_ACTION_FLEE,
	COMBAT_ACTION_ACTIVATE_COMBAT_ITEM,
	COMBAT_ACTION_USE_COMBAT_ITEM,
	COMBAT_ACTION_ACQUIRE_LINE_OF_SIGHT,
	COMBAT_ACTION_HIDE,
	COMBAT_ACTION_APPROACH_TARGET,
};

class CombatAction
{
public:
	CombatAction();
	~CombatAction();

	virtual void IsApplicable(void);
	virtual void AddProcedureToController(CombatController* controller, void* a2);
	virtual void Func0002(void);
	virtual void GetCost(CombatController* combatController, int a2);

	tList<void> list04;
	tList<void> list0C;
	tList<void> list14;
	tList<void> list1C;
	UInt32 actionID;
	UInt32 unk28;
};
STATIC_ASSERT(sizeof(CombatAction) == 0x2C);


class CombatProcedure
{
public:
	CombatProcedure();
	~CombatProcedure();

	virtual void Destroy(bool doFree);
	virtual void Update(void);
	virtual void SetCombatController(CombatController* controller);
	virtual void Unk03(void);
	virtual void ClearTargettedRefIfEqualTo(TESObjectREFR* targettedReference);
	virtual void DebugPrint(void);
	virtual void Unk06(NiPoint3* out, NiPoint3* in);
	virtual void SetLastError_Disarmed(int unused);
	virtual void Unk08(int a1);
	virtual void nullsub09(void);
	virtual void nullsub0A(int a1, int a2, int a3, int a4);
	virtual void Unk0B(void);
	virtual void Unk0C(void);
	virtual void Unk0D(void);
	virtual void SaveGame(int a1);
	virtual void LoadGame(int a1);
	virtual void LoadGame2(int a1);

	CombatController* combatController;
	DWORD* status;
	char* errorText;
};

class CombatProcedureMove : CombatProcedure
{
	CombatProcedureMove();
	~CombatProcedureMove();
	
public:

	enum Stage
	{
		kCombatProcedureMove_Initialising = 0x0,
		kCombatProcedureMove_Moving = 0x1,
		kCombatProcedureMove_MovingUsingCover = 0x2,
		kCombatProcedureMove_HidingBehindCover = 0x3,
	};
	
	UInt32 stage;
	PosAndCell destination;
	PosAndCell posAndCell24;
	float unk034;
	TESObjectREFR* targetedReference;
	UInt32 unk03C;
	UInt32 hidingTimer;
	UInt32 unk044;
	NiPoint3 pt048;
	UInt8 byte054;
	UInt8 gap055[3];
	NiRefObject* unk058;
	UInt8 byte05C;
	UInt8 gap05D[3];
	UInt32 unk060;
	TimePair timer064;
	TimePair checkCoverTimer;
	TimePair checkDestinationTimer;
};
