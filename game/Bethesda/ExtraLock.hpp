#pragma once

#include "BSExtraData.hpp"

class TESKey;

class ExtraLock : public BSExtraData {
public:
	ExtraLock();
	virtual ~ExtraLock();

	struct Data
	{
		UInt32	lockLevel;	// 00
		TESKey* key;		// 04
		UInt8	flags;		// 08
		UInt8	pad[3];
		UInt32  unk0C;		// 0C introduced since form version 0x10
		UInt32	unk10;		// 10
	};

	Data* data;		// 00C

	static ExtraLock* Create();
};