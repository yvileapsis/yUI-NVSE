#pragma once

#include "BaseFormComponent.hpp"
#include "BSString.hpp"

class TESDescription : public BaseFormComponent {
public:
	TESDescription();
	~TESDescription();

	virtual char* Get(TESForm* overrideForm, UInt32 chunkID);

	UInt32	uiFileOffset;	// 4 - how does this work for descriptions in mods?
	// maybe extracts the mod ID then uses that to find the src file?

	static BSStringT CurrentDescriptionText;
	static TESDescription* CurrentDescription;
};

ASSERT_SIZE(TESDescription, 0x8);