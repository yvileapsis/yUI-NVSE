#pragma once
#include "ExtraSeenData.hpp"

class ExtraIntSeenData : public ExtraSeenData {
public:
	ExtraIntSeenData();
	virtual ~ExtraIntSeenData();

	UInt8				coordX;		// 024
	UInt8				coordY;		// 025
	UInt8				filler[2];	// 026
	ExtraIntSeenData*	next;		// 028
};
static_assert(sizeof(ExtraIntSeenData) == 0x2C);