#pragma once

void*	NiNew(size_t stSize);
void*	NiAlloc(size_t stSize);
void	NiFree(void* pvMem);
void	NiAlignedFree(void* pvMem);
void	NiDelete(void* pvMem, size_t stElementSize);
template <typename T_Data>
static T_Data* NiNew() {
	return (T_Data*)NiNew(sizeof(T_Data));
}

template <typename T_Data>
static T_Data* NiAlloc(UInt32 auiCount = 1) {
	return (T_Data*)NiAlloc(sizeof(T_Data) * auiCount);
}


template <typename T, const UInt32 ConstructorPtr = 0, typename... Args>
T* NiCreate(Args &&... args) {
	auto* alloc = NiNew(sizeof(T));
	if constexpr (ConstructorPtr) {
		ThisStdCall(ConstructorPtr, alloc, std::forward<Args>(args)...);
	}
	else {
		memset(alloc, 0, sizeof(T));
	}
	return static_cast<T*>(alloc);
}