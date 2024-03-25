#pragma once
#include "TESForm.hpp"

// 0x38
class BGSCameraPath : public TESForm
{
public:
	BSSimpleList<void*> list18;
	BSSimpleList<void*> pkCameraShots;
	UInt8				ucFlags;
	UInt8				gap29[3];
	UInt32				uiNiFormArray;
	BGSCameraPath*		pkParentCameraPath;
	BGSCameraPath*		pkPreviousSiblingCameraPath;
};
static_assert(sizeof(BGSCameraPath) == 0x38);