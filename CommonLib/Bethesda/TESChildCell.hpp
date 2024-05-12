#pragma once
#include "TESObjectCELL.hpp"

class TESChildCell 
{
public:
	virtual TESObjectCELL* GetPersistentCell();
};
static_assert(sizeof(TESChildCell) == 0x4);