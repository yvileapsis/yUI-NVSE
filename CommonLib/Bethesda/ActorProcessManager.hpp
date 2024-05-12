#pragma once

#include "tList.hpp"

class Actor;
class BSTempEffect;

// Straight from OBSE. Needs to be debugged ! ! !
// This is used all over the game code to manage actors and occassionally other objects.
class ActorProcessManager {
public:
	ActorProcessManager();
	~ActorProcessManager();

	struct ActorList {
		tList<Actor>			head;
		tList<Actor>::_Node* tail;
	};

	ActorList				middleHighActors;		// 00
	ActorList				lowActors0C;			// 0C significantly smaller list than lowActors18.
	//	  Plausible: actors currently scheduled for low processing
	ActorList				lowActors18;			// 18
	float					unk24;					// 24
	UInt32					unk28;					// 28
	void* unk2C;				// 2C
	UInt32					unk30;					// 30
	void* unk34;				// 34
	UInt32					unk38[0x0A];			// 38
	// almost confirmed to be at index 60
	tList<BSTempEffect>		tempEffects;			// 60
	// needs recalc of offsets
	UInt32					unk4C[4];				// 4C
	tList<Actor>			highActors;				// 5C
	Actor* actor64;				// 64
	tList<Actor>::_Node* unkNodes[3];			// 68 ##TODO: which lists do these belong to
	UInt32					unk74;					// 74 Possibly not a member. Definitely no more members following this.
};

extern ActorProcessManager* g_actorProcessManager;