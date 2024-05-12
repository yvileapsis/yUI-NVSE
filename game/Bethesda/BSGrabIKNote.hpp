#pragma once

#include "BSAnimNote.hpp"

class BSGrabIKNote : public BSAnimNote {
public:
	virtual ~BSGrabIKNote();

	enum Arm {
		LEFT	= 0,
		RIGHT	= 1,
		NONE	= 2,
	};

	enum State {
		START	= 1,
		STOP	= 2,
		NONE	= 3,
	};

	Arm		eArm;
	float	fGain;
	State	eState;

	NIRTTI_ADDRESS(0x11F3EF0);
};

ASSERT_SIZE(BSGrabIKNote, 0x1C);