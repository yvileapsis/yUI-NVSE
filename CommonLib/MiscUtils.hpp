#pragma once
// TODO: do something regarding perf_counter
//#include "PerfCounter.hpp"
#include "d3d9.h"

static inline void D3DEventStart(LPCWSTR apName) {
	D3DPERF_BeginEvent(0xFFFFFFFF, apName);
}

static inline void D3DEventEnd() {
	D3DPERF_EndEvent();
}

static inline void D3DSetMarker(LPCWSTR apName) {
	D3DPERF_SetMarker(0xFFFFFFFF, apName);
}

static void Clamp(float& afValue, float afMin, float afMax) {
	if (afMax >= afValue) {
		if (afMin > afValue)
			afValue = afMin;
	}
	else {
		afValue = afMax;
	}
}