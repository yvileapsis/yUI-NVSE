#pragma once
#include "BSExtraData.hpp"

// 0x10
class ExtraWeaponModFlags : public BSExtraData {
public:
	ExtraWeaponModFlags();
	~ExtraWeaponModFlags();

	UInt8	flags; // 00C

	static ExtraWeaponModFlags* Create();
};
static_assert(sizeof(ExtraWeaponModFlags) == 0x10);