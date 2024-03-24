#pragma once

#include "TESForm.hpp"
#include "BSSimpleArray.hpp"
#include "NiTArray.hpp"
#include "TESFullName.hpp"
#include "TESTopicInfo.hpp"

class TESQuest;
class TESTopicInfo;

class TESTopic : public TESForm {
public:
	TESTopic();
	~TESTopic();

	struct Info	//	34
	{
		TESQuest* quest;		//	00
		TopicInfoArray	infoArray;	//	04
		BSSimpleArray<INFO_LINK_ELEMENT*>		unk01C;
		TESQuest* quest2;		//	2C
		UInt8			unk030;
		UInt8			pad031[3];
	};

	TESFullName		fullName;	// 18

	UInt8			type;			// 24	DATA
	UInt8			flags;			// 25	DATA also used as bool or flag, connected to INFOGENERAL
	UInt8			pad26[2];		// 26
	float			priority;		// 28	PNAM
	BSSimpleList<Info>	infos;			// 2C
	BSStringT			unk34;			// 34	TDUM
	UInt16			unk3C;			// 3C	XIDX
	UInt16			unk3E;			// 3E
};

ASSERT_OFFSET(TESTopic, fullName, 0x018);