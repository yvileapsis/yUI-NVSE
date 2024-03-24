#pragma once

class CombatController;
class TESObjectREFR;
class NiPoint3;

class CombatProcedure {
public:
	virtual void	Destroy(bool doFree);
	virtual void	Update(void);
	virtual void	SetCombatController(CombatController* _combatCtrl);
	virtual void	Unk_03(void);
	virtual void	ClearTargettedRefIfEqualTo(TESObjectREFR* targettedRef);
	virtual void	DebugPrint(void);
	virtual void	Unk_06(NiPoint3* out, NiPoint3* in);
	virtual void	SetLastError_Disarmed(int unused);
	virtual void	Unk_08(int a1);
	virtual void	Unk_09(void);
	virtual void	Unk_0A(int a1, int a2, int a3, int a4);
	virtual void	Unk_0B(void);
	virtual void	Unk_0C(void);
	virtual void	Unk_0D(void);
	virtual void	SaveGame(int a1);
	virtual void	LoadGame(int a1);
	virtual void	LoadGame2(int a1);

	CombatController* combatCtrl;	// 04
	UInt32* status;		// 08
	char* errorText;		// 0C
};