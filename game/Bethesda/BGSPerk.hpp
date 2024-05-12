#pragma once
#include "TESForm.hpp"
#include "TESDescription.hpp"
#include "TESIcon.hpp"
#include "TESCondition.hpp"

class BGSPerkEntry;

// 0x8
struct PerkData
{
	UInt8				bIsTrait;		// 00
	UInt8				ucMinLevel;		// 01
	UInt8				ucNumRanks;		// 02
	UInt8				bIsPlayable;	// 03 unverified
	UInt8				bIsHidden;		// 04 unverified
	UInt8				unk05;			// 05 todo: collapse to pad[3] after verifying isPlayable and isHidden
	UInt8				unk06;			// 06
	UInt8				unk07;			// 07
};
static_assert(sizeof(PerkData) == 0x8);

// 0x50
class BGSPerk :
	public TESForm,
	public TESFullName,
	public TESDescription,
	public TESIcon,
	public PerkData
{
public:
	BGSPerk();
	~BGSPerk();

	TESCondition					kConditions;			// 40
	BSSimpleList<BGSPerkEntry*>		kPerkEntries;			// 48
};
static_assert(sizeof(BGSPerk) == 0x50);