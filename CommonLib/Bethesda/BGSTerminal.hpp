#pragma once
#include "TESObjectACTI.hpp"
#include "TESCondition.hpp"

class BGSNote;

// 0xA8
class BGSTerminal : public TESObjectACTI {
public:
	BGSTerminal();
	~BGSTerminal();

	enum
	{
		kTerminalFlagLeveled		= 1 << 0,
		kTerminalFlagUnlocked		= 1 << 1,
		kTerminalFlagAltColors		= 1 << 2,
		kTerminalFlagHideWelcome	= 1 << 3,
	};

	enum
	{
		kEntryFlagAddNote			= 1 << 0,
		kEntryFlagForceRedraw		= 1 << 1,
	};

	struct TermData
	{
		enum EnumLockLevel : UInt8
		{
			kLockLevel_VeryEasy		= 0x0,
			kLockLevel_Easy			= 0x1,
			kLockLevel_Normal		= 0x2,
			kLockLevel_Hard			= 0x3,
			kLockLevel_VeryHard		= 0x4,
			kLockLevel_RequiresKey	= 0x5,
		};

		EnumLockLevel	eDifficulty;
		UInt8			ucTerminalFlags;
		UInt8			ucType;			 // 0-9, corresponds to GECK types 1-10
	};

	struct MenuEntry
	{
		BSStringT			kEntryText;
		BSStringT			kResultText;
		UInt8				ucEntryFlags;
		BGSNote*			pkDisplayNote;
		BGSTerminal*		pkSubMenu;
		ScriptEventList*	pkScriptEventList;
		TESCondition		kConditions;
	};

	BSStringT					kDescription;		// 090	DESC
	BSSimpleList<MenuEntry*>	kMenuEntryList;		// 098
	BGSNote*					kPassword;			// 0A0	PNAM
	TermData					kData;				// 0A4	DNAM
};
static_assert(sizeof(BGSTerminal) == 0xA8);