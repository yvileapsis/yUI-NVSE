#pragma once
#include "NiTArray.hpp"
#include "NiTLargeArray.hpp"
#include "TESObjectREFR.hpp"

struct ChangesMap;
struct InteriorCellNewReferencesMap;
struct ExteriorCellNewReferencesMap;
struct NumericIDBufferMap;

class TESSaveLoadGame {
public:
	TESSaveLoadGame();
	~TESSaveLoadGame();

	struct CreatedObject
	{
		UInt32 refID;
		CreatedObject* next;
	};

	ChangesMap* changesMap;						// 000
	UInt32 unk004;								// 004
	InteriorCellNewReferencesMap* intRefMap;	// 008
	ExteriorCellNewReferencesMap* extRefMap00C; // 00C
	ExteriorCellNewReferencesMap* extRefMap010; // 010
	void* saveLoadBuffer;						// 014
	UInt32 unk018;								// 018
	UInt8 unk01C;								// 01C
	UInt8 pad01D[3];
	NiTLargePrimitiveArray<TESObjectREFR*>* arr020; // 020
	UInt32 unk024;					   // 024
	UInt32 unk028;					   // 028
	CreatedObject createdObjecBSSimpleList;   // 02C data is formID - verified
	NiTPointerMap<void*, void*>* map034;	   // 034
	UInt32 unk034[(0x58 - 0x44) >> 2]; // 044
	NumericIDBufferMap* idMap058;	   // 058
	NumericIDBufferMap* idMap05C;	   // 05C
	NumericIDBufferMap* idMap060;	   // 060
	NumericIDBufferMap* idMap064;	   // 064
	UInt32 unk068;					   // 068
	UInt32 unk06C;					   // 06C
	UInt32 unk070;					   // 070
	UInt8 unk074;					   // 074
	UInt8 unk075;					   //	 init to 0x7D
	UInt8 pad076[2];
	NiTLargePrimitiveArray<UInt32>*		array078; // 078 NiTLargePrimitiveArray<?>
	NiTLargePrimitiveArray<UInt32>*		array07C; // 07C NiTLargePrimitiveArray<?>
	UInt8 unk080;				// 080 version of save?
	UInt8 unk081;
	UInt8 pad082[2];
	UInt32 unk084[(0xAC - 0x84) >> 2];	 // 084
	UInt8 unk0AC;						 // 0AC
	UInt8 unk0AD;						 // 0AD
	UInt8 unk0AE;						 // 0AE
	UInt8 unk0AF;						 // 0AF
	UInt32 unk0B0[(0x1C8 - 0x0B0) >> 2]; // 0B0

	static TESSaveLoadGame* GetSingleton();

	void AddCreatedForm(TESForm* pForm) { ThisCall(0x00861780, this, pForm); }
};