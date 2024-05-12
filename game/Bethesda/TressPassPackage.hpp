#pragma once
#include "TESPackage.hpp"

// 09C
class TressPassPackage : public TESPackage {
public:
	TressPassPackage();
	~TressPassPackage();

	Float32		unk080;		// 080
	UInt32		unk084;		// 084
	TESForm*	unk088;		// 088
	TESForm*	unk08C;		// 08C
	UInt32		unk090;		// 090
	UInt32		unk094;		// 094
	UInt32		unk098;		// 098
};