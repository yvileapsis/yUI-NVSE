#pragma once

#include "LowProcess.hpp"

class MiddleLowProcess : public LowProcess {
public:
	MiddleLowProcess();
	~MiddleLowProcess();

	virtual void	Unk_207();

	UInt32				unk0B4;			// 0B4
	ActorValueList		tempModifiers;	// 0B8
};	// 0C8