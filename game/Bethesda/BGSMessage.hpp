#pragma once
#include "TESForm.hpp"
#include "TESCondition.hpp"
#include "TESDescription.hpp"

class BGSMenuIcon;

// 0x40
class BGSMessage :
	public TESForm,			// 00
	public TESFullName,		// 18
	public TESDescription	// 24
{
public:
	BGSMessage();
	~BGSMessage();

	enum EnumFlags
	{
	};

	struct Button {
		BSStringT			kButtonText;	// 000
		TESCondition		kCondition;		// 008
	};

	BGSMenuIcon*			menuIcon;		// 2C
	BSSimpleList<Button*>	kButtons;		// 030
	EnumFlags				eFlags;			// 038 init'd to 1
	Float32					fDisplayTime;	// 03C init'd to 2
};
static_assert(sizeof(BGSMessage) == 0x40);