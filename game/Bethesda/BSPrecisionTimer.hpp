#pragma once

struct BSPrecisionTimer {
	BSPrecisionTimer();
	BSPrecisionTimer(float afTargetTime);

	SInt64 iStartTime;
	SInt64 iTargetTime;


	static float fFrequencyMSRecip;
	static float fFrequencyMS;

	static void Initialize();
	static LARGE_INTEGER GetTimer();
	void Reset(float afTargetTime);
	bool Check();
	float ElapsedTime();
	float RemainingTime();
	void Clear();
};

ASSERT_SIZE(BSPrecisionTimer, 0x10);