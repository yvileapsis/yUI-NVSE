#pragma once

#include "BSExtraData.hpp"

//	Seen used as ForceEquip ! Unused as forceUnequip (bug?)
class ExtraCannotWear : public BSExtraData {
public:
	ExtraCannotWear();
	~ExtraCannotWear();

	static ExtraCannotWear* Create();
};