#pragma once
#include "TESForm.hpp"
#include "BSSimpleArray.hpp"
#include "NiTArray.hpp"
#include "TESFullName.hpp"
#include "TESTopicInfo.hpp"

class TESQuest;
class TESTopicInfo;

// 40
class TESTopic :
	public TESForm,
	public TESFullName
{
public:
	TESTopic();
	~TESTopic();

	struct Info	//	34
	{
		TESQuest*							pkQuest;	//	00
		NiTPrimitiveArray<TESTopicInfo*>	kInfoArray;	//	04 // TODO: confirm which NiTArray is used here
		BSSimpleArray<void*>				unk01C;
		TESQuest*							quest2;		//	2C
		UInt8								unk030;
		UInt8								pad031[3];
	};

	UInt8				ucType;			// 24	DATA
	UInt8				ucFlags;		// 25	DATA also used as bool or flag, connected to INFOGENERAL
	UInt8				pad26[2];		// 26
	Float32				fPriority;		// 28	PNAM
	BSSimpleList<Info*>	kInfos;			// 2C
	BSStringT<char>		unk34;			// 34	TDUM
	UInt16				unk3C;			// 3C	XIDX
	UInt16				unk3E;			// 3E
};
static_assert(sizeof(TESTopic) == 0x40);