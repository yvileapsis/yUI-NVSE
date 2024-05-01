#pragma once

#include "BSMemObject.hpp"

class BSHash : public BSMemObject {
public:
	enum Process {
		HASH_FILENAME	= 0,
		HASH_DIRECTORY	= 1,
		HASH_NORMAL		= 2
	};

	BSHash() : cLast(0), cLast2(0), cLength(0), cFirst(0), iCRC(0) {};
	BSHash(char* apString, UInt32 aeProcess);
	~BSHash() {};

	UInt8	cLast;
	UInt8	cLast2;
	UInt8	cLength;
	UInt8	cFirst;
	UInt32	iCRC;

	SInt32 Compare(const BSHash& arHash) const;
	SInt32 WildCardCompare(const BSHash& arHash) const;
	UInt32 ExtensionIndex() const;

	static void Create(const char* apString, const char* apExtension, BSHash& arHash);
	static void MakeDirAndFile(const char* apString, BSHash& arDirHash, BSHash& arFileHash);
};

ASSERT_SIZE(BSHash, 0x8);