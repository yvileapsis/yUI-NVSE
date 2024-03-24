#pragma once

#include "BSExtraData.hpp"

struct ScriptEventList;
class TESObjectREFR;

class ExtraScript : public BSExtraData {
public:
	ExtraScript();
	virtual ~ExtraScript();

	EXTRADATATYPE(SCRIPT);

	Script* script;		// 00C
	ScriptEventList* eventList;	// 010

	static ExtraScript* Create(TESForm* baseForm = NULL, bool create = true, TESObjectREFR* container = NULL);
	void EventCreate(UInt32 eventCode, TESObjectREFR* container);
};