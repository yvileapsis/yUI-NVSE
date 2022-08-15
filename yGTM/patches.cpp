#include <patches.h>
#include <SafeWrite.h>
#include <timeMult.h>
#include <gameplay.h>

void patchTimeMult(const bool bEnable)
{
	if (bEnable) {

		WriteRelCall(0x93EC38, TimeMult::SetGTMHook<TimeMult::eTimeIndexTurbo>);
		WriteRelCall(0x93ECCE, TimeMult::RestoreGTMHook<TimeMult::eTimeIndexTurbo>);

		WriteRelCall(0x93EAA0, TimeMult::SetGTMHook<TimeMult::eTimeIndexKillCam>);
		WriteRelCall(0x93E77C, TimeMult::RestoreGTMHook<TimeMult::eTimeIndexKillCam>);

		WriteRelCall(0x9C7EA9, TimeMult::SetGTMHook<TimeMult::eTimeIndexKillCam>);
		WriteRelCall(0x9C6F3A, TimeMult::RestoreGTMHook<TimeMult::eTimeIndexKillCam>);

		WriteRelCall(0x850CEE, TimeMult::SetGTMOnLoadHook);

		WriteRelCall(0x9C8A82, TimeMult::SetGTMVATSHook);

		WriteRelJump(0x720133, TimeMult::DialogVATSDestructorHook<0x720138>);
		WriteRelJump(0x761953, TimeMult::DialogVATSDestructorHook<0x761958>);

	} else {

		UndoSafeWrite(0x93EC38);
		UndoSafeWrite(0x93ECCE);

		UndoSafeWrite(0x93EAA0);
		UndoSafeWrite(0x93E77C);

		UndoSafeWrite(0x9C7EA9);
		UndoSafeWrite(0x9C6F3A);

		UndoSafeWrite(0x850CEE);

		UndoSafeWrite(0x9C8A82);

		UndoSafeWrite(0x720133);
		UndoSafeWrite(0x761953);

	}
}

void patchFixExplosionPushForce(const bool bEnable)
{
	if (bEnable) {
		// stooee
		WriteRelJump(0x89C8CF, HitKnockbackHook<0x89C8D4>);
		SafeWriteBuf(0x89C8E3, "\x41\x08\x85\xC0\x0F\x44\xC2\x83\xC0\x30\x0F\x1F\x40\x00", 14);

	} else {

		UndoSafeWrite(0x89C8CF);
		UndoSafeWrite(0x89C8E3);

	}
}

void patchRestoreSpreadGameSettings(const bool bEnable)
{
	if (bEnable) {

		WriteRelJump(0x8B0E54, RestoreMinSpreadHook<0x8B0E65>);
		WriteRelJump(0x8B0EEE, RestoreSpreadConditionHook<0x8B0F01>);
		WriteRelJump(0x64695B, AlterSpreadHook<0x646961, 0x646CA3>);
	}
	else {

		UndoSafeWrite(0x8B0E54);

	}
}

void patchCorrectAmmoEffects(const bool bEnable)
{
	if (bEnable) {

		SafeWriteBuf(0x9BC0C9, "\xD9\xEE", 2);								// zero the health percent

		WriteRelJump(0x9BC1D7, StoreAmmoOnProjectileHook<0x9BC1F9>);		// store ammo on the projectile

		WriteRelJump(0x9C36E3, StoreAmmoOnExplosionHook<0x9C36F1>);			// copy ammo from projectile to explosion

		WriteRelJump(0x89ABDC, GetAmmoFromHitDataHook<0x89AC3C>);			// get ammo from projectile and explosion


		WriteRelJump(0x9AC77E, FixExplosionAmmoHook1<0x9AC785>);			// HACKY HACKY HACKY
		WriteRelJump(0x9B2662, FixExplosionAmmoHook1<0x9B2669>);			// Fix Destruct and Save so that it checks for 
		WriteRelJump(0x9B066E, FixExplosionAmmoHook1<0x9B0675>);
		WriteRelJump(0x9B21F7, FixExplosionAmmoHook2<0x9B2201>);			// the same things Init does
		WriteRelJump(0x9B0681, FixExplosionAmmoHook3<0x9B0688>);
		// jipln

		WriteRelJump(0x9B5623, PreCalculateHitDamageHook<0x9B5628>);
		WriteRelCall(0x9B562D, PostCalculateHitDamageHook1);
		WriteRelJump(0x9B5702, PreCalculateHitDamageHook<0x9B5707>);
		WriteRelCall(0x9B5716, PostCalculateHitDamageHook2);
		WriteRelJump(0x9B575C, PreCalculateHitDamageHook<0x9B5761>);
		WriteRelCall(0x9B5764, PostCalculateHitDamageHook2);
		WriteRelJump(0x9B58AE, PreCalculateHitDamageHook<0x9B58B3>);
		WriteRelCall(0x9B58BE, PostCalculateHitDamageHook2);
		WriteRelJump(0x9B5A10, PreCalculateHitDamageHook<0x9B5A15>);
		WriteRelCall(0x9B5A18, PostCalculateHitDamageHook2);
	}
}

void patchCorrectMeltdownEffects(const bool bEnable)
{
	if (bEnable) {
		WriteRelJump(0x89B455, MeltdownPassHitDataWeaponHook<0x89B4CC>);	// pass current weapon not player weapon
		WriteRelJump(0x89B670, MeltdownSetWeaponAndAmmoHook<0x89B6D0>);		// set weapon and ammo for the meltdown explosion
	}
}

void patchCorrectWeaponEffects(const bool bEnable)
{
	if (bEnable) {

		//	WriteRelCall(0x8C248E, Test);
		//	WriteRelCall(0x8C2593, Test);
		//	WriteRelCall(0x8C250E, Test);
		//	WriteRelJump(0x816266, Test3);


		WriteRelJump(0x8C2243, WeaponEffectHook1<0x8C2310>);
		WriteRelJump(0x8C2342, WeaponEffectHook2<0x8C2366>);

		WriteRelJump(0x89AB44, MergeScriptEventHook<0x89ABDC>);
	}
}

void patchMultiplicativeProjectileCount(const bool bEnable)
{
	if (bEnable) {
		WriteRelJump(0x525B20, TESObjectWEAPGetNumProjectilesHook);
		SafeWrite8(0x708343, 0x38);
		WriteRelCall(0x70835E, 0x525B20); // undo stewie's fix, replace with mine
	}
}

void patchArmedUnarmed(const bool bEnable)
{
	if (bEnable) {
		WriteRelCall(0x7724CB, ArmedUnarmed::ShouldNotShowAmmo);

		WriteRelJump(0x893C37, ArmedUnarmed::ShouldFireHook<0x893C3D, 0x893E2C>);

		WriteRelJump(0x895C9B, ArmedUnarmed::QueuePowerAttackFireHook<0x895CA1>);
		WriteRelJump(0x895D39, ArmedUnarmed::QueueAttackFireHook<0x895D3E>);
		WriteRelCall(0x8BADD3, ArmedUnarmed::ExecuteAttackHook);
		WriteRelJump(0x8BAE53, ArmedUnarmed::ExecuteAttackHook2<0x8BAE58, 0x8BAF03>);

		WriteRelJump(0x9492A1, ArmedUnarmed::PlayerAttackRemoveAmmoCheckHook<0x949308, 0x94948D>);
	}
}