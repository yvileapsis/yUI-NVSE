#pragma once

#include "TESPackage.hpp"
#include "Character.hpp"

class TESSound;
class TESTopicInfo;
class TESQuest;

struct DialogueResponse {
	BSStringT			responseText;	// 000
	UInt32				emotionType;	// 008
	SInt32				emotionValue;	// 00C
	BSStringT			voiceFileName;	// 010
	TESIdleForm*		speakerIdle;	// 018	idle
	TESIdleForm*		listenerIdle;	// 01C	idle
	TESSound*			sound;			// 020
	UInt32				unk024;			// 024	uninitialized, unused in xEdit
	UInt32				unk028;			// 028
};

struct DialogueItem {
	BSSimpleList<DialogueResponse>	responses;				// 000
	DialogueResponse*				currentResponse;	// 008
	TESTopicInfo*					currentTopicInfo;	// 00C
	TESTopic*						currentTopic;		// 010
	TESQuest*						currentQuest;		// 014
	TESForm*						currentSpeaker;		// 018
};

// 0D0
class DialoguePackage : public TESPackage {
public:
	DialoguePackage();
	~DialoguePackage();

	struct Data0080 {	// The constructor is referenced 169 time all over the place !
		UInt32	unk000;		//	000
		UInt8	byt004;		//	004
		UInt8	pad005[3];	//	005-007
		UInt32	unk008;		//	008
	};	//	00C

	Data0080	data0080;		// 080
	TESTopic*	topic;		// 08C
	Float32		flt090;			// 090
	Character*	speaker;		// 094
	UInt8		unk098;			// 098
	UInt8		unk099;			// 098
	UInt8		unk09A;			// 098
	UInt8		unk09B;			// 098
	TESForm*	unk09C;		// 09C
	UInt32		unk0A0;			// 0A0
	void*		unk0A4;			// 0A4	list of Dialogue Item and Dialogue Response, plus current item and current response
	UInt32		unk0A8;			// 0A8
	UInt32		unk0AC;			// 0AC
	Character*	subject;		// 0B0
	Character*	target;		// 0B4
	TESForm*	unk0B8;		// 0B8
	UInt8		unk0BC;			// 0BC
	UInt8		unk0BD;			// 0BD
	UInt8		unk0BE;			// 0BE
	UInt8		unk0BF;			// 0BF
	UInt8		unk0C0;			// 0C0
	UInt8		unk0C1;			// 0C1
	UInt8		unk0C2;			// 0C2
	UInt8		unk0C3;			// 0C3
	UInt32		unk0C4;			// 0C4
	UInt32		unk0C8;			// 0C8
	UInt8		unk0CC;			// OCC
	UInt8		unk0CD[3];		// OCD
};	// 0D0