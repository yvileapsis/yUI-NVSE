#include "Containers.h"
#include "NiTypes.h"
#include "Utilities.hpp"

// Form type class: use to preload some information for created objects (?) refr and Cells
struct formTypeClassData
{
	typedef UInt8 EncodedID[3];	// Codes the refID on 3 bytes, as used in changed forms and save refID mapping

	struct Data01 // Applies to CELL where changeFlags bit30 (Detached CELL) and bit29 (CHANGE_CELL_EXTERIOR_CHAR) are set
	{
		UInt16	worldspaceIndex;	// 00 Index into visitedWorldspaces		goes into unk000
		UInt8	coordX;				// 02	goes into unk004
		UInt8	coordY;				// 03	goes into unk008, paired with 002
		UInt8	detachTime;			// 04	goes into unk00C
	};

	struct Data02 // Applies to CELL where changeFlags bit30 (Detached CELL) and bit 28 (CHANGE_CELL_EXTERIOR_SHORT) are set and changeFlags bit29 is clear
	{
		UInt16	worldspaceIndex;	// 00 Index into visitedWorldspaces		goes into unk000
		UInt16	coordX;				// 02	goes into unk004
		UInt16	coordY;				// 03	goes into unk008, paired with 002
		UInt32	detachTime;			// 04	goes into unk00C
	};

	// The difference between the two preceding case seems to be how big the data (coordinates?) are

	struct Data03 // Applies to CELL where changeFlags bit30 (Detached CELL) is set and changeFlags bit28 and bit29 are clear
	{
		UInt32	detachTime;	// 00	goes into unk00C. Null goes into unk000, 004 and 008
	};

	struct Data04 // Applies to references where changeFlags bit3 (CHANGE_REFR_CELL_CHANGED) is clear and
					// either bit1 (CHANGE_REFR_MOVE) or bit2 (CHANGE_REFR_HAVOK_MOVE) is set
	{
		EncodedID	cellOrWorldspaceID;	// 000	goes into unk000, Null goes into unk004, 008, 00C, 010 and byt02C
		float		posX;	// 003	goes into unk014
		float		posY;	// 007	goes into unk018, associated with unk003
		float		posZ;	// 00B	goes into unk01C, associated with unk003	(pos?)
		float		rotX;	// 00F	goes into unk020
		float		rotY;	// 013	goes into unk024, associated with unk00F
		float		rotZ;	// 017	goes into unk028, associated with unk00F	(rot?)
	};

	struct Data05 // Applies to created objects (ie 0xFFnnnnnn)
	{
		EncodedID	cellOrWorldspaceID;	// 000	goes into unk000
		float		posX;	// 003	goes into unk014
		float		posY;	// 007	goes into unk018, associated with unk003
		float		posZ;	// 00B	goes into unk01C, associated with unk003	(pos?)
		float		rotX;	// 00F	goes into unk020
		float		rotY;	// 013	goes into unk024, associated with unk024
		float		rotZ;	// 017	goes into unk028, associated with unk028	(rot?)
		UInt8		flags;	// 01B	goes into unk02C	bit0 always set, bit1 = ESP or persistent, bit2 = Byt081 true
		EncodedID	baseFormID;	// 01C	goes into unk004, Null goes into unk008, 00C and 010
	};

	struct Data06 // Applies to references whose changeFlags bit3 (CHANGE_REFR_CELL_CHANGED) is set
	{
		EncodedID	cellOrWorldspace;		// 000	goes into unk000
		float		posX;					// 003	goes into unk014
		float		posY;					// 007	goes into unk018, associated with unk003
		float		posZ;					// 00B	goes into unk01C, associated with unk003	(pos?)
		float		rotX;					// 00F	goes into unk020
		float		rotY;					// 013	goes into unk024, associated with unk00F
		float		rotZ;					// 017	goes into unk028, associated with unk00F	(rot?)
		EncodedID	newCellOrWorldspaceID;	// 01C	goes into unk008
		SInt16		coordX;					// 01E	goes into unk00C
		SInt16		coordY;					// 020	goes into unk010, Null goes into unk004 and byt02C
	};

	struct Data00  // Every other cases (no data)
	{
	};

	union Data
	{
		Data00	data00;
		Data01	data01;
		Data02	data02;
		Data03	data03;
		Data04	data04;
		Data05	data05;
		Data06	data06;
	};

	Data data;	// 00
};

