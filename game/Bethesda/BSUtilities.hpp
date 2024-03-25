#pragma once

static void BSsprintf(const char* apDest, size_t auiSize, const char* apFormat, ...) {
	va_list va;
	va_start(va, apFormat);
	CdeclCall(0xEC6B2D, apDest, auiSize, apFormat, 0, va);
}

static double remap(float new_min, float new_max, float old_min, float old_max, float old_value) {
	return ((old_value - old_min) / (old_max - old_min) * (new_max - new_min) + new_min);
}

// 0x4E44F0
static void fClamp(float* apOut, float afMin, float afMax) {
	if (afMax >= afMin) {
		if (afMax >= *apOut) {
			if (afMin > *apOut) {
				*apOut = fmodf(afMax - afMin, *apOut - afMin) + afMax;
			}
		}
		else {
			*apOut = fmodf(afMax - afMin, *apOut - afMin) + afMin;
		}
	}
}