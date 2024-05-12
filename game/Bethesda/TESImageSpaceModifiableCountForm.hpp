#pragma once

#include "BaseFormComponent.hpp"

class TESImageSpaceModifier;

class TESImageSpaceModifiableCountForm : public BaseFormComponent {
public:
	TESImageSpaceModifier* pFormImageSpaceModifying[6];
};