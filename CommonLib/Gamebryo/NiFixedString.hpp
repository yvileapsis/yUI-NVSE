#pragma once

#include "NiMemObject.hpp"
#include "NiSmartPointer.hpp"
#include "NiGlobalStringTable.hpp"

class NiFixedString : public NiMemObject {
public:
	NiFixedString();
	NiFixedString(const char* pcString);
	NiFixedString(const NiFixedString& kString);
	~NiFixedString();

	NiGlobalStringTable::GlobalStringHandle m_kHandle;

	NiFixedString& operator=(const char* pcString);
	NiFixedString& operator=(NiFixedString& arString);
	bool operator==(const NiFixedString& akString);
	bool operator==(const char* pcString);

	UInt32 GetLength() const;

	std::string GetStd() const {
		if (((const char*)this)[0] != 0 && ((const char*)this)[0] != ' ')
			return (const char*)this;
		return "";
	}
};

ASSERT_SIZE(NiFixedString, 0x4)