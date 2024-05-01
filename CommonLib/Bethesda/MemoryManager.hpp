#pragma once

#include "NiTMap.hpp"

class BSExtraData;
class ExtraDataList;
class NiNode;
class ScriptLocals;
class TESObjectREFR;
class MemoryPool;
class IMemoryHeap;
struct HeapStats;
class ScrapHeap;
class IMemoryManagerFile;
class TESForm;
class Script;

enum Context {
	MC_UNK0						= 0,
	MC_UNK1						= 1,
	MC_UNK2						= 2,
	MC_MEMPOOLS					= 3,
	MC_WINDOW					= 4,
	MC_UNK5						= 5,
	MC_THREAD_SAFE_STRUCT		= 6,
	MC_EFFECTS					= 7,
	MC_STRING					= 8,
	MC_SETTINGS					= 9,
	MC_SYSTEM					= 10,
	MC_AUDIO					= 11,
	MC_UNK_12					= 12,
	MC_MENU						= 13,
	MC_LOCAL_MAP				= 14,
	MC_RENDER_15				= 15, // Texture palettes, NiStaticDataManager
	MC_HAVOK					= 16,
	MC_SAVE_LOAD				= 17,
	MC_UNK_18					= 18,
	MC_ARCHIVE_MANAGER			= 19,
	MC_MOVIE_PLAYER				= 20,
	MC_SCRIPT					= 21,
	MC_UNK_22					= 22,
	MC_FILE_BUFFER				= 23,
	MC_FILE_CACHE				= 24,
	MC_SCENEGRAPH				= 25,
	MC_CELL						= 26,
	MC_TERRAIN_LAND				= 27,
	MC_TERRAIN_LOD				= 28,
	MC_WATER					= 29,
	MC_TREES_0					= 30,
	MC_TREES_1					= 31,
	MC_GRIDCELL					= 32,
	MC_SKY						= 33,
	MC_TERRAIN_NODE				= 34,
	MC_TERRAIN_35				= 35,
	MC_TERRAIN_36				= 36,
	MC_TERRAIN_37				= 37,
	MC_RENDER_38				= 38,
	MC_IMAGESPACE				= 39,
	MC_HAIR_SHADER				= 40,
	MC_RENDER_41				= 41,
	MC_PROCESS_MANAGER			= 42,
	MC_COMBAT_SYSTEM			= 43,
	MC_LOADED_REF_COLLECTION	= 44,
	MC_ACTOR_0					= 45,
	MC_NAVMESH					= 46,
	MC_DATAHANDLER				= 47,
	MC_FILE_COMPRESSED			= 48,
	MC_REF						= 49,
	MC_ACTOR_1					= 50,
	MC_ANIMATION				= 51,
	MC_PLAYER					= 52,
	MC_UNK_53					= 53,
	MC_INVENTORY				= 54,
	MC_FACEGEN_0				= 55,
	MC_FACEGEN_1				= 56,
	MC_FACEGEN_2				= 57,
	MC_UNK_58					= 58,
	MC_UNK_59					= 59,
	MC_UNK_60					= 60,
	MC_UNK_61					= 61,
	MC_UNK_62					= 62,
	MC_UNK_63					= 63,
	MC_UNK_64					= 64,
	MC_DEFAULT					= 65,
};

// Stolen from JIP
struct TLSData {

	UInt32			unk000;				// 000
	UInt32			unk004;				// 004
	ExtraDataList*	pLastXtraList;		// 008
	UInt32			unk00C;				// 00C
	BSExtraData*	xDatas[0x93];	// 010
	bool			bLoadedActorIsHighDetail;			// 25C
	bool			byte25D;			// 25C
	NiNode*			pLastNiNode;		// 260
	TESObjectREFR*  pLastNiNodeREFR;	// 264
	bool			bConsoleMode;		// 268
	UInt32			unk26C[4];			// 26C
	TESForm*		pLastRefVar;		// 27C
	SInt32			iLastVarIndex;		// 280
	ScriptLocals*	pLastEventList;		// 284
	Script*			pLastScript;		// 288
	UInt32			activateRecursionDepth;	// 28C
	UInt32			unk290;				// 290
	UInt32			uiFlags;			// 294
	bool			bUnk298;
	UInt32			unk29C;
	float			fLastScaledTime;
	UInt32			eLastCycle;
	float			fLastWeightedPhaseTime;
	float			fLastLoKeyTime;
	float			fLastHiKeyTime;
	UInt32			uiHeapIndex;			// 2B4
	UInt32			uiWarningCount;				// 2B8
	UInt32			uiAccumulatorCount;				// 2BC
    UInt32          unk2C0;

