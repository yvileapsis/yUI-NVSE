#pragma once

#include "TESRace.hpp"

class TESRaceForm : public BaseFormComponent {
public:
	TESRaceForm();
	~TESRaceForm();

	TESRace* race;	// 04
};