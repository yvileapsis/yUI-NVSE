#pragma once
#include "TESForm.hpp"
#include "TESDescription.hpp"
#include "TESIcon.hpp"
#include "ActorValue.hpp"

// 0xC4
class ActorValueInfo :
	public TESForm,						// 00
	public TESFullName,					// 18
	public TESDescription,				// 24
	public TESIcon						// 2C
{
public:
	enum ActorValueInfoFlags
	{
		kAVInfo_Unk1		= 0x1,		// used in list of modified ActorValue for Player and others. Either can damage or "special damage", see 0x00937280
		kAVInfo_CanModify	= 0xE,
	};

	char*				infoName;		// 38
	BSStringT			avName;			// 3C
	ActorValueInfoFlags	eFlags;			// 44
	UInt32				unk48;			// 48
	UInt32				initDefaultCallback;		// 4C
	UInt32				unk50;			// 50
	UInt32				onChangeCallback;		// 54
	UInt32				numLinkedActorValues;
	EnumActorValueCode	eLinkedActorValues[15];
	UInt32				numUnk9Cs;
	UInt32				unk9C[10];
};
static_assert(sizeof(ActorValueInfo) == 0xC4);

extern const ActorValueInfo** ActorValueInfoPointerArray;

typedef ActorValueInfo* (*_GetActorValueInfo)(UInt32 actorValueCode);
extern const _GetActorValueInfo GetActorValueInfo;