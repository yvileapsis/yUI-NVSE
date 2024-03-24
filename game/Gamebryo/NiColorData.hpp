#pragma once

#include "NiObject.hpp"
#include "NiColorKey.hpp"

NiSmartPointer(NiColorData);

class NiColorData : public NiObject {
public:
	NiColorData();
	virtual ~NiColorData();

	UInt32					m_uiNumKeys;
	NiColorKey*				m_pkKeys;
	NiColorKey::KeyType		m_eType;
	UInt8					m_ucKeySize;
};

ASSERT_SIZE(NiColorData, 0x18);