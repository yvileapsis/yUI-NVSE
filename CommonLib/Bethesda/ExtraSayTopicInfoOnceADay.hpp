#pragma once

#include "BSExtraData.hpp"
#include "BSSimpleList.hpp"

class TESTopicInfo;
struct SayOnceTopicInfos;

class ExtraSayTopicInfoOnceADay : public BSExtraData {
public:
	EXTRADATATYPE(SAYTOPICINFOONCEADAY);

	BSSimpleList<SayOnceTopicInfos*>* pSayOnceInfos;
};

ASSERT_SIZE(ExtraSayTopicInfoOnceADay, 0x10);

struct SayOnceTopicInfos {
	TESTopicInfo*	pInfo;
	SInt32			iDateStamp;
	float			fHourStamp;

	SInt32 GetDay() const { return iDateStamp & 0x1FF; }
	void SetDay(SInt32 day) { iDateStamp = (iDateStamp & ~0x1FF) | (day & 0x1FF); }

	SInt32 GetMonth() const { return (iDateStamp >> 9) & 15; }
	void SetMonth(SInt32 month) { iDateStamp = (iDateStamp & ~0x1E00) | ((month & 15) << 9); }

	SInt32 GetYear() const { return (iDateStamp >> 13) & 0x3FFF; }
	void SetYear(SInt32 year) { iDateStamp = (iDateStamp & ~0x7FFE000) | ((year & 0x3FFF) << 13); }
};

ASSERT_SIZE(SayOnceTopicInfos, 0xC);