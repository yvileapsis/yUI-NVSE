#pragma once

class BSRandom {
public:
	DWORD dword0;
	DWORD dword4;
	int gap8[624];
	DWORD iter;

	static BSRandom* GetSingleton() {
		return (BSRandom*)0x11C4180;
	}

	UInt32 UnsignedIntBelow(UInt32 auiMax) {
		return ThisStdCall(0xAA5230, this, auiMax);
	}

	UInt32 UnsignedInt() {
		return UnsignedIntBelow(0xFFFFFFFF);
	}

	float GetRandomNumberBetween(float min, float max) {
		return (UnsignedInt() * (max - min) / 4294967296.0 + min);
	}
};
