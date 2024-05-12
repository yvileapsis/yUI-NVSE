#pragma once

#include "BaseFormComponent.hpp"
#include "BSExtraData.hpp"
#include "BSSimpleList.hpp"

class TESObjectREFR;
class ContainerExtraData;
class TESGlobal;

class TESLeveledList : public BaseFormComponent {
public:
	struct LoadBaseData	// as used by LoadForm
	{
		SInt16			level;		// 000
		UInt16			fill002;	// 002
		TESObjectREFR* refr;		// 004
		SInt16			count;		// 008
		UInt16			fill00A;	// 00A
	};	// 00C

	struct BaseData
	{
		TESObjectREFR* refr;		// 000
		SInt16				count;		// 004
		SInt16				level;		// 006
		ContainerExtraData* coed;		// 008
	};	// 00C

	BSSimpleList<BaseData*>	datas;			// 004
	UInt8 chanceNone;
	UInt8 flags;
	UInt16 pad00E;
	TESGlobal* global;
	BSSimpleList<BSExtraData*> extraDatas;
};	// 01C
ASSERT_SIZE(TESLeveledList, 0x1C);