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

		WriteRelJump(0x8B0E54, RestoreSpreadHook<0x8B0E65>);

	} else {

		UndoSafeWrite(0x8B0E54);
		
	}
}