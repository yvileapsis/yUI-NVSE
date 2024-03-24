#pragma once

#include "NiPoint3.hpp"

class BSSoundHandle {
public:
	UInt32	uiSoundID;
	bool	bAssumeSuccess;
	UInt32	uiState;

	BSSoundHandle() : uiSoundID(-1), bAssumeSuccess(false), uiState(0) {}
	~BSSoundHandle() {}

	bool IsValid();
	bool IsInvalid() const { return uiSoundID == -1; }

	bool Play(bool abUnk);
	bool Stop();

	bool Release();

	bool SetStaticAttenuation(UInt16 ausAttenuation);
	bool SetReverbAttenuation(UInt16 ausAttenuation);
	bool SetVolume(float afVolume);
	bool SetSpeed(float afSpeed);
	bool SetPosition(NiPoint3& arPosition);
	bool SetMinMaxDistance(float afMin, float afMax);
	bool SetAttenuationCurve(SInt16 ausUnk1, SInt16 ausUnk2, UInt16 ausStaticAttenuation, UInt16 ausSystemAttenuation, UInt16 ausDistanceAttenuation);
};

ASSERT_SIZE(BSSoundHandle, 0xC);