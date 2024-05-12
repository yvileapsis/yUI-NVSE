#pragma once

#include "InterfacedClass.hpp"
#include "BSSimpleList.hpp"

template<class T_DATA>
class LockFreeQueue : InterfacedClass {
	DWORD dword4;
	DWORD dword8;
	DWORD dwordC;
	DWORD dword10;
	DWORD dword14;
	DWORD dword18;
	DWORD dword1C;
	BSSimpleList<UInt32> bssimplelist_int20;
	DWORD dword28[6];
};

STATIC_ASSERT(sizeof(LockFreeQueue<char const*>) == 0x40);