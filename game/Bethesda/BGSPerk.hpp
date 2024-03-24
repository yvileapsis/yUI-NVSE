#pragma once

#include "TESForm.hpp"
#include "TESDescription.hpp"
#include "TESIcon.hpp"
#include "TESCondition.hpp"

class BGSPerkEntry;

class BGSPerk : public TESForm {
public:
	BGSPerk();
	~BGSPerk();

	struct PerkData
	{
		UInt8				isTrait;	// 00
		UInt8				minLevel;	// 01
		UInt8				numRanks;	// 02
		UInt8				isPlayable;	// 03 unverified
		UInt8				isHidden;	// 04 unverified
		UInt8				unk05;		// 05 todo: collapse to pad[3] after verifying isPlayable and isHidden
		UInt8				unk06;		// 06
		UInt8				unk07;		// 07
	};

	TESFullName				fullName;			// 18
	TESDescription			description;		// 24
	TESIcon					icon;				// 2C
	PerkData				data;				// 38
	TESCondition		conditions;			// 40
	BSSimpleList<BGSPerkEntry>		entries;			// 48
};