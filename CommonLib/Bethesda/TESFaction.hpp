#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESTexture.hpp"
#include "TESReactionForm.hpp"

// 0x4C
class TESFaction :
	public TESForm,			// 00
	public TESFullName,		// 18
	public TESReactionForm	// 24
{
public:
	TESFaction();
	~TESFaction();

	enum EnumFactionFlags : UInt32
	{
		kFlag_HiddenFromPC	= 0x00000001,
		kFlag_Evil			= 0x00000002,
		kFlag_SpecialCombat	= 0x00000004,

		kFlag_TrackCrime	= 0x00000100,
		kFlag_AllowSell		= 0x00000200,
	};

	// 1C
	struct Rank
	{
		BSStringT<char>		kName;			// 00
		BSStringT<char>		kFemaleName;	// 08
		TESTexture			kInsignia;		// 10 - effectively unused, can be set but there is no faction UI
	};

	EnumFactionFlags		eFlags;			// 34
	TESReputation*			pkReputation;	// 38
	BSSimpleList<Rank*>		kRanks;			// 3C
	UInt32					crimeCount44;	// 44
	UInt32					crimeCount48;	// 48

	bool IsFlagSet(UInt32 flag) {
		return (eFlags & flag) != 0;
	}
	void SetFlag(UInt32 flag, bool bMod) {
		eFlags = (EnumFactionFlags) (bMod ? (eFlags | flag) : (eFlags & ~flag));
		AddChange(kModified_FactionFlags);
	}
	bool IsHidden()
	{
		return IsFlagSet(kFlag_HiddenFromPC);
	}
	bool IsEvil()
	{
		return IsFlagSet(kFlag_Evil);
	}
	bool HasSpecialCombat()
	{
		return IsFlagSet(kFlag_SpecialCombat);
	}
	void SetHidden(bool bHidden)
	{
		SetFlag(kFlag_HiddenFromPC, bHidden);
	}
	void SetEvil(bool bEvil)
	{
		SetFlag(kFlag_Evil, bEvil);
	}
	void SetSpecialCombat(bool bSpec)
	{
		SetFlag(kFlag_SpecialCombat, bSpec);
	}
	const char* GetNthRankName(UInt32 whichRank, bool bFemale = false);
	void SetNthRankName(const char* newName, UInt32 whichRank, bool bFemale);
};
static_assert(sizeof(TESFaction) == 0x4C);