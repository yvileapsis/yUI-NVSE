#pragma once
#include "TESBoundObject.hpp"
#include "TESModel.hpp"
#include "TESIcon.hpp"
#include "BGSPickupPutdownSounds.hpp"
#include "BSSimpleList.hpp"

class TESDescription;

// 0x80
class BGSNote :
	public TESBoundObject,			// 000
	public TESModel,				// 030
	public TESFullName,				// 048
	public TESIcon,					// 054
	public BGSPickupPutdownSounds	// 060
{
public:
	BGSNote();
	~BGSNote();

	// bases
	TESDescription*		pkNoteText;	// 06C
	TESNPC*				pkVoiceNPC;	// 070
	BSSimpleList<void*> kQuestList;	// 074
	UInt8				ucNoteType;	// 07C
	UInt8				ucRead;
	UInt8				byte7E;
	UInt8				byte7F;
};
static_assert(sizeof(BGSNote) == 0x80);