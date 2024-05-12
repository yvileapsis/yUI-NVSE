#pragma once

#include "TESForm.hpp"
#include "TESTexture.hpp"
#include "TESDescription.hpp"


class TESLoadScreenType;

class TESLoadScreen : public TESForm, public TESTexture, public TESDescription {
public:
	TESLoadScreen();
	~TESLoadScreen();

	BSSimpleList<void*> unkList2C;
	TESLoadScreenType*	pType;
	BSStringT			string;
};

ASSERT_SIZE(TESLoadScreen, 0x40);