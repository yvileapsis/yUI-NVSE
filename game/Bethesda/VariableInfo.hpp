#pragma once
#include "BSString.hpp"
#include "ScriptEventList.hpp"

// 0x20
struct VariableInfo
{
	enum EnumType : UInt8
	{
		eVarType_Float = 0, // ref is also zero
		eVarType_Integer,

		// NVSE, return values only
		eVarType_String,
		eVarType_Array,
		eVarType_Ref,

		eVarType_Invalid
	};

	UInt32			uiID;		// 00
	UInt32			pad04;		// 04
	Float64			dValue;		// 08
	EnumType		eType;		// 10
	UInt8			pad11[3];	// 11
	UInt32			unk14;		// 14
	BSStringT<char>	kName;		// 18

	ScriptLocal* Resolve(ScriptEventList* evenBSSimpleList);
	TESForm* GetTESForm();
	bool IsReferenceType(Script* parentScript);
};
static_assert(sizeof(VariableInfo) == 0x20);