#include <patches.h>
#include <SafeWrite.h>
#include <timemult.h>

void patchTimeMult()
{
	WriteRelCall(0x93EC38, reinterpret_cast<UInt32>(TimeMult::SetGTMHook<TimeMult::eTimeIndexTurbo>));
	WriteRelCall(0x93ECCE, reinterpret_cast<UInt32>(TimeMult::RestoreGTMHook<TimeMult::eTimeIndexTurbo>));

	WriteRelCall(0x93EAA0, reinterpret_cast<UInt32>(TimeMult::SetGTMHook<TimeMult::eTimeIndexKillCam>));
	WriteRelCall(0x93E77C, reinterpret_cast<UInt32>(TimeMult::RestoreGTMHook<TimeMult::eTimeIndexKillCam>));

	WriteRelCall(0x9C7EA9, reinterpret_cast<UInt32>(TimeMult::SetGTMHook<TimeMult::eTimeIndexKillCam>));
	WriteRelCall(0x9C6F3A, reinterpret_cast<UInt32>(TimeMult::RestoreGTMHook<TimeMult::eTimeIndexKillCam>));

	WriteRelCall(0x850CEE, reinterpret_cast<UInt32>(TimeMult::SetGTMOnLoadHook));

	WriteRelCall(0x9C8A82, reinterpret_cast<UInt32>(TimeMult::SetGTMVATSHook));

	WriteRelJump(0x720133, reinterpret_cast<UInt32>(TimeMult::DialogVATSDestructorHook<0x720138>));
	WriteRelJump(0x761953, reinterpret_cast<UInt32>(TimeMult::DialogVATSDestructorHook<0x761958>));
}
