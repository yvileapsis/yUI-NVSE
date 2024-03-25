#pragma once
#include "BSSimpleList.hpp"

class TESObjectREFR;
class EffectItem;
class ActiveEffect;

// 0x10
class MagicTarget {
public:
	MagicTarget();
	virtual ~MagicTarget();
	virtual TESObjectREFR* GetParent();
	virtual BSSimpleList<ActiveEffect>* GetEffectList();

	UInt8					byt004;		// 004
	UInt8					byt005;		// 005
	UInt8					byt006[2];	// 006-7
	BSSimpleList<UInt32>	lst008;		// 008

	void RemoveEffect(EffectItem* effItem);

	void StopEffect(void* arg0, bool arg1)
	{
		ThisStdCall(0x8248E0, this, arg0, arg1);
	}
};
static_assert(sizeof(MagicTarget) == 0x10);