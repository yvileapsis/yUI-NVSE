#pragma once

#include "TESBoundAnimObject.hpp"
#include "TESSoundFile.hpp"

class TESSound : public TESBoundAnimObject {
public:
	TESSound();
	~TESSound();

	enum TESSoundFlags
	{
		kFlag_RandomFrequencyShift = 1,
		kFlag_PlayAtRandom = 2,
		kFlag_EnvironmentIgnored = 4,
		kFlag_RandomLocation = 8,
		kFlag_Loop = 16,
		kFlag_MenuSound = 32,
		kFlag_2D = 64,
		kFlag_360LFE = 128,
		kFlag_DialogueSound = 256,
		kFlag_EnvelopeFast = 512,
		kFlag_EnvelopeSlow = 1024,
		kFlag_2DRadius = 2048,
		kFlag_MuteWhenSubmerged = 4096,
		kFlag_StartAtRandomPosition = 8192,
	};

	struct  TESSoundData
	{
		UInt8 minAttenuationDist;
		UInt8 maxAttenuationDist;
		byte frequencyAdj;
		UInt8 byte03;
		UInt32 soundFlags;
		UInt16 staticAttenuation;
		UInt8 endsAt;
		UInt8 startsAt;
		UInt16 attenuationCurve[5];
		UInt16 reverbAttenuation;
		UInt32 priority;
		UInt32 loopPointBegin;
		UInt32 loopPointEnd;
	};

	TESSoundFile soundFile;
	BSStringT editorID;
	TESSoundData data;
	UInt8 rngChance;
	UInt8 gap69[3];

	void SetFlag(UInt32 pFlag, bool bMod)
	{
		data.soundFlags = bMod ? (data.soundFlags | pFlag) : (data.soundFlags & ~pFlag);
	}
};

ASSERT_SIZE(TESSound, 0x6C);