#pragma once
#include "TESBoundAnimObject.hpp"
#include "TESSoundFile.hpp"

// 0x6C
class TESSound :
	public TESBoundAnimObject,
	public TESSoundFile
{
public:
	TESSound();
	~TESSound();

	enum EnumSoundFlags : UInt32
	{
		kFlag_RandomFrequencyShift	= 1 << 0,
		kFlag_PlayAtRandom			= 1 << 1,
		kFlag_EnvironmentIgnored	= 1 << 2,
		kFlag_RandomLocation		= 1 << 3,
		kFlag_Loop					= 1 << 4,
		kFlag_MenuSound				= 1 << 5,
		kFlag_2D					= 1 << 6,
		kFlag_360LFE				= 1 << 7,
		kFlag_DialogueSound			= 1 << 8,
		kFlag_EnvelopeFast			= 1 << 9,
		kFlag_EnvelopeSlow			= 1 << 10,
		kFlag_2DRadius				= 1 << 11,
		kFlag_MuteWhenSubmerged		= 1 << 12,
		kFlag_StartAtRandomPosition	= 1 << 13,
	};

	struct  TESSoundData
	{
		UInt8			minAttenuationDist;
		UInt8			maxAttenuationDist;
		UInt8			frequencyAdj;
		UInt8			byte03;
		EnumSoundFlags	eFlags;
		UInt16			staticAttenuation;
		UInt8			endsAt;
		UInt8			startsAt;
		UInt16			attenuationCurve[5];
		UInt16			reverbAttenuation;
		UInt32			priority;
		UInt32			loopPointBegin;
		UInt32			loopPointEnd;
	};

	BSStringT<char>	kEditorID;
	TESSoundData	kData;
	UInt8			ucRngChance;
	UInt8			gap69[3];

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		kData.eFlags = (EnumSoundFlags) (bMod ? (kData.eFlags | pFlag) : (kData.eFlags & ~pFlag));
	}
};
static_assert(sizeof(TESSound) == 0x6C);