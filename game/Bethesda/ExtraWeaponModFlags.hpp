#pragma once

#include "BSExtraData.hpp"

class ExtraWeaponModFlags : public BSExtraData {
public:
	ExtraWeaponModFlags();
	~ExtraWeaponModFlags();

	UInt8	flags; // 00C

	static ExtraWeaponModFlags* Create();
};