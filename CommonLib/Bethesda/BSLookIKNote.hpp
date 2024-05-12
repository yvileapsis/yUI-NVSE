#pragma once

#include "BSAnimNote.hpp"

class BSLookIKNote : public BSAnimNote {
public:
	float	fGain;
	UInt32	eState;

	NIRTTI_ADDRESS(0x11F3EE8);
};

ASSERT_SIZE(BSLookIKNote, 0x18);