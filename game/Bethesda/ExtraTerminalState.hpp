#pragma once

#include "BSExtraData.hpp"

class ExtraTerminalState : public BSExtraData {
public:
	ExtraTerminalState();
	virtual ~ExtraTerminalState();

	enum
	{
		kFlag_Locked = 1 << 7,        // terminal is locked
	};

	UInt8	flags;
	UInt8	lockLevel;
	UInt8	filler[2];
};