struct PreloadCELLdata	// Unpacked and decoded version of Data01, 02 end 03
{
	UInt32	worldspaceID;	// 000
	SInt32	coordX;			// 004
	SInt32	coordY;			// 008
	UInt32	detachTime;		// 00C
};

struct PreloadREFRdata	// Unpacked and decoded version of Data04, 05 and 06
{
	UInt32	cellOrWorldspaceID;		// 000
	UInt32	baseFormID;				// 004
	UInt32	newCellOrWorldspaceID;	// 008
	SInt32	coordX;					// 00C
	SInt32	coordY;					// 010
	float	posXcoordX;				// 014
	float	posYcoordY;				// 018
	float	posZ;					// 01C
	float	rotX;					// 020
	float	rotY;					// 024
	float	rotZ;					// 028
	UInt8	flg02C;					// 02C
};

union preloadData
{
	PreloadCELLdata	cell;
	PreloadREFRdata	refr;
};

class BGSLoadGameBuffer
{
	BGSLoadGameBuffer();
	~BGSLoadGameBuffer();

	virtual UInt8			GetSaveFormVersion();	// replaced in descendant 
	virtual TESForm* getForm();				// only implemented in descendants
	virtual TESObjectREFR* getREFR();				// only implemented in descendants
	virtual Actor* getActor();				// only implemented in descendants

	char* chunk;			// 004
	UInt32	chunkSize;		// 008
	UInt32	chunkConsumed;	// 00C
};

struct BGSFormChange
{
	UInt32	changeFlags;
	UInt32	unk004;			// Pointer to the changed record or the save record ?
};

struct	BGSSaveLoadChangesMap
{
	NiTPointerMap<BGSFormChange> BGSFormChangeMap;
	// most likely more
};

// 030
class BGSLoadFormBuffer : public BGSLoadGameBuffer
{
	BGSLoadFormBuffer();
	~BGSLoadFormBuffer();

	typedef UInt8 EncodedID[3];
	struct Header	// 00C
	{
		EncodedID	encodeID;			// 00
		UInt32		changeFlags;		// 03
		UInt8		codedTypeAndLength;	// 07
		UInt8		formVersion;		// 08
		UInt8		pad009[3];			// 09
	};

	UInt32			refID;				// 010
	Header			header;				// 014
	UInt32			bufferSize;			// 020
	TESForm* form;				// 024
	UInt32			flg028;				// 028	bit1 form invalid
	BGSFormChange* currentFormChange;	// 02C
};

class BGSSaveFormBuffer : public BGSLoadGameBuffer
{
	BGSSaveFormBuffer();
	~BGSSaveFormBuffer();

};	// in BGSSaveGameBuffer there is a 010, which look like a counter (ChunkCount ?), then the Header

// 1C8 - only explicitly marked things are verified
class TESSaveLoadGame
{
public:
	TESSaveLoadGame();
	~TESSaveLoadGame();

	struct CreatedObject {
		UInt32			refID;
		CreatedObject* next;
	};

	ChangesMap* changesMap;			// 000
	UInt32							unk004;				// 004
	InteriorCellNewReferencesMap* intRefMap;			// 008
	ExteriorCellNewReferencesMap* extRefMap00C;		// 00C
	ExteriorCellNewReferencesMap* extRefMap010;		// 010
	void* saveLoadBuffer;		// 014
	UInt32							unk018;				// 018
	UInt8							unk01C;				// 01C
	UInt8							pad01D[3];
	NiTArray<TESObjectREFR*>* arr020;				// 020
	UInt32							unk024;				// 024
	UInt32							unk028;				// 028
	CreatedObject					createdObjectList;	// 02C data is formID - verified
	NiTPointerMap<void*>* map034;				// 034
	UInt32							unk034[(0x58 - 0x44) >> 2];	// 044
	NumericIDBufferMap* idMap058;			// 058
	NumericIDBufferMap* idMap05C;			// 05C
	NumericIDBufferMap* idMap060;			// 060
	NumericIDBufferMap* idMap064;			// 064
	UInt32							unk068;				// 068
	UInt32							unk06C;				// 06C
	UInt32							unk070;				// 070
	UInt8							unk074;				// 074
	UInt8							unk075;				//     init to 0x7D
	UInt8							pad076[2];
	NiTArray<UInt32>*				array078;			// 078 NiTLargePrimitiveArray<?>
	NiTArray<UInt32>*				array07C;			// 07C NiTLargePrimitiveArray<?>	
	UInt8							unk080;				// 080 version of save?
	UInt8							unk081;
	UInt8							pad082[2];
	UInt32							unk084[(0xAC - 0x84) >> 2];	// 084
	UInt8							unk0AC;				// 0AC
	UInt8							unk0AD;				// 0AD
	UInt8							unk0AE;				// 0AE
	UInt8							unk0AF;				// 0AF
	UInt32							unk0B0[(0x1C8 - 0x0B0) >> 2];	// 0B0

