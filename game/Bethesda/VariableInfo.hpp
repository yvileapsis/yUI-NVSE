#pragma once

#include "BSString.hpp"
#include "ScriptEventList.hpp"

struct VariableInfo {
	UInt32			idx;		// 00
	UInt32			pad04;		// 04
	double			data;		// 08
	UInt8			type;		// 10
	UInt8			pad11[3];	// 11
	UInt32			unk14;		// 14
	BSStringT		name;		// 18

	ScriptLocal* Resolve(ScriptEventList* evenBSSimpleList);
	TESForm* GetTESForm();
	bool IsReferenceType(Script* parentScript);
};