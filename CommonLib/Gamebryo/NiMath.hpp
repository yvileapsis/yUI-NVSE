#pragma once

__forceinline bool NiIsPowerOf2(UInt32 uiI) {
	if (!uiI)
		return false;

	return (uiI & (uiI - 1)) == 0;
}