	static TESSaveLoadGame* Get() { return reinterpret_cast<TESSaveLoadGame*>(0x011DE45C); }

	void AddCreatedForm(TESForm* pForm) { ThisCall(0x00861780, this, pForm); }
};


class BGSSaveLoadGame	// 0x011DDF38
{
public:
	BGSSaveLoadGame();
	~BGSSaveLoadGame();

	typedef UInt32	RefID;
	typedef UInt32	IndexRefID;
	struct RefIDIndexMapping	// reversible map between refID and loaded form index
	{
		NiTMap<RefID, IndexRefID>*	map000;	// 000
		NiTMap<IndexRefID, RefID>*	map010;	// 010
		UInt32			            countRefID;	// 020
	};

	struct SaveChapters	// 06E	chapter table in save
	{
		struct RefIDArray	// List of all refID referenced in save for tranlation in RefIDIndexMapping
		{
			UInt32	count;	// 000
			RefID	IDs[1];	// 004
		};

		RefIDArray* arr000;	// 000
		RefIDArray* arr004;	// 004

	};

	struct Struct010
	{
		NiTPointerMap<UInt32>* map000;	// 000
		BGSCellNumericIDArrayMap* map010;	// 010
		NiTPointerMap<BGSCellNumericIDArrayMap*>* map020;	// 020
	};

	BGSSaveLoadChangesMap*					changesMap;			// 000
	BGSSaveLoadChangesMap*					previousChangeMap;	// 004
	RefIDIndexMapping*						refIDmapping;			// 008
	RefIDIndexMapping*						visitedWorldspaces;	// 00C
	Struct010*								sct010;				// 010
	NiTMap<TESForm*, BGSLoadGameSubBuffer>*	maps014[3];			// 014	0 = changed Animations, 2 = changed Havok Move
	NiTMap<UInt32, UInt32>*					map018;				// 018	
	BSSimpleArray<char*>*					strings;				// 01C
	BGSReconstructFormsInAllFilesMap*		rfiafMap;				// 020
	BSSimpleArray<BGSLoadFormBuffer*>		changedForms;			// 024
	NiTPointerMap<Actor*>					map0034;				// 034 Either dead or not dead actors
	UInt8									saveMods[255];			// 044
	UInt8									loadedMods[255];		// 143

	UInt16									pad242;					// 242
	UInt32									flg244;					// 244 bit 6 block updating player position/rotation from save, bit 2 set during save
	UInt8									formVersion;			// 248
	UInt8									pad249[3];				// 249

};

class SaveGameManager
{
public:
	SaveGameManager();
	~SaveGameManager();

	struct SaveGameData
	{
		const char* name;		// 00
		UInt32		unk04;		// 04
		UInt32		saveNumber;	// 08 index?
		const char* pcName;	// 0C
		const char* pcTitle;	// 10
		const char* location;	// 14
		const char* time;		// 18
	};

	TList<SaveGameData>*	saveList;		// 00
	UInt32					numSaves;		// 04
	UInt32					unk08;			// 08
	UInt8					unk0C;			// 0C	flag for either opened or writable or useSeparator (|)
	UInt8					unk0D;
	UInt8					unk0E;
	UInt8					unk0F;
/*
	const char				* unk10;		// 10 name of most recently loaded/saved game?
	UInt32					unk14;			// 14 init to -1
	UInt8					unk18;			// 18
	UInt8					pad19[3];
	UInt8					unk20;			// 20 init to 1
	UInt8					unk21;
	UInt8					pad22[2];
	UInt32					unk24;			// 24
	UInt32					unk28;			// 28
*/
	static SaveGameManager*		GetSingleton() { return *reinterpret_cast<SaveGameManager**>(0x011DE134); }
	__forceinline UInt32		ConstructSavegameFilename(const char* filename, char* outputBuf, bool bTempFile) { return StdCall<UInt32>(0x0084FF90, filename, outputBuf, bTempFile); }
	__forceinline bool			ConstructSavegamePath(char* outputBuf) { return StdCall<bool>(0x0084FF30, outputBuf); }
};

std::string GetSavegamePath();