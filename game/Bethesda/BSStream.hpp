#pragma once

#include "NiStream.hpp"
#include "BSNodeReferences.hpp"

NiSmartPointer(BSStream);

class BSStream : public NiStream {
public:
	BSStream();
	virtual ~BSStream();

	virtual bool SetFileAndName(const char* pcSrc, NiBinaryStream* pkIstr);

	static BSStream* Create(BSStream* apThis);

	NiPointer<BSNodeReferences> spNodeReferences;
};

ASSERT_SIZE(BSStream, 0x5CC)