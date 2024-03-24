#pragma once

#include "BSExtraData.hpp"

class TESAmmo;

class ExtraAmmo : public BSExtraData {
public:
	ExtraAmmo();
	~ExtraAmmo();

	TESAmmo* ammo;
	UInt32 unk4;
};