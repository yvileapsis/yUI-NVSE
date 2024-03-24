#pragma once

#include "BSExtraData.hpp"
#include "BSSimpleList.hpp"
#include "BSPointer.hpp"

class TESObjectREFR;
class ExtraRefractionProperty;
class ExtraSayTopicInfoOnceADay;

class ExtraDataList : public BaseExtraList {
public:
	static ExtraDataList* Create(BSExtraData* xBSData = NULL);

	BSSimpleList<BSPointer<TESObjectREFR>*>* GetReflectedRefs() const;
	ExtraRefractionProperty* GetRefractionProperty() const;
	ExtraSayTopicInfoOnceADay* GetExtraSayTopicInfoOnceADayExtra() const;
};

ASSERT_SIZE(ExtraDataList, 0x20);