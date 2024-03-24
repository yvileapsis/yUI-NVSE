#pragma once

#include "NiPoint3.hpp"

class BSSoundInfo {
public:
	struct Data {
		const char* dword0;
		char		cPath04[260];
		const char* pFilePath;
		char		cFilePath[260];
		float		float210;
		float		float214;
		NiPoint3	kPosition;
		DWORD		dword224;
		float		float228;
		float		float22C;
		UInt32		uiStaticAttenuation;
		UInt32		uiDistanceAttenuation;
		UInt32		uiFaderAttenuation;
	};


	UInt32		uiID;
	float		fFrequency;
	float		fVolume;
	Bitfield32	uiAudioFlags;
	UInt32		uiDuration;
	bool		bIsPlaying;
	Data		kData;
};
ASSERT_SIZE(BSSoundInfo, 0x254);