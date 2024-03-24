#pragma once

#include "BaseFormComponent.hpp"

class TESSound;

class BGSPickupPutdownSounds : public BaseFormComponent {
public:
	BGSPickupPutdownSounds();
	~BGSPickupPutdownSounds();

	TESSound* pPickupSound;		// 004
	TESSound* pPutdownSound;		// 008
};