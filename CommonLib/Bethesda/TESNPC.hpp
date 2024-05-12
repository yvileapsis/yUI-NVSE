#pragma once
#include "TESActorBase.hpp"
#include "TESRaceForm.hpp"

class BSFaceGenNiNode;
class TESClass;
class BGSHeadPart;
class FaceGenUndo;

// 0x20C
class TESNPC :
	public TESActorBase,
	public TESRaceForm
{
public:
	TESNPC();
	virtual ~TESNPC();

	enum eFaceGenDataType {
		kFaceGenData_GeometrySymetric = 0,
		kFaceGenData_GeometryAsymetric = 0,
		kFaceGenData_TextureSymetric = 0,
	};

	// 20 - facegen data
	struct Data
	{
		void* unk000;
		void* unk004;	// used as this owning unk000.
		UInt32	unk008;
		void* unk00C;	// the last three looks a lot like an array.
		UInt32	unk010;
		UInt32	unk014;
		UInt32 unk018[(0x058 - 0x018) >> 2];	// used as an array of 88 bytes
	};	// the owner of the array is FaceGenData::Unk008

	struct FaceGenData
	{						// sample Sunny Smiles
		UInt32	unk00;		// 000 134 filled		filled			filled
		void* unk04;	// 004 138 null			null			null
		UInt32	unk08;		// 008 13C null			null			null
		float** values;	// 00C 140 filled		filled			filled			base for offset to data. Looks like an array of arrays.
		UInt32	useOffset;	// 010 144 filled +0xC8	filled +0x78	filled +0xC8	participate in array base calculation
		UInt32	maxOffset;	// 014 148 = unk144		= unk144		= unk144
		UInt32  count;		// 018 14C 0x32			0x1e			0x32
		UInt32  size;		// 01C 150 0x01			0x01			0x01			also participate in array offset calculation (offset = count * size)
	};

	UInt8		skillValues[0x0E];				// 114/148
	UInt8		skillOffsets[0x0E];				// 122
	TESClass* classID;						// 130/164
	FaceGenData	faceGenData[3];					// 134/180	runtime suggests there are 2 arrays of 2 FaceGenData. the last one being null.
	UInt32		pad194[(0x1A4 - 0x194) >> 2];		// 194
	UInt32		unk1A4;
	UInt32		unk1A8;
	UInt32		unk1AC;
	UInt32		unk1B0;							// 1B0
	void* unk1B4;						// 1B4	pointer to 84 bytes struct related to Face, can be returned instead of faceGenData so identical or at least starts the same ?
	TESHair* hair;							// 1B8/1EC
	float		hairLength;						// 1BC/1F0
	TESEyes* eyes;							// 1C0/1F4
	BSFaceGenNiNode* unk1C4;					// 1C4
	BSFaceGenNiNode* unk1C8;					// 1C8
	UInt32		unk1CC;							// 1CC
	UInt16		unk1D0;							// 1D0
	UInt16		unk1D2;							// 1D2
	TESCombatStyle* unk1D4;					// 1D4
	UInt32		hairColor;						// 1D8/20C
	BSSimpleList<BGSHeadPart*>	headPart;				// 1DC/210
	UInt32		impactMaterialType;				// 1E4/218
	UInt32	  unk01E8;						// 1E8
	TESRace* race1EC;						// 1EC
	TESNPC* copyFrom;						// 1F0		not set once PlayerRef exists and the target is the Player
	float		height;							// 1F4/21C
	float		weight;							// 1F8/220 Aparently, getWeight purposly returns height except for the player.
	NiTPrimitiveArray<FaceGenUndo*>	faceGenUndo;	// 1FC
};
static_assert(sizeof(TESNPC) == 0x20C);