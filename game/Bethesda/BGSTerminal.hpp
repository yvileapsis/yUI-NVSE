#pragma once

#include "TESObjectACTI.hpp"
#include "TESCondition.hpp"

class BGSNote;
struct ScriptEventList;

class BGSTerminal : public TESObjectACTI {
public:
	BGSTerminal();
	~BGSTerminal();

	enum
	{
		kTerminalFlagLeveled = 1 << 0,
		kTerminalFlagUnlocked = 1 << 1,
		kTerminalFlagAltColors = 1 << 2,
		kTerminalFlagHideWelcome = 1 << 3,
	};

	enum
	{
		kEntryFlagAddNote = 1 << 0,
		kEntryFlagForceRedraw = 1 << 1,
	};

	struct TermData
	{
		UInt8 difficulty;       // 0: very easy, 1: easy, 2: average, 3: hard, 4: very hard, 5: requires key
		UInt8 terminalFlags;
		UInt8 type;             // 0-9, corresponds to GECK types 1-10
	};

	struct MenuEntry
	{
		BSStringT				entryText;
		BSStringT				resultText;
		UInt8				entryFlags;
		BGSNote* displayNote;
		BGSTerminal* subMenu;
		ScriptEventList* ScriptEventList;
		TESCondition	conditions;
	};

	BSStringT				desc;			// 090	DESC
	BSSimpleList<MenuEntry>	menuEntries;	// 098
	BGSNote* password;		// 0A0	PNAM
	TermData			data;			// 0A4	DNAM
};