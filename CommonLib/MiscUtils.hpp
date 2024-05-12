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

static float ConvertToKB(SIZE_T size) {
	return (float)size / 1024.0f;
}

static float ConvertToMB(SIZE_T size) {
	return (float)size / 1024.0f / 1024.0f;
}

static float ConvertToGB(SIZE_T size) {
	return (float)size / 1024.0f / 1024.0f / 1024.0f;
}

static std::string FormatSize(SIZE_T size) {
	std::string result;
	if (size < 1024) {
		result = std::format("{:>6d} B", size);
	}
	else if (size < 1024 * 1024) {
		result = std::format("{:>6.2f} KB", ConvertToKB(size));
	}
	else if (size < 1024 * 1024 * 1024) {
		result = std::format("{:>6.2f} MB", ConvertToMB(size));
	}
	else {
		result = std::format("{:>6.2f} GB", ConvertToGB(size));
	}
	return result;
}