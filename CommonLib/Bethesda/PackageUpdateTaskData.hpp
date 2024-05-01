#pragma once

#include "MobileObjectTaskletData.hpp"

class TESObjectREFR;

// Unused
class PackageUpdateTaskData : public MobileObjectTaskletData {
public:
	UInt32 unk38;
	UInt32 unk3C;
	UInt32 unk40;

	static void __fastcall ExecuteTaskEx(PackageUpdateTaskData* apThis, void*, TESObjectREFR* apRefr);
};

ASSERT_SIZE(PackageUpdateTaskData, 0x44);