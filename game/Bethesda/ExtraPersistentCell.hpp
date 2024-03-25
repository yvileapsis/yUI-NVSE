#pragma once

#include "BSExtraData.hpp"

class TESObjectCELL;

class ExtraPersistentCell : public BSExtraData {
public:
	ExtraPersistentCell();
	virtual ~ExtraPersistentCell();

	TESObjectCELL* persistentCell;	// 0C
};
static_assert(sizeof(ExtraPersistentCell) == 0x10);