#pragma once
#include "BSString.hpp"
#include "NiSmartPointer.hpp"

class TESIdleForm;

struct TESTopicInfoResponse {
	struct Data
	{
		UInt32	emotionType;	//	00
		UInt32	emotionValue;	//	04	Init'd to 0x32
		UInt32	unused;			//	08
		UInt8	responseNumber;	//	0C
		UInt8	pad00D[3];
		UInt32	sound;			//	10
		UInt8	flags;			//	14	Init'd to 1
		UInt8	pad015[3];
	};

	Data					data;					//	000
	BSStringT<char>			responseText;			//	018
	NiPointer<TESIdleForm>	spkSpeakerAnimation;	//	020
	NiPointer<TESIdleForm>	listenerAnimation;		//	024
	TESTopicInfoResponse*	next;					//	028
};