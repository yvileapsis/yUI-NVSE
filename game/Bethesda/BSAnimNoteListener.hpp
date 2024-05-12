#pragma once

#include "NiTArray.hpp"

class BSAnimNoteListener {
public:
	struct BSAnimReceiverType {
		UInt32	eNoteType;
		void*	pReceiver;
	};

	NiTPrimitiveArray<BSAnimNoteListener::BSAnimReceiverType*> Receivers;
};