#pragma once
#include "BSMemObject.hpp"

// Technically it should be a template but I'm lazy
// 0x8
template <class T_Name>
class BSStringT : public BSMemObject
{
public:
	BSStringT();
	~BSStringT();

	T_Name*		pcString;
	UInt16		usLen;
	UInt16		usMaxLen;

	bool		Set(const T_Name* src);
	bool		Includes(const T_Name* toFind) const;
	bool		Replace(const T_Name* toReplace, const T_Name* replaceWith); // replaces instance of toReplace with replaceWith
	bool		Append(const T_Name* toAppend);
	double		Compare(const BSStringT& compareTo, bool caseSensitive = false);

	UInt32		GetLength();

	const char* CStr();
	std::string StdStr() const { return { pcString, usLen }; }

	[[nodiscard]] operator std::basic_string_view<T_Name>() const noexcept { return { pcString, usLen }; }

};
static_assert(sizeof(BSStringT<char>) == 0x8);

typedef BSStringT<char> String;