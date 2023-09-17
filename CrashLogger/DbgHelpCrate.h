#pragma once
#include <DbgHelp.h>
/*

#define IMAGEAPI __stdcall

#define SYMOPT_CASE_INSENSITIVE          0x00000001
#define SYMOPT_UNDNAME                   0x00000002
#define SYMOPT_DEFERRED_LOADS            0x00000004
#define SYMOPT_NO_CPP                    0x00000008
#define SYMOPT_LOAD_LINES                0x00000010
#define SYMOPT_OMAP_FIND_NEAREST         0x00000020
#define SYMOPT_LOAD_ANYTHING             0x00000040
#define SYMOPT_IGNORE_CVREC              0x00000080
#define SYMOPT_NO_UNQUALIFIED_LOADS      0x00000100
#define SYMOPT_FAIL_CRITICAL_ERRORS      0x00000200
#define SYMOPT_EXACT_SYMBOLS             0x00000400
#define SYMOPT_ALLOW_ABSOLUTE_SYMBOLS    0x00000800
#define SYMOPT_IGNORE_NT_SYMPATH         0x00001000
#define SYMOPT_INCLUDE_32BIT_MODULES     0x00002000
#define SYMOPT_PUBLICS_ONLY              0x00004000
#define SYMOPT_NO_PUBLICS                0x00008000
#define SYMOPT_AUTO_PUBLICS              0x00010000
#define SYMOPT_NO_IMAGE_SEARCH           0x00020000
#define SYMOPT_SECURE                    0x00040000
#define SYMOPT_NO_PROMPTS                0x00080000
#define SYMOPT_OVERWRITE                 0x00100000
#define SYMOPT_IGNORE_IMAGEDIR           0x00200000
#define SYMOPT_FLAT_DIRECTORY            0x00400000
#define SYMOPT_FAVOR_COMPRESSED          0x00800000
#define SYMOPT_ALLOW_ZERO_ADDRESS        0x01000000
#define SYMOPT_DISABLE_SYMSRV_AUTODETECT 0x02000000
#define SYMOPT_READONLY_CACHE            0x04000000
#define SYMOPT_SYMPATH_LAST              0x08000000
#define SYMOPT_DISABLE_FAST_SYMBOLS      0x10000000
#define SYMOPT_DISABLE_SYMSRV_TIMEOUT    0x20000000
#define SYMOPT_DISABLE_SRVSTAR_ON_STARTUP 0x40000000
#define SYMOPT_DEBUG                     0x80000000


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

typedef enum {
	SymNone = 0,
	SymCoff,
	SymCv,
	SymPdb,
	SymExport,
	SymDeferred,
	SymSym,       // .sym file
	SymDia,
	SymVirtual,
	NumSymTypes
} SYM_TYPE;

typedef struct _IMAGEHLP_MODULE {
	DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE)
	DWORD    BaseOfImage;            // base load address of module
	DWORD    ImageSize;              // virtual size of the loaded module
	DWORD    TimeDateStamp;          // date/time stamp from pe header
	DWORD    CheckSum;               // checksum from the pe header
	DWORD    NumSyms;                // number of symbols in the symbol table
	SYM_TYPE SymType;                // type of symbols loaded
	CHAR     ModuleName[32];         // module name
	CHAR     ImageName[256];         // image name
	CHAR     LoadedImageName[256];   // symbol file name
} IMAGEHLP_MODULE, * PIMAGEHLP_MODULE;

typedef struct _IMAGEHLP_SYMBOL {
	DWORD SizeOfStruct;           // set to sizeof(IMAGEHLP_SYMBOL)
	DWORD Address;                // virtual address including dll base address
	DWORD Size;                   // estimated size of symbol, can be zero
	DWORD Flags;                  // info about the symbols, see the SYMF defines
	DWORD                       MaxNameLength;          // maximum size of symbol name in 'Name'
	CHAR                        Name[1];                // symbol name (null terminated string)
} IMAGEHLP_SYMBOL, *PIMAGEHLP_SYMBOL;

typedef BOOL	(__stdcall* PREAD_PROCESS_MEMORY_ROUTINE)		(_In_ HANDLE hProcess, _In_ DWORD lpBaseAddress, _Out_writes_bytes_(nSize) PVOID lpBuffer, _In_ DWORD nSize, _Out_ PDWORD lpNumberOfBytesRead);
typedef BOOL	(__stdcall* PREAD_PROCESS_MEMORY_ROUTINE64)		(_In_ HANDLE hProcess, _In_ DWORD64 qwBaseAddress, _Out_writes_bytes_(nSize) PVOID lpBuffer, _In_ DWORD nSize, _Out_ LPDWORD lpNumberOfBytesRead);
typedef PVOID	(__stdcall* PFUNCTION_TABLE_ACCESS_ROUTINE)		(_In_ HANDLE hProcess, _In_ DWORD AddrBase);
typedef PVOID	(__stdcall* PFUNCTION_TABLE_ACCESS_ROUTINE64)	(_In_ HANDLE hProcess, _In_ DWORD64 AddrBase);
typedef DWORD	(__stdcall* PGET_MODULE_BASE_ROUTINE)			(_In_ HANDLE hProcess, _In_ DWORD Address);
typedef DWORD64	(__stdcall* PGET_MODULE_BASE_ROUTINE64)			(_In_ HANDLE hProcess, _In_ DWORD64 Address);
typedef DWORD	(__stdcall* PTRANSLATE_ADDRESS_ROUTINE)			(_In_ HANDLE hProcess, _In_ HANDLE hThread, _Out_ LPADDRESS lpaddr);
typedef	DWORD64	(__stdcall* PTRANSLATE_ADDRESS_ROUTINE64)		(_In_ HANDLE hProcess, _In_ HANDLE hThread, _In_ LPADDRESS64 lpaddr);


typedef BOOL (CALLBACK *PENUMLOADED_MODULES_CALLBACK)(
	_In_ PCSTR ModuleName,
	_In_ ULONG ModuleBase,
	_In_ ULONG ModuleSize,
	_In_opt_ PVOID UserContext
	);

typedef
DWORD
(__stdcall *PGET_MODULE_BASE_ROUTINE)(
	_In_ HANDLE hProcess,
	_In_ DWORD Address
	);

	*/

