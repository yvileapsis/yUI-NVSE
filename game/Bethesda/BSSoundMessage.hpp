#pragma once

#include "BSMemObject.hpp"
#include "NiPoint3.hpp"
#include "NiRefObject.hpp"
#include "NiTPointerList.hpp"

class BSSoundMessage : public BSMemObject {
public:
	BSSoundMessage();
	~BSSoundMessage();

	UInt32			uiType;
	UInt32			uiID;
	SInt32			iData;
	BSSoundMessage*	pNext;
	NiRefObjectPtr	spData;
	union {
		float	fVector[3];
		UInt16	usCurve[5];
	};

	BSSoundMessage operator=(const BSSoundMessage& arCopy);
};

ASSERT_SIZE(BSSoundMessage, 0x20);

typedef NiTPointerList<BSSoundMessage*> SoundMessageList;