	static TLSData* GetTLSData();
	static UInt32 GetHeapIndex();
	static UInt32 GetAccumulatorCount();
	static void SetHeapIndex(UInt32 index);

	// 25C is used as do not head track the player , 2B8 is used to init QueudFile::unk0018
};
ASSERT_SIZE(TLSData, 0x2C4);

struct MemoryManager {
	static const char* ContextNames[66];
	static UInt32 ContextCounters[66];

    bool						bInitialized;
    UInt16						usNumHeaps;
    UInt16						usNumPhysicalHeaps;
    IMemoryHeap**				ppHeaps;
    IMemoryHeap*				pHeapsByContext[66];
    NiTMap<UInt32, ScrapHeap*>	kHeapMap; // BSMapBase
	IMemoryHeap**				ppPhysicalHeaps;
    bool						bSpecialHeaps;
    bool						bAllowPoolUse;
    UInt32						uiSysAllocBytes;
	UInt32						uiMallocBytes;
	UInt32						uiAlignmentForPools;

	static MemoryPool* GetPool(UInt32 auiIndex);

    static MemoryManager* GetSingleton();
    static ScrapHeap* GetThreadScrapHeap();
    
	template <typename T_Data>
	__declspec(restrict) __declspec(allocator) static T_Data* Allocate() {
        return static_cast<T_Data*>(Allocate(sizeof(T_Data)));
    };
	template <typename T_Data>
	__declspec(restrict) __declspec(allocator) static T_Data* Allocate(size_t aCount) {
		return static_cast<T_Data*>(Allocate(sizeof(T_Data) * aCount));
	};
	__declspec(restrict) __declspec(allocator) static void* Allocate(size_t aSize); // operator new
    static void* Reallocate(void* apOldMem, size_t auiNewSize);
    static void Deallocate(void* apMemory); // operator delete

    static void* AllocateScrap(size_t aSize, UInt32 aAlignment);
    static void DeallocateScrap(void* apMemory);

	static void EnterContext(UInt32& apPrevious, UInt32 auiNew);
	static void LeaveContext(UInt32& apPrevious);

	void SpecifyMemoryLayout();
	void CreatePool(UInt32 auiPoolNumber, UInt32 auiSize, const char* apName);
	static MemoryPool* SelectPoolFromID(UInt32 auiID);

	size_t Size(void* apMemory);

	void* DefaultAllocate(UInt32 auiSize, bool abTrack = false);
	void Free(void* apMemory, bool abTrack = false);
	IMemoryHeap* GetHeapByIndex(UInt32 auiIndex) const;
	IMemoryHeap* GetHeapForPointer(void* apMem) const;
	IMemoryHeap* GetHeapForPhysicalPointer(void* apMem) const;
	UInt32 ProcessMemoryProblemImpl(IMemoryHeap* apMemoryHeap, UInt32 aiMemoryPass, bool* apbAllowSystemAllocs);

	bool GetHeapStats(UInt32 auiIndex, bool abFullBlockInfo, HeapStats* apStats) const;
	bool GetPhysicalHeapStats(UInt32 auiPhysicalHeapIndex, bool abFullBlockInfo, HeapStats* apStats) const;
};

class AutoMemContext {
public:
	AutoMemContext(UInt32 auiContext) {
		MemoryManager::EnterContext(uiPrevious, auiContext);
	}

	~AutoMemContext() {
		MemoryManager::LeaveContext(uiPrevious);
	}

private:
	UInt32 uiPrevious;
};

template <typename T, const UInt32 ConstructorPtr = 0, typename... Args>
__declspec(restrict) T* BSCreate(Args &&... args)
{
	auto* alloc = MemoryManager::Allocate<T>();
	if constexpr (ConstructorPtr)
	{
		ThisStdCall(ConstructorPtr, alloc, std::forward<Args>(args)...);
	}
	else
	{
		memset(alloc, 0, sizeof(T));
	}
	return static_cast<T*>(alloc);
}

template <typename T, const UInt32 DestructorPtr = 0, typename... Args>
void BSDelete(T* t, Args &&... args)
{
	if constexpr (DestructorPtr)
	{
		ThisStdCall(DestructorPtr, t, std::forward<Args>(args)...);
	}
	MemoryManager::Deallocate(t);
}

#define BS_ALLOCATOR \
    static __declspec(restrict) void* operator new(size_t stSize) { \
        return MemoryManager::Allocate(stSize); \
    } \
	static void operator delete(void* apMem) { \
        return MemoryManager::Deallocate(apMem); \
    }