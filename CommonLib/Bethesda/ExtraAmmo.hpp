#pragma once
#include "BSExtraData.hpp"

class TESAmmo;

// 0x14
class ExtraAmmo : public BSExtraData {
public:
	ExtraAmmo();
	~ExtraAmmo();

	TESAmmo*	pkAmmo;
	UInt32		uiCount;
};
static_assert(sizeof(ExtraAmmo) == 0x14);