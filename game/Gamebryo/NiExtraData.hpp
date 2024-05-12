#pragma once

#include "NiObject.hpp"
#include "NiFixedString.hpp"

NiSmartPointer(NiExtraData);

class NiExtraData : public NiObject {
public:
	NiExtraData();
	virtual ~NiExtraData();

	NiFixedString m_kName;

	CREATE_OBJECT(NiExtraData, 0xA7B380);

	const char* GetName() const { return m_kName.m_kHandle; }

	static void __fastcall LoadBinaryEx(NiExtraData* apThis, void*, NiStream* kStream);
};

ASSERT_SIZE(NiExtraData, 0xC);