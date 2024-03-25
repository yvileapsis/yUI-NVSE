#pragma once
#include "TESPackage.hpp"
#include "BSSimpleArray.hpp"
#include "NiPoint3.hpp"

// old NVSE header, everything should be verified.

// 030
struct SpectatorThreatInfo {
	TESForm*		unk000;		// 000
	TESForm*		unk004;		// 004
	UInt32			unk008;		// 008
	UInt32			unk00C;		// 00C	elapsed tick count
	UInt32			unk010;		// 010
	NiPoint3		unk014;		// 014	is array of 3 floats, should be Pos
	NiPoint3		unk020;		// 020	is array of 3 floats, should be Rot
	UInt8			unk02C;		// 02C
	UInt8			unk02D;		// 02D
	UInt8			pad[2];		// 02E
};

// 0B4
class SpectatorPackage : public TESPackage
{
public:
	SpectatorPackage();
	~SpectatorPackage();

	UInt32			unk080;		// 080
	UInt32			unk084;		// 084
	UInt32			unk088;		// 088
	UInt32			unk08C;		// 08C
	UInt8			unk090;		// 090
	UInt8			pad091[3];	// 091
	NiPoint3		unk094;		// 094	is array of 3 floats, should be Pos
	BSSimpleArray<SpectatorThreatInfo*>	arr0A0;	// 0A0
	// There is an object containing a semaphore at B0/B4
};