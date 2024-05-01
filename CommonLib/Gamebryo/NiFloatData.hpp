#pragma once

#include "NiObject.hpp"
#include "NiFloatKey.hpp"

NiSmartPointer(NiFloatData);

class NiFloatData : public NiObject {
public:
	NiFloatData();
	virtual ~NiFloatData();

	UInt32				m_uiNumKeys;
	NiFloatKey*			m_pkKeys;
	NiFloatKey::KeyType m_eType;
	UInt8			m_ucKeySize;
};

ASSERT_SIZE(NiFloatData, 0x18);