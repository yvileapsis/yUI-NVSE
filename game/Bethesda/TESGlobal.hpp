#pragma once

#include "TESForm.hpp"

class TESGlobal : public TESForm {
public:
	TESGlobal();
	~TESGlobal();

	enum
	{
		kType_Short = 's',
		kType_Long = 'l',
		kType_Float = 'f'
	};

	BSStringT	strName;
	UInt8		ucType;
	union {
		SInt32	iData;
		UInt32	uiData;
		SInt16	sData;
		UInt16	usData;
		float	fData;
	};

};

ASSERT_SIZE(TESGlobal, 0x28);