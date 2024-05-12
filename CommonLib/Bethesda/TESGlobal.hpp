#pragma once
#include "TESForm.hpp"

// 28
class TESGlobal : public TESForm
{
public:
	TESGlobal();
	~TESGlobal();

	enum EnumType : UInt8
	{
		kType_Short = 's',
		kType_Long = 'l',
		kType_Float = 'f'
	};

	BSStringT<char>	kName;		// 18
	EnumType		eType;		// 20
	UInt8			pad21[3];	// 21
	union {
		SInt32		iData;
		UInt32		uiData;
		SInt16		sData;
		UInt16		usData;
		float		fData;
	};							// 24
};
static_assert(sizeof(TESGlobal) == 0x28);