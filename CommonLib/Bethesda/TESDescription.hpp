#pragma once
#include "BaseFormComponent.hpp"
#include "BSString.hpp"

// 0x8
class TESDescription : public BaseFormComponent
{
public:
	TESDescription();
	~TESDescription();

	virtual char* Get(TESForm* overrideForm, UInt32 chunkID);

	UInt32	uiFileOffset;	// 4 - how does this work for descriptions in mods?
	// maybe extracts the mod ID then uses that to find the src file?

	static BSStringT<char>	kCurrentDescriptionText;
	static TESDescription*	pkCurrentDescription;
};
static_assert(sizeof(TESDescription) == 0x8);