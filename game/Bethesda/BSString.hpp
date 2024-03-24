#pragma once

#include "BSMemObject.hpp"

// Technically it should be a template but I'm lazy
class BSStringT : BSMemObject {
public:
	BSStringT();
	~BSStringT();

	char*	pString;
	UInt16	sLen;
	UInt16	sMaxLen;

	bool		Set(const char* src);
	bool		Includes(const char* toFind) const;
	bool		Replace(const char* toReplace, const char* replaceWith); // replaces instance of toReplace with replaceWith
	bool		Append(const char* toAppend);
	double		Compare(const BSStringT& compareTo, bool caseSensitive = false) const;

	UInt32		GetLength() const;

	const char* CStr() const;
};

typedef BSStringT String;