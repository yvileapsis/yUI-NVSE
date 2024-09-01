#pragma once

[[nodiscard]]
extern __declspec(allocator) void* BSNew(size_t stSize);
[[nodiscard]]
extern __declspec(allocator) void* BSNewAligned(size_t stAlign, size_t stSize);
[[nodiscard]]
extern __declspec(allocator) void* BSReallocate(void* pvMem, size_t stSize);
extern void  BSFree(void* pvMem);

extern SIZE_T BSSize(void* pvMem);

template <typename T_Data>
[[nodiscard]]
__declspec(restrict) __declspec(allocator) T_Data* BSNew() {
	return static_cast<T_Data*>(BSNew(sizeof(T_Data)));
};

template <typename T_Data>
[[nodiscard]]
__declspec(restrict) __declspec(allocator) T_Data* BSNew(size_t aCount) {
	return static_cast<T_Data*>(BSNew(sizeof(T_Data) * aCount));
};

template <typename T, const UInt32 ConstructorPtr = 0, typename... Args>
[[nodiscard]]
__declspec(restrict) T* BSCreate(Args &&... args)
{
	auto* alloc = BSNew<T>();
	if constexpr (ConstructorPtr) {
		ThisStdCall(ConstructorPtr, alloc, std::forward<Args>(args)...);
	}
	else {
		memset(alloc, 0, sizeof(T));
	}
	return static_cast<T*>(alloc);
}

template <typename T, const UInt32 DestructorPtr = 0, typename... Args>
void BSDelete(T* t, Args &&... args) {
	if constexpr (DestructorPtr) {
		ThisStdCall(DestructorPtr, t, std::forward<Args>(args)...);
	}
	BSFree(t);
}

#define BS_ALLOCATORS \
	/* 0x401000 */ \
	[[nodiscard]] __declspec(allocator) void* operator new(std::size_t size) { return BSNew(size); } \
	[[nodiscard]] __declspec(allocator) void* operator new[](std::size_t size) { return BSNew(size); } \
	/* 0x401030 */ \
	void operator delete(void* block) noexcept {  BSFree(block); } \
	void operator delete(void* block, std::size_t size) noexcept {  BSFree(block); } \
	void operator delete[](void* block) noexcept { BSFree(block); } \
	void operator delete[](void* block, std::size_t size) noexcept { BSFree(block); }