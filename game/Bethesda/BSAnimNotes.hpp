#pragma once

#include "NiObject.hpp"

class BSAnimNote;

class BSAnimNotes : public NiObject {
public:
	BSAnimNote**	ppNotes;
	UInt16			usCount;

	NIRTTI_ADDRESS(0x11F3F00);
};

ASSERT_SIZE(BSAnimNotes, 0x10);