BOOL __stdcall Safe_SymInitializeW(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess);
BOOL __stdcall  Safe_SymGetSearchPathW(HANDLE hProcess, PCWSTR SearchPath, DWORD SearchPathLength);
BOOL __stdcall  Safe_SymSetSearchPathW(HANDLE hProcess, PCWSTR SearchPath);
BOOL __stdcall  Safe_SymRefreshModuleList(HANDLE hProcess);
BOOL __stdcall  Safe_StackWalk(_In_ DWORD MachineType, _In_ HANDLE hProcess, _In_ HANDLE hThread, _Inout_ LPSTACKFRAME StackFrame, _Inout_ PVOID ContextRecord, _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE ReadMemoryRoutine, _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE FunctionTableAccessRoutine, _In_opt_ PGET_MODULE_BASE_ROUTINE GetModuleBaseRoutine, _In_opt_ PTRANSLATE_ADDRESS_ROUTINE	 TranslateAddress);
BOOL __stdcall  Safe_StackWalk64(_In_ DWORD MachineType, _In_ HANDLE hProcess, _In_ HANDLE hThread, _Inout_ LPSTACKFRAME64 StackFrame, _Inout_ PVOID ContextRecord, _In_opt_ PREAD_PROCESS_MEMORY_ROUTINE64 ReadMemoryRoutine, _In_opt_ PFUNCTION_TABLE_ACCESS_ROUTINE64 FunctionTableAccessRoutine, _In_opt_ PGET_MODULE_BASE_ROUTINE64 GetModuleBaseRoutine, _In_opt_ PTRANSLATE_ADDRESS_ROUTINE64 TranslateAddress);
DWORD __stdcall  Safe_SymSetOptions(DWORD SymOptions);
DWORD __stdcall  Safe_SymGetModuleBase(_In_ HANDLE hProcess, _In_ DWORD dwAddr);
DWORD64 __stdcall  Safe_SymGetModuleBase64(_In_ HANDLE hProcess, _In_ DWORD dwAddr);
BOOL __stdcall  Safe_SymGetLineFromAddr(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE Line);
BOOL __stdcall  Safe_SymGetLineFromAddr64(_In_ HANDLE hProcess, _In_ DWORD64 qwAddr, _Out_ PDWORD pdwDisplacement, _Out_ PIMAGEHLP_LINE64 Line64);
BOOL __stdcall  Safe_SymUnloadModule(_In_ HANDLE hProcess, _In_ DWORD BaseOfDll);
BOOL __stdcall  Safe_SymUnloadModule64(_In_ HANDLE hProcess, _In_ DWORD64 BaseOfDll);
BOOL __stdcall  Safe_SymGetTypeInfo(_In_ HANDLE hProcess, _In_ DWORD64 ModBase, _In_ ULONG TypeId, _In_  IMAGEHLP_SYMBOL_TYPE_INFO GetType, _Out_ PVOID pInfo);
BOOL __stdcall  Safe_SymFromAddr(_In_ HANDLE hProcess, _In_ DWORD64 Address, _Out_opt_ PDWORD64 Displacement, _Inout_ PSYMBOL_INFO Symbol);
BOOL __stdcall  Safe_SymInitialize(_In_ HANDLE hProcess, _In_opt_ PCSTR UserSearchPath, _In_ BOOL fInvadeProcess);
BOOL __stdcall  Safe_SymCleanup(_In_ HANDLE hProcess);
DWORD __stdcall  Safe_SymLoadModule(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD BaseOfDll, _In_ DWORD SizeOfDll);
DWORD64 __stdcall  Safe_SymLoadModule64(_In_ HANDLE hProcess, _In_opt_ HANDLE hFile, _In_opt_ PCSTR ImageName, _In_opt_ PCSTR ModuleName, _In_ DWORD64 BaseOfDll, _In_ DWORD SizeOfDll);
BOOL __stdcall  Safe_EnumerateLoadedModules(_In_ HANDLE hProcess, _In_ PENUMLOADED_MODULES_CALLBACK EnumLoadedModulesCallback, _In_opt_ PVOID UserContext);
BOOL __stdcall Safe_SymGetModuleInfo(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_ PIMAGEHLP_MODULE ModuleInfo);
BOOL __stdcall Safe_SymGetSymFromAddr(_In_ HANDLE hProcess, _In_ DWORD dwAddr, _Out_opt_ PDWORD pdwDisplacement, _Inout_ PIMAGEHLP_SYMBOL Symbol);
PVOID __stdcall Safe_SymFunctionTableAccess(_In_ HANDLE hProcess, _In_ DWORD AddrBase);