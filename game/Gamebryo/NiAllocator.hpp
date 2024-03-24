#pragma once

enum NiMemEventType {
	NI_UNKNOWN = 0x0,
	NI_OPER_NEW = 0x1,
	NI_OPER_NEW_ARRAY = 0x2,
	NI_OPER_DELETE = 0x3,
	NI_OPER_DELETE_ARRAY = 0x4,
	NI_MALLOC = 0x5,
	NI_REALLOC = 0x6,
	NI_ALIGNEDMALLOC = 0x7,
	NI_ALIGNEDREALLOC = 0x8,
	NI_FREE = 0x9,
	NI_ALIGNEDFREE = 0xA,
	NI_EXTERNAL_MALLOC = 0xB,
	NI_EXTERNAL_REALLOC = 0xC,
	NI_EXTERNAL_ALIGNEDMALLOC = 0xD,
	NI_EXTERNAL_ALGINEDREALLOC = 0xE,
	NI_EXTERNAL_FREE = 0xF,
	NI_EXTERNAL_ALIGNEDFREE = 0x10,
};


class NiAllocator {
public:
	virtual void	Destroy(bool);
	virtual void*	Allocate(size_t& stSizeInBytes, size_t& stAlignment, NiMemEventType eEventType, bool bProvideAccurateSizeOnDeallocate, const char* pcFile, int iLine, const char* pcFunction);
	virtual void	Deallocate(void* pvMemory, NiMemEventType eEventType, size_t stSizeInBytes);
	virtual void*	Reallocate(void* pvMemory, size_t& stSizeInBytes, size_t& stAlignment, NiMemEventType eEventType, bool bProvideAccurateSizeOnDeallocate, size_t stSizeCurrent, const char* pcFile, int iLine, const char* pcFunction);
	virtual void*	AllocateExternal(size_t& stSizeInBytes, size_t& stAlignment, NiMemEventType eEventType, bool bProvideAccurateSizeOnDeallocate, const char* pcFile, int iLine, const char* pcFunction);
	virtual void	DeallocateExternal(void* pvMemory, NiMemEventType eEventType, size_t stSizeInBytes);
	virtual void*	ReallocateExternal(void* pvMemory, size_t* stSizeInBytes, size_t* stAlignment, NiMemEventType eEventType, bool bProvideAccurateSizeOnDeallocate, size_t stSizeCurrent, const char* pcFile, int iLine, const char* pcFunction);
	virtual void	Initialize();
	virtual void	Shutdown();
	virtual bool	VerifyAddress(const void* pvMemory);
};
