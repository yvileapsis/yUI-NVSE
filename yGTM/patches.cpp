#include <patches.h>
#include <SafeWrite.h>
#include <timemult.h>

void patchTimeMult()
{
	WriteRelCall(0x93EC38, reinterpret_cast<UInt32>(SetGTMHook<eTimeIndexTurbo>));
	WriteRelCall(0x93ECCE, reinterpret_cast<UInt32>(RestoreGTMHook<eTimeIndexTurbo>));

	WriteRelCall(0x93EAA0, reinterpret_cast<UInt32>(SetGTMHook<eTimeIndexKillCam>));
	WriteRelCall(0x93E77C, reinterpret_cast<UInt32>(RestoreGTMHook<eTimeIndexKillCam>));

	WriteRelCall(0x9C7EA9, reinterpret_cast<UInt32>(SetGTMHook<eTimeIndexKillCam>));
	WriteRelCall(0x9C6F3A, reinterpret_cast<UInt32>(RestoreGTMHook<eTimeIndexKillCam>));

	WriteRelCall(0x850CEE, reinterpret_cast<UInt32>(SetGTMOnLoadHook));

	WriteRelCall(0x9C8A82, reinterpret_cast<UInt32>(SetGTMVATSHook));

	WriteRelJump(0x720133, reinterpret_cast<UInt32>(DialogVATSDestructorHook<0x720138>));
	WriteRelJump(0x761953, reinterpret_cast<UInt32>(DialogVATSDestructorHook<0x761958>));
}
