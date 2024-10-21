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
		if (m_kHandle[0] != 0 && m_kHandle[0] != ' ')
			return m_kHandle;
		return "";
	}
};

ASSERT_SIZE(NiFixedString, 0x4)