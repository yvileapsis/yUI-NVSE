#pragma once
#include "GameTasks.h"

class ProcessManager
{
	enum ProcessLevel
	{
		kProcessManagerLevel_High = 0x0,
		kProcessManagerLevel_MidHigh = 0x1,
		kProcessManagerLevel_MidLow = 0x2,
		kProcessManagerLevel_Low = 0x3,
	};

	struct Unk160
	{
		UInt32 unk0000[16480];
		UInt8 byte10180;
		UInt8 gap10181[3];
	};

	float unk000;
	MobileObject* objectsNiTArray[4];
	UInt32 beginOffsets[4];
	UInt32 endOffsets[4];
	UInt32 unk034[4];
	TList<void> crimes[5];
	UInt32 unk058[2];
	TList<void> BSTempEffects;
	TList<void> list068;
	TList<void> arrowProjectileList;
	TList<void> actorList78;
	TList<void> highActors;
	MobileObject* nearbyActorsArray[50];
	UInt32 numShadowUpdatedActors;
	float unk154;
	float unk158;
	UInt8 playerInRadiation1;
	UInt8 playerInRadiation2;
	UInt8 gap15E[2];
	Unk160 unk160;
	TList<void> list102E4;
	UInt32 unk102EC;
	UInt32 unk102F0;
	UInt32 unk102F4;
	UInt32 unk102F8;
	UInt32 unk102FC;
	CRITICAL_SECTION criticalSection;
	UInt32 unk10318;
	UInt32 unk1031C;
	UInt32 unk10320;
	UInt32 unk10324;
	UInt32 unk10328;
	UInt32 unk1032C;
	UInt32 unk10330;
	UInt32 unk10334;
	UInt32 unk10338;
	UInt32 unk1033C;
	UInt32 unk10340;
	UInt32 unk10344;
	UInt32 unk10348;
	UInt32 unk1034C;
	UInt32 unk10350;
	UInt32 unk10354;
	UInt32 unk10358;
	UInt32 unk1035C;
	LockFreeMap<UInt32, void*> lockFreeMap10360;
	UInt32 unk1038C;
	UInt32 unk10390;
	UInt32 unk10394;
	UInt32 unk10398;
	UInt32 unk1039C;
	UInt8 byte103A0;
	UInt8 byte103A1;
	UInt8 byte103A2;
	UInt8 gap103A3;
	UInt32 unk103A4;
	UInt8 byte103A8;
	UInt8 byte103A9;
	UInt8 byte103AA;
	UInt8 byte103AB;
	UInt8 byte103AC;
	UInt8 byte103AD;
	UInt8 byte103AE;
	UInt8 gap103AF;
	UInt32 unk103B0;
	float unk103B4;
	UInt32 unk103B8;
	UInt32 fRemoveExcessComplexDeadTime;
	Semaphore semaphore103C0;
};

inline ProcessManager* g_processManager = (ProcessManager*)0x011E0E80;