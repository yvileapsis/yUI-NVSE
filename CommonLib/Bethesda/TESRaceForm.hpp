#pragma once
#include "BaseFormComponent.hpp"

class TESRace;

// 0x8
class TESRaceForm : public BaseFormComponent {
public:
	TESRaceForm();
	~TESRaceForm();

	TESRace* pkRace;	// 04
};
static_assert(sizeof(TESRaceForm) == 0x8);