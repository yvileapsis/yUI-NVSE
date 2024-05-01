#pragma once
#include "TESForm.hpp"
#include "TESTexture.hpp"
#include "TESDescription.hpp"

class TESLoadScreenType;

// 0x3C
class TESLoadScreen :
	public TESForm,
	public TESTexture,
	public TESDescription
{
public:

	TESLoadScreen();
	~TESLoadScreen();

	BSSimpleList<void*> unkList2C;
	TESLoadScreenType*	pType;
	BSStringT<char>		string;	// 02C
};
static_assert(sizeof(TESLoadScreen) == 0x40);