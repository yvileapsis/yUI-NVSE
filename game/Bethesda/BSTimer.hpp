#pragma once

struct BSTimer {
	bool bDisableCounter;
	float fClamp;
	float fClampRemainder;
	float fDelta;
	float fRealTimeDelta;
	UInt32 uiMsPassed;
	UInt32 uiUnk18;
	bool bIsChangeTimeMultSlowly;
	byte unk1D;
	byte unk1E;
	byte unk1F;
};