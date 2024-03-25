#pragma once
#include "BaseFormComponent.hpp"

class TESSound;

// 0xC
class BGSPickupPutdownSounds : public BaseFormComponent {
public:
	BGSPickupPutdownSounds();
	~BGSPickupPutdownSounds();

	TESSound* pkPickupSound;		// 004
	TESSound* pkPutdownSound;		// 008
};
static_assert(sizeof(BGSPickupPutdownSounds) == 0xC);