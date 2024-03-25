#pragma once
#include "ValueModifierEffect.hpp"

class NiRefObject;

// 0x5C
class AbsorbEffect : public ValueModifierEffect
{
public:
	AbsorbEffect();
	~AbsorbEffect();

	NiRefObject* unk4C;
	NiRefObject* unk50;
	NiRefObject* unk54;
	NiRefObject* unk58;
};
static_assert(sizeof(AbsorbEffect) == 0x5C);