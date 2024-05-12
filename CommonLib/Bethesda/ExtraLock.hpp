#pragma once
#include "BSExtraData.hpp"

class TESKey;

class ExtraLock : public BSExtraData {
public:
	ExtraLock();
	virtual ~ExtraLock();

	enum EnumLockLevel : UInt32
	{
		kLockLevel_VeryEasy		= 0x0,
		kLockLevel_Easy			= 0x1,
		kLockLevel_Normal		= 0x2,
		kLockLevel_Hard			= 0x3,
		kLockLevel_VeryHard		= 0x4,
		kLockLevel_Impossible	= 0x5,
	};

	enum EnumLockFlags : UInt8
	{
		kLocked = 1
	};

	struct Data
	{
		ELockLevel	lockLevel;	// 00
		TESKey*		key;		// 04
		ELockFlags	flags;		// 08
		UInt8		pad[3];
		UInt32		unk0C;		// 0C introduced since form version 0x10
		UInt32		unk10;		// 10
	};

	Data* data;		// 00C

	static ExtraLock* Create();
};
static_assert(sizeof(ExtraLock) == 0x10);