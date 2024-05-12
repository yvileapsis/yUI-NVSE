#pragma once
#include "BGSPerkEntry.hpp"

class TESQuest;

// 0x10
class BGSQuestPerkEntry : public BGSPerkEntry
{
public:
	BGSQuestPerkEntry();
	~BGSQuestPerkEntry();

	TESQuest*			pkQuest;			// 08
	UInt8				ucStage;			// 0C
	UInt8				pad[3];				// 0D
};
static_assert(sizeof(BGSQuestPerkEntry) == 0x10);