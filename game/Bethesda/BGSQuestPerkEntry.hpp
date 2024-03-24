#pragma once

#include "BGSPerkEntry.hpp"

class TESQuest;

class BGSQuestPerkEntry : public BGSPerkEntry {
public:
	BGSQuestPerkEntry();
	~BGSQuestPerkEntry();

	TESQuest* quest;				// 08
	UInt8				stage;				// 0C
	UInt8				pad[3];				// 0D
};