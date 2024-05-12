#pragma once

#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESReactionForm.hpp"
#include "TESReputation.hpp"

class TESFaction : public TESForm, public TESFullName, public TESReactionForm {
public:
	TESFaction();
	~TESFaction();
	enum
	{
		// TESForm flags

		// TESReactionForm flags

		kModified_FactionFlags = 0x00000004
		// CHANGE_FACTION_FLAGS
		// UInt8	flags;
	};

	enum
	{
		kFlag_HiddenFromPC = 0x00000001,
		kFlag_Evil = 0x00000002,
		kFlag_SpecialCombat = 0x00000004,

		kFlag_TrackCrime = 0x00000100,
		kFlag_AllowSell = 0x00000200,
	};

	// 1C
	struct Rank {
		BSStringT		name;		// 00
		BSStringT		femaleName;	// 08
		TESTexture	insignia;	// 10 - effectively unused, can be set but there is no faction UI
	};

	Bitfield32			factionFlags;	// 34
	TESReputation*		reputation;		// 38
	BSSimpleList<Rank*>	ranks;			// 3C
	UInt32				crimeCount44;	// 44
	UInt32				crimeCount48;	// 48


	void SetFlag(UInt32 flag, bool bMod) {
		factionFlags.SetBit(flag, bMod);
		AddChange(kModified_FactionFlags);
	}
	bool IsHidden() const {
		return factionFlags.GetBit(kFlag_HiddenFromPC);
	}
	bool IsEvil() const {
		return factionFlags.GetBit(kFlag_Evil);
	}
	bool HasSpecialCombat() const {
		return factionFlags.GetBit(kFlag_SpecialCombat);
	}
	void SetHidden(bool bHidden) {
		SetFlag(kFlag_HiddenFromPC, bHidden);
	}
	void SetEvil(bool bEvil) {
		SetFlag(kFlag_Evil, bEvil);
	}
	void SetSpecialCombat(bool bSpec) {
		SetFlag(kFlag_SpecialCombat, bSpec);
	}
	const char* GetNthRankName(UInt32 whichRank, bool bFemale = false);
	void SetNthRankName(const char* newName, UInt32 whichRank, bool bFemale);
};

ASSERT_SIZE(TESFaction, 0x4C);