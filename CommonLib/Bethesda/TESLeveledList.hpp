#pragma once
#include "BaseFormComponent.hpp"
#include "BSExtraData.hpp"
#include "ExtraDataList.hpp"
#include "BSSimpleList.hpp"

class TESObjectREFR;
class ContainerExtraData;
class TESGlobal;

// 0x1C
class TESLeveledList : public BaseFormComponent {
public:
	virtual UInt8 GetChanceNone();
	virtual void GetCalcEachInCount();
	virtual void Func0006();

	struct LoadBaseData	// as used by LoadForm
	{
		SInt16			level;		// 000
		UInt16			fill002;	// 002
		TESObjectREFR*	refr;		// 004
		SInt16			count;		// 008
		UInt16			fill00A;	// 00A
	};	// 00C

	struct BaseData
	{
		TESObjectREFR*		pkObject;				// 000
		SInt16				usCount;				// 004
		SInt16				usLevel;				// 006
		ContainerExtraData* pkContainerExtraData;	// 008
	};	// 00C

	BSSimpleList<BaseData*>		kDataList;	// 004
	UInt8						cChanceNone;
	UInt8						cFlags;
	UInt16						pad00E;
	TESGlobal*					pkGlobal;
	BSSimpleList<BSExtraData*>	kExtraDataList; // TODO: check if it's ExtraDataList
};
static_assert(sizeof(TESLeveledList) == 0x1C);