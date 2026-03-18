#pragma once

#include "NiGlobalStringTable.hpp"

class NiFixedString : public NiMemObject {
public:
	NiFixedString();
	NiFixedString(const char* apcString);
	NiFixedString(const NiFixedString& arString);
	~NiFixedString();

	NiGlobalStringTable::GlobalStringHandle m_kHandle;

	NiFixedString& operator=(const char* apcString);
	NiFixedString& operator=(const NiFixedString& arString);
	friend bool operator==(const NiFixedString& arString1, const NiFixedString& arString2);
	friend bool operator==(const NiFixedString& arString, const char* apcString);
	friend bool operator==(const char* apcString, const NiFixedString& arString);

	operator const char*() const;

	operator bool() const;

	const char* c_str() const;

	operator std::string_view() const noexcept;

	uint32_t GetLength() const;

	bool Includes(const char* apToFind) const;
};

ASSERT_SIZE(NiFixedString, 0x4)