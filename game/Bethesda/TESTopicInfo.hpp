#pragma once

#include "TESForm.hpp"
#include "TESCondition.hpp"
#include "NiTArray.hpp"

class TESTopic;

class TESTopicInfo : public TESForm {
public:
	TESTopicInfo();
	~TESTopicInfo();

	struct RelatedTopics
	{
		BSSimpleList<TESTopic*>	linkFrom;
		BSSimpleList<TESTopic*>	choices;
		BSSimpleList<TESTopic*>	unknown;
	};

	TESCondition			conditions;			// 18
	UInt16					unk20;				// 20
	UInt8					saidOnce;			// 22
	UInt8					type;				// 23
	UInt8					nextSpeaker;		// 24
	UInt8					flags1;				// 25
	UInt8					flags2;				// 26
	UInt8					pad27;				// 27
	BSStringT				prompt;				// 28
	BSSimpleList<TESTopic*>	addTopics;			// 30
	RelatedTopics*			relatedTopics;		// 38
	UInt32					speaker;			// 3C
	UInt32					actorValueOrPerk;	// 40
	UInt32					speechChallenge;	// 44
	UInt32					unk48;				// 48
	UInt32					modInfoFileOffset;	// 4C	during LoadForm
	TESTopic*				parentTopic; // Modded
};

ASSERT_SIZE(TESTopicInfo, 0x50 + 4);

typedef NiTArray<TESTopicInfo*> TopicInfoArray;
typedef void* INFO_LINK_ELEMENT;