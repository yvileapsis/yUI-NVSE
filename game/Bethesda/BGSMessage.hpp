#pragma once

#include "TESForm.hpp"
#include "TESCondition.hpp"
#include "TESDescription.hpp"

class BGSMenuIcon;

class BGSMessage : public TESForm {
public:
	BGSMessage();
	~BGSMessage();

	struct Button {
		BSStringT	buttonText;	// 000
		TESCondition condition;	// 008
	};

	TESFullName		fullName;			// 018
	TESDescription	description;		// 024

	BGSMenuIcon* menuIcon;		// 2C
	BSSimpleList<Button*>	buttons;		// 030
	UInt32			messageFlags;	// 038 init'd to 1
	float			displayTime;	// 03C init'd to 2
};

ASSERT_SIZE(BGSMessage, 0x040);