#pragma once
// This file exists as a trash heap of all basic dependencies of many commonlib headers
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>

#include <winsock2.h>
#include <Windows.h>
#include <xmmintrin.h>


typedef unsigned char		UInt8;		//!< An unsigned 8-bit integer value
typedef unsigned short		UInt16;		//!< An unsigned 16-bit integer value
typedef unsigned long		UInt32;		//!< An unsigned 32-bit integer value
typedef unsigned long long	UInt64;		//!< An unsigned 64-bit integer value
typedef signed char			SInt8;		//!< A signed 8-bit integer value
typedef signed short		SInt16;		//!< A signed 16-bit integer value
typedef signed long			SInt32;		//!< A signed 32-bit integer value
typedef signed long long	SInt64;		//!< A signed 64-bit integer value
typedef float				Float32;	//!< A 32-bit floating point value
typedef double				Float64;	//!< A 64-bit floating point value



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



__forceinline void* GameHeapAlloc(UInt32 size) { return ThisStdCall<void*>(0xAA3E40, (void*)0x11F6238, size); }
template <typename t> __forceinline t* GameHeapAlloc(UInt32 size) { return ThisStdCall<t*>(0xAA3E40, (void*)0x11F6238, size); }
template <typename t> __forceinline void GameHeapFree(t* ptr) { ThisStdCall(0xAA4060, (t*)0x11F6238, ptr); }



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
#define SIZEOF_ARRAY(arrayName, elementType) (sizeof(arrayName) / sizeof(elementType))



#include <d3d9.h>

#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <stdio.h>

#include <string>
#include <vector>

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

class NiDX9Renderer;

typedef NiDX9Renderer NiD3DRenderer;

typedef struct _D3DVECTOR4 {
	float x;
	float y;
	float z;
	float w;
} D3DVECTOR4;
#define D3DVECTOR4_DEFINED

// added by yvile to have less reliance on d3d
typedef struct D3DXMATRIX {
	FLOAT _ij;
} D3DXMATRIX, * LPD3DXMATRIX;

typedef struct D3DXVECTOR3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} D3DXVECTOR3, * LPD3DXVECTOR3;

typedef enum D3DXIMAGE_FILEFORMAT {
	D3DXIFF_BMP = 0,
	D3DXIFF_JPG = 1,
	D3DXIFF_TGA = 2,
	D3DXIFF_PNG = 3,
	D3DXIFF_DDS = 4,
	D3DXIFF_PPM = 5,
	D3DXIFF_DIB = 6,
	D3DXIFF_HDR = 7,
	D3DXIFF_PFM = 8,
	D3DXIFF_FORCE_DWORD = 0x7fffffff
} D3DXIMAGE_FILEFORMAT, * LPD3DXIMAGE_FILEFORMAT;

typedef struct D3DXMACRO {
	LPCSTR Name;
	LPCSTR Definition;
} D3DXMACRO, * LPD3DXMACRO;

// Align D3DXMATRIX
typedef __declspec(align(16)) D3DXMATRIX	D3DXALIGNEDMATRIX;

typedef UInt32 NiShaderError;

#define STACK_ALLOCATE(class_name, pointer_name) \
	char pointer_name[sizeof(class_name)]; \
	class_name* p##pointer_name = (class_name*)pointer_name;

#define DEBUG_MESSAGES false

#define M_TAU 6.28318530717958647692   // tau

inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr))
		throw std::exception();
}

#define ALIGN8 __declspec(align(8))
#define ALIGN16 __declspec(align(16))

//#define USE_DX_MATH

#ifdef USE_DX_MATH
#include <DirectXMath.h>
using namespace DirectX;
#endif

//#define USE_DX_COLLISION

#ifdef USE_DX_COLLISION
#include <DirectXCollision.h>
using namespace DirectX;
#endif

#ifdef _DEBUG
class DebugString : public std::string {
public:
	__forceinline std::string get() {
		return *this;
	}
};
#else
class DebugString {
public:
	DebugString() : string() {}
	DebugString(const char* str) : string(str) {}

	void* fakeProxy;
	std::string string;

	__forceinline std::string get() const {
		return string;
	}
};
#endif

//Example in https://en.cppreference.com/w/cpp/utility/variant/visit
//Allows function overloading with c++ lambdas.
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

#define ASSERT_SIZE(a, b) static_assert(sizeof(a) == b, "Wrong structure size!");
#define ASSERT_OFFSET(a, b, c) static_assert(offsetof(a, b) == c, "Wrong member offset!");
#define CREATE_OBJECT(CLASS, ADDRESS) static CLASS* CreateObject() { return StdCall<CLASS*>(ADDRESS); };

/**
 *	A bitfield.
 */
template <typename T>
class Bitfield
{
public:
	Bitfield() { }
	~Bitfield() { }

	void	Clear(void) { field = 0; }						//!< Clears all bits
	void	RawSet(UInt32 data) { field = data; }					//!< Modifies all bits

	void	Set(UInt32 data) { field |= data; }					//!< Sets individual bits
	void	Clear(UInt32 data) { field &= ~data; }					//!< Clears individual bits
	void	Unset(UInt32 data) { Clear(data); }					//!< Clears individual bits
	void	Mask(UInt32 data) { field &= data; }					//!< Masks individual bits
	void	Toggle(UInt32 data) { field ^= data; }					//!< Toggles individual bits
	void	SetBit(UInt32 data, bool state)
	{
		if (state) Set(data); else Clear(data);
	}

	void	SetField(T data, T mask, T pos) {
		field = (field & ~mask) | (data << pos);
	}

	T		GetField(T mask, T pos) const {
		return (field & mask) >> pos;
	}

	T		Get(void) const { return field; }					//!< Gets all bits
	T		GetBit(UInt32 data) const { return field & data; }			//!< Gets individual bits
	T		Extract(UInt32 bit) const { return (field >> bit) & 1; }		//!< Extracts a bit
	T		ExtractField(UInt32 shift, UInt32 length)					//!< Extracts a series of bits
	{
		return (field >> shift) & (0xFFFFFFFF >> (32 - length));
	}

	bool	IsSet(UInt32 data) const { return ((field & data) == data) ? true : false; }	//!< Are all these bits set?
	bool	IsUnSet(UInt32 data) const { return (field & data) ? false : true; }			//!< Are all these bits clear?
	bool	IsClear(UInt32 data) const { return IsUnSet(data); }							//!< Are all these bits clear?

	T		field;	//!< bitfield data
};

typedef Bitfield <UInt8>	Bitfield8;		//!< An 8-bit bitfield
typedef Bitfield <UInt16>	Bitfield16;		//!< A 16-bit bitfield
typedef Bitfield <UInt32>	Bitfield32;		//!< A 32-bit bitfield

#define STATIC_ASSERT static_assert

STATIC_ASSERT(sizeof(Bitfield8) == 1);
STATIC_ASSERT(sizeof(Bitfield16) == 2);
STATIC_ASSERT(sizeof(Bitfield32) == 4);