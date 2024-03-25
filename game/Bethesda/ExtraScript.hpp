#pragma once
#include "BSExtraData.hpp"

struct ScriptEventList;
class TESObjectREFR;

// 0x14
class ExtraScript : public BSExtraData {
public:
	ExtraScript();
	virtual ~ExtraScript();

	Script* script;		// 00C
	ScriptEventList* eventList;	// 010

	static ExtraScript* Create(TESForm* baseForm = NULL, bool create = true, TESObjectREFR* container = NULL);
	void EventCreate(UInt32 eventCode, TESObjectREFR* container);
};
static_assert(sizeof(ExtraScript) == 0x14);