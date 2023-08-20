#pragma once

#define IMAGEAPI __stdcall

enum SK_Bitness {
	ThirtyTwoBit = 32,
	SixtyFourBit = 64
};

#ifdef _WIN64
#define BITNESS SixtyFourBit
#else 
#define BITNESS ThirtyTwoBit
#endif


typedef enum {
	AddrMode1616,
	AddrMode1632,
	AddrModeReal,
	AddrModeFlat
} ADDRESS_MODE;

typedef enum _IMAGEHLP_SYMBOL_TYPE_INFO {
	TI_GET_SYMTAG,
	TI_GET_SYMNAME,
	TI_GET_LENGTH,
	TI_GET_TYPE,
	TI_GET_TYPEID,
	TI_GET_BASETYPE,
	TI_GET_ARRAYINDEXTYPEID,
	TI_FINDCHILDREN,
	TI_GET_DATAKIND,
	TI_GET_ADDRESSOFFSET,
	TI_GET_OFFSET,
	TI_GET_VALUE,
	TI_GET_COUNT,
	TI_GET_CHILDRENCOUNT,
	TI_GET_BITPOSITION,
	TI_GET_VIRTUALBASECLASS,
	TI_GET_VIRTUALTABLESHAPEID,
	TI_GET_VIRTUALBASEPOINTEROFFSET,
	TI_GET_CLASSPARENTID,
	TI_GET_NESTED,
	TI_GET_SYMINDEX,
	TI_GET_LEXICALPARENT,
	TI_GET_ADDRESS,
	TI_GET_THISADJUST,
	TI_GET_UDTKIND,
	TI_IS_EQUIV_TO,
	TI_GET_CALLING_CONVENTION,
	TI_IS_CLOSE_EQUIV_TO,
	TI_GTIEX_REQS_VALID,
	TI_GET_VIRTUALBASEOFFSET,
	TI_GET_VIRTUALBASEDISPINDEX,
	TI_GET_IS_REFERENCE,
	TI_GET_INDIRECTVIRTUALBASECLASS,
	TI_GET_VIRTUALBASETABLETYPE,
	IMAGEHLP_SYMBOL_TYPE_INFO_MAX,
} IMAGEHLP_SYMBOL_TYPE_INFO;

typedef struct _KDHELP64 {
	DWORD64		Thread;				// address of kernel thread object, as provided in the WAIT_STATE_CHANGE packet.
	DWORD		ThCallbackStack;	// offset in thread object to pointer to the current callback frame in kernel stack.
	DWORD		ThCallbackBStore;	// offset in thread object to pointer to the current callback backing store frame in kernel stack.
	DWORD		NextCallback;		// offsets to values in frame: address of next callback frame
	DWORD		FramePointer;		// address of saved frame pointer (if applicable)
	DWORD64		KiCallUserMode;		// Address of the kernel function that calls out to user mode
	DWORD64		KeUserCallbackDispatcher;	// Address of the user mode dispatcher function
	DWORD64		SystemRangeStart;	// Lowest kernel mode address
	DWORD64		KiUserExceptionDispatcher;	// Address of the user mode exception dispatcher function. Added in API version 10.
	DWORD64		StackBase;			// Stack bounds, added in API version 11.
	DWORD64		StackLimit;
	DWORD		BuildVersion;		// Target OS build number. Added in API version 12.
	DWORD		RetpolineStubFunctionTableSize;
	DWORD64		RetpolineStubFunctionTable;
	DWORD		RetpolineStubOffset;
	DWORD		RetpolineStubSize;
	DWORD64		Reserved0[2];
} KDHELP64, * PKDHELP64;

typedef struct _tagADDRESS64 {
	DWORD64			Offset;
	WORD			Segment;
	ADDRESS_MODE	Mode;
} ADDRESS64, * LPADDRESS64;

typedef struct _tagSTACKFRAME64 {
	ADDRESS64   AddrPC;				// program counter
	ADDRESS64   AddrReturn;			// return address
	ADDRESS64   AddrFrame;			// frame pointer
	ADDRESS64   AddrStack;			// stack pointer
	ADDRESS64   AddrBStore;			// backing store pointer
	PVOID		FuncTableEntry;		// pointer to pdata/fpo or NULL
	DWORD64		Params[4];			// possible arguments to the function
	BOOL		Far;				// WOW far call
	BOOL		Virtual;			// is this a virtual frame?
	DWORD64		Reserved[3];
	KDHELP64	KdHelp;
} STACKFRAME64, * LPSTACKFRAME64;

typedef struct _tagADDRESS {
	DWORD			Offset;
	WORD			Segment;
	ADDRESS_MODE	Mode;
} ADDRESS, * LPADDRESS;

typedef struct _KDHELP {
	DWORD	Thread; 			// address of kernel thread object, as provided in the WAIT_STATE_CHANGE packet.
	DWORD	ThCallbackStack;	// offset in thread object to pointer to the current callback frame in kernel stack.
	DWORD	NextCallback; 		// offsets to values in frame: address of next callback frame
	DWORD	FramePointer; 		// address of saved frame pointer (if applicable)
	DWORD	KiCallUserMode;		// Address of the kernel function that calls out to user mode
	DWORD	KeUserCallbackDispatcher; 	// Address of the user mode dispatcher function
	DWORD	SystemRangeStart;	// Lowest kernel mode address
	DWORD	ThCallbackBStore;	// offset in thread object to pointer to the current callback backing store frame in kernel stack.
	DWORD	KiUserExceptionDispatcher;	// Address of the user mode exception dispatcher function. Added in API version 10.
	DWORD	StackBase; 			// Stack bounds, added in API version 11.
	DWORD   StackLimit;
	DWORD   Reserved[5];
} KDHELP, * PKDHELP;

typedef struct _tagSTACKFRAME {
	ADDRESS	AddrPC;			// program counter
	ADDRESS	AddrReturn;		// return address
	ADDRESS	AddrFrame;		// frame pointer
	ADDRESS	AddrStack;		// stack pointer
	PVOID	FuncTableEntry;	// pointer to pdata/fpo or NULL
	DWORD	Params[4];		// possible arguments to the function
	BOOL	Far;			// WOW far call
	BOOL	Virtual;		// is this a virtual frame?
	DWORD	Reserved[3];
	KDHELP	KdHelp;
	ADDRESS	AddrBStore;		// backing store pointer
} STACKFRAME, * LPSTACKFRAME;

typedef struct _IMAGEHLP_LINE64 {
	DWORD	SizeOfStruct;		// set to sizeof(IMAGEHLP_LINE64)
	PVOID	Key;				// internal
	DWORD	LineNumber;			// line number in file
	PCHAR	FileName;			// full filename
	DWORD64	Address;			// first instruction of line
} IMAGEHLP_LINE64, * PIMAGEHLP_LINE64;

typedef struct _IMAGEHLP_LINE {
	DWORD	SizeOfStruct;		   // set to sizeof(IMAGEHLP_LINE)
	PVOID	Key;					// internal
	DWORD	LineNumber;			 // line number in file
	PCHAR	FileName;			   // full filename
	DWORD	Address;				// first instruction of line
} IMAGEHLP_LINE, * PIMAGEHLP_LINE;

typedef struct _SYMBOL_INFO {
	ULONG		SizeOfStruct;
	ULONG		TypeIndex;			// Type Index of symbol
	ULONG64		Reserved[2];
	ULONG		Index;
	ULONG		Size;
	ULONG64		ModBase;			// Base Address of module comtaining this symbol
	ULONG		Flags;
	ULONG64		Value;				// Value of symbol, ValuePresent should be 1
	ULONG64		Address;			// Address of symbol including base address of module
	ULONG		Register;			// register holding value or pointer to value
	ULONG		Scope;				// scope of the symbol
	ULONG		Tag;				// pdb classification
	ULONG		NameLen;			// Actual length of name
	ULONG		MaxNameLen;
	CHAR		Name[1];			// Name of symbol
} SYMBOL_INFO, * PSYMBOL_INFO;
