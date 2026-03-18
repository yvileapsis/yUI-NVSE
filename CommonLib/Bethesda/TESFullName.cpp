#include "TESFullName.hpp"

// GAME - 0x408DA0
const char* TESFullName::GetFullName() const {
	return ThisStdCall<const char*>(0x408DA0, this);
}

// GAME - 0x482720
// GECK - 0x437DB0
const char* TESFullName::GetFullName(const TESForm* apForm) {
	return CdeclCall<const char*>(0x482720, apForm);
}
