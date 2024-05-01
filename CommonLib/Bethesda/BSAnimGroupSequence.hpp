#pragma once

#include "NiControllerSequence.hpp"

class TESAnimGroup;

class BSAnimGroupSequence : public NiControllerSequence {
public:
	BSAnimGroupSequence();
	virtual ~BSAnimGroupSequence();

	NiPointer<TESAnimGroup> spAnimGroup;
};