#pragma once
#include "TESPackage.hpp"

// 0xAC
class FleePackage : public TESPackage {
public:
	FleePackage();
	~FleePackage();

	typedef struct {
		float vector[3];
	} ThreeFloatArray;

	UInt8					unk080;		// 080
	UInt8					unk081;		// 081
	UInt8					pad082[2];	// 082
	ThreeFloatArray			unk084;		// 084	is array of 3 floats, should be Pos
	Float32					unk090;		// 090
	UInt8					unk094;		// 094
	UInt8					pad095[3];	// 095
	BSSimpleList<TESForm*>	list098;	// 098
	TESForm*				unk0A0;		// 0A0
	TESForm*				unk0A4;		// 0A4
	UInt8					unk0A8;		// 0A8
	UInt8					unk0A9;		// 0A9
	UInt8					pad0AA[2];	// 0AA
};