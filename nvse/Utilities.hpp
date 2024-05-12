#pragma once
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <type_traits>
#include <chrono>

#include <ranges>
#include <string>
#include <vector>
#include <span>
#include <unordered_set>
#include <unordered_map>

#include <functional>

#include <intrin.h>
#include <Logging.hpp>

#include "Defines.hpp"

// thread-safe template versions of ThisStdCall()
template <typename T_Ret = UInt32, typename ...Args>
__forceinline T_Ret ThisCall(UInt32 _addr, void* _this, Args ...args)
{
	class T {};
	union {
		UInt32  addr;
		T_Ret(T::* func)(Args...);
	} u = { _addr };
	return ((T*)_this->*u.func)(std::forward<Args>(args)...);
}

template <typename T_Ret = UInt32, typename ...Args>
__forceinline T_Ret ThisStdCall(UInt32 _addr, const void* _this, Args ...args)
{
	return ((T_Ret(__thiscall*)(const void*, Args...))_addr)(_this, std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret StdCall(UInt32 _addr, Args ...args)
{
	return ((T_Ret(__stdcall*)(Args...))_addr)(std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret CdeclCall(UInt32 _addr, Args ...args)
{
	return ((T_Ret(__cdecl*)(Args...))_addr)(std::forward<Args>(args)...);
}

/*
class CriticalSection
{
public:
	CriticalSection() { InitializeCriticalSection(&critSection); }
	~CriticalSection() { DeleteCriticalSection(&critSection); }

	void	Enter() { EnterCriticalSection(&critSection); }
	void	Leave() { LeaveCriticalSection(&critSection); }
	bool	TryEnter() { return TryEnterCriticalSection(&critSection) != 0; }

private:
	CRITICAL_SECTION	critSection;
};

class ScopedLock
{
public:
	ScopedLock(CriticalSection& critSection);

	~ScopedLock();

private:
	CriticalSection& m_critSection;
};

class SpinLock
{
	UInt32	owningThread;
	UInt32	enterCount;

public:
	SpinLock() : owningThread(0), enterCount(0) {}

	void Enter();
	void EnterSleep();
	void Leave();
};
*/
__forceinline void* GameHeapAlloc(UInt32 size) { return ThisStdCall<void*>(0xAA3E40, (void*)0x11F6238, size); }
template <typename t> __forceinline t* GameHeapAlloc(UInt32 size) { return ThisStdCall<t*>(0xAA3E40, (void*)0x11F6238, size); }
template <typename t> __forceinline void GameHeapFree(t* ptr) { ThisStdCall(0xAA4060, (t*)0x11F6238, ptr); }

const std::filesystem::path& GetFalloutDirectory();
std::string GetNVSEConfigOption(const char * section, const char * key);
bool GetNVSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut);

// this has been tested to work for non-varargs functions
// varargs functions end up with 'this' passed as the last parameter (ie. probably broken)
// do NOT use with classes that have multiple inheritance

// if many member functions are to be declared, use MEMBER_FN_PREFIX to create a type with a known name
// so it doesn't need to be restated throughout the member list

// all of the weirdness with the _GetType function is because you can't declare a static const pointer
// inside the class definition. inlining automatically makes the function call go away since it's a const

#define MEMBER_FN_PREFIX(className)	\
	typedef className _MEMBER_FN_BASE_TYPE

#define DEFINE_MEMBER_FN_LONG(className, functionName, retnType, address, ...)		\
	typedef retnType (className::* _##functionName##_type)(__VA_ARGS__);			\
																					\
	inline _##functionName##_type * _##functionName##_GetPtr()					\
	{																				\
		static const UInt32 _address = address;										\
		return (_##functionName##_type *)&_address;									\
	}

#define DEFINE_MEMBER_FN(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)

#define CALL_MEMBER_FN(obj, fn)	\
	((*(obj)).*(*((obj)->_##fn##_GetPtr())))


// ConsolePrint() limited to 512 chars; use this to print longer strings to console
void Console_Print_Long(const std::string& str);

#define SIZEOF_ARRAY(arrayName, elementType) (sizeof(arrayName) / sizeof(elementType))

namespace MersenneTwister
{

	/* initializes mt[N] with a seed */
	void init_genrand(unsigned long s);

	/* initialize by an array with array-length */
	void init_by_array(unsigned long init_key[], int key_length);

	/* generates a random number on [0,0xffffffff]-interval */
	unsigned long genrand_int32();

	/* generates a random number on [0,0x7fffffff]-interval */
	long genrand_int31();

	/* generates a random number on [0,1]-real-interval */
	double genrand_real1();

	/* generates a random number on [0,1)-real-interval */
	double genrand_real2();

	/* generates a random number on (0,1)-real-interval */
	double genrand_real3();

	/* generates a random number on [0,1) with 53-bit resolution*/
	double genrand_res53();

};

// alternative to strtok; doesn't modify src string, supports forward/backward iteration
class Tokenizer
{
public:
	Tokenizer(const char* src, const char* delims);
	~Tokenizer();

	// these return the offset of token in src, or -1 if no token
	UInt32 NextToken(std::string& outStr);
	UInt32 PrevToken(std::string& outStr);

private:
	std::string m_delims;
	size_t		m_offset;
	std::string m_data;
};

const char GetSeparatorChar(Script * script);
std::string GetSeparatorChars(Script * script);

bool ci_equal(char ch1, char ch2);
bool ci_less(const char* lh, const char* rh);
void MakeUpper(std::string& str);
void MakeUpper(char* str);
void MakeLower(std::string& str);

// this copies the string onto the GameHeap - used to work around alloc/dealloc mismatch when passing
// data between nvse and plugins
char* CopyCString(const char* src);

// Generic error/warning output
// provides a common way to output errors and warnings
class ErrOutput
{
	typedef void (* _ShowError)(const char* msg);
	typedef bool (* _ShowWarning)(const char* msg);		// returns true if user requests to disable warning

	_ShowError		ShowError;
	_ShowWarning	ShowWarning;
public:
	ErrOutput(_ShowError errorFunc, _ShowWarning warningFunc);

	struct Message
	{
		const char*		fmt;
		bool			bCanDisable;
		bool			bDisabled;
	};

	void Show(Message msg, ...);
	void Show(const char* msg, ...);
	void vShow(Message& msg, va_list args);
	void vShow(const char* msg, va_list args);
};

void ShowErrorMessageBox(const char* message);

const char* GetModName(const TESForm* script);

std::vector<void*> GetCallStack(int i);


typedef void* (*memcpy_t)(void*, const void*, size_t);
extern memcpy_t _memcpy, _memmove;

//	Workaround for bypassing the compiler calling the d'tor on function-scope objects.
template <typename T> class TempObject
{
	friend T;

	struct Buffer
	{
		UInt8	bytes[sizeof(T)];
	}
	objData;

public:
	TempObject() { Reset(); }
	TempObject(const T& src) { objData = *(Buffer*)&src; }

	void Reset() { new ((T*)&objData) T(); }

	T& operator()() { return *(T*)&objData; }

	TempObject& operator=(const T& rhs) { objData = *(Buffer*)&rhs; }
	TempObject& operator=(const TempObject& rhs) { objData = rhs.objData; }
};

//	Assign rhs to lhs, bypassing operator=
template <typename T> __forceinline void RawAssign(const T& lhs, const T& rhs)
{
	struct Helper
	{
		UInt8	bytes[sizeof(T)];
	};
	*(Helper*)&lhs = *(Helper*)&rhs;
}

//	Swap lhs and rhs, bypassing operator=
template <typename T> __forceinline void RawSwap(const T& lhs, const T& rhs)
{
	struct Helper
	{
		UInt8	bytes[sizeof(T)];
	}
	temp = *(Helper*)&lhs;
	*(Helper*)&lhs = *(Helper*)&rhs;
	*(Helper*)&rhs = temp;
}

int __vectorcall ifloor(float value);

int __vectorcall iceil(float value);

UInt32 __fastcall StrLen(const char* str);

void __fastcall MemZero(void* dest, UInt32 bsize);

char* __fastcall StrCopy(char* dest, const char* src);

char* __fastcall StrNCopy(char* dest, const char* src, UInt32 length);

char* __fastcall StrCat(char* dest, const char* src);

char __fastcall StrCompare(const char* lstr, const char* rstr);

void __fastcall StrToLower(char* str);

char* __fastcall SubStrCI(const char* srcStr, const char* subStr);

char* __fastcall SlashPos(const char* str);

char* __fastcall CopyString(const char* key);

char* __fastcall IntToStr(char* str, int num);

UInt32 __fastcall StrHashCS(const char* inKey);

UInt32 __fastcall StrHashCI(const char* inKey);

#define GetRandomUInt(n) ThisStdCall<UInt32, UInt32>(0xAA5230, (void*)0x11C4180, n)

template <typename T>
bool In(T t, std::initializer_list<T> l)
{
	for (auto i : l)
		if (l == t)
			return true;
	return false;
}

#define _L(x, y) [&](x) {return y;}
#define _VL(x, y)[&]x {return y;} // variable parenthesis lambda

namespace ra = std::ranges;

std::string& ToLower(std::string&& data);
std::string& StripSpace(std::string&& data);

bool StartsWith(const char* left, const char* right);

template <typename T, typename S, typename F>
std::vector<T*> Filter(const S& s, const F& f)
{
	std::vector<T*> vec;
	for (auto& i : s)
	{
		if (f(i))
			vec->push_back(&i);
	}
	return vec;
}

template <typename T, typename V, typename F>
std::vector<T> MapTo(const V& v, const F& f)
{
	std::vector<T> vec;
	for (auto& i : v)
	{
		vec.emplace_back(f(i));
	}
	return vec;
}


template <typename T, const UInt32 ConstructorPtr = 0, typename... Args>
T* New(Args &&... args)
{
	auto* alloc = GameHeapAlloc(sizeof(T));
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
void Delete(T* t, Args &&... args)
{
	if constexpr (DestructorPtr)
	{
		ThisStdCall(DestructorPtr, t, std::forward<Args>(args)...);
	}
	GameHeapFree(t);
}

template <typename T>
using game_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

template <typename T, const UInt32 DestructorPtr = 0>
game_unique_ptr<T> MakeUnique(T* t)
{
	return game_unique_ptr<T>(t, [](T* t2) { Delete<T, DestructorPtr>(t2); });
}

template <typename T, const UInt32 ConstructorPtr = 0, const UInt32 DestructorPtr = 0, typename... ConstructorArgs>
game_unique_ptr<T> MakeUnique(ConstructorArgs &&... args)
{
	auto* obj = New<T, ConstructorPtr>(std::forward(args)...);
	return MakeUnique<T, DestructorPtr>(obj);
}

char* stristr(const char* str1, const char* str2);


union FunctionArg
{
	void* pVal;
	float		fVal;
	UInt32		uVal;
	SInt32		iVal;

	inline void operator=(void* other) { pVal = other; }
	inline void operator=(float other) { fVal = other; }
	inline void operator=(UInt32 other) { uVal = other; }
	inline void operator=(SInt32 other) { iVal = other; }
};

template<std::size_t Size = 256>
class bumping_memory_resource {
public:
	char* _ptr;
	char buffer[Size];

	explicit bumping_memory_resource()
		: _ptr(&buffer[0]) {}

	void* allocate(std::size_t size) noexcept {
		auto ret = _ptr;
		_ptr += size;
		return ret;
	}

	void deallocate(void*) noexcept {}
};

template <typename T, typename Resource = bumping_memory_resource<256>>
class bumping_allocator {
public:
	Resource* _res;

	using value_type = T;

	bumping_allocator() {}

	explicit bumping_allocator(Resource& res)
		: _res(&res) {}

	bumping_allocator(const bumping_allocator&) = default;
	template <typename U>
	bumping_allocator(const bumping_allocator<U, Resource>& other)
		: bumping_allocator(other.resource()) {}

	Resource& resource() const { return *_res; }

	T* allocate(std::size_t n) { return static_cast<T*>(_res->allocate(sizeof(T) * n)); }
	void deallocate(T* ptr, std::size_t) { _res->deallocate(ptr); }

	friend bool operator==(const bumping_allocator& lhs, const bumping_allocator& rhs) {
		return lhs._res == rhs._res;
	}

	friend bool operator!=(const bumping_allocator& lhs, const bumping_allocator& rhs) {
		return lhs._res != rhs._res;
	}
};

template <typename T, size_t Size>
class StackVector
{
	using InternalVector = std::vector<T, bumping_allocator<T, bumping_memory_resource<Size * sizeof(T)>>>;
	bumping_memory_resource<Size * sizeof(T)> resource_;
	bumping_allocator<T, bumping_memory_resource<Size * sizeof(T)>> allocator_;
	InternalVector internalVector_;
public:
	StackVector() : allocator_(resource_), internalVector_(allocator_)
	{
		internalVector_.reserve(Size);
	}

	StackVector(const StackVector& other) :
		allocator_(resource_),
		internalVector_(other.internalVector_, allocator_)
	{
	}

	StackVector& operator=(const StackVector& other)
	{
		if (this == &other)
			return *this;
		resource_._ptr = resource_.buffer;
		internalVector_ = std::vector(other.internalVector_, allocator_);
		return *this;
	}

	InternalVector* operator->() { return &internalVector_; }
	const InternalVector* operator->() const { return &internalVector_; }

	InternalVector& operator*() { return internalVector_; }
	const InternalVector& operator*() const { return internalVector_; }
};

template <size_t Size, typename T, typename S, typename F>
StackVector<T*, Size> Filter(S& s, F&& f)
{
	StackVector<T*, Size> vec;
	for (auto& i : s)
	{
		if (f(i))
			vec->emplace_back(&i);
	}
	return vec;
}

std::string UTF8toANSI(const std::string& str);
std::filesystem::path GetCurPath();
bool IsConsoleOpen();
__forceinline bool MenuMode() { return CdeclCall<bool>(0x702360); }

int HexStringToInt(const std::string& str);

std::string GetClipboardText();

inline int GetCharsSinceSpace(const char* text, UInt32 offset)
{
	const char* barPos = text + offset;
	int numChars = 0;

	while (barPos != text && !isalnum(*--barPos)) numChars++;
	while (barPos >= text && isalnum(*barPos--)) numChars++;

	return numChars;
}

inline int GetCharsTillSpace(const char* text, UInt32 offset)
{
	const char* barPos = text + offset;

	int numChars = 0;

	while (isalnum(*++barPos)) numChars++;
	while (*barPos && !isalnum(*barPos++)) numChars++;

	return numChars;
}

std::string DecompileScriptToFolder(const std::string& scriptName, Script* script, const std::string& fileExtension, const std::string_view& modName);

const std::string& SanitizeString(std::string&& str);