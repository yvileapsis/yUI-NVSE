#pragma once

#include "BSTimer.hpp"

struct TimeGlobal : public BSTimer {

	static TimeGlobal* GetSingleton() {
		return &*(TimeGlobal*)0x11F6394;
	}

	static float GetGlobalTimeMult() {
		return *(float*)0x11AC3A0;
	}
};
