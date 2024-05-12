#pragma once

#include "NiObject.hpp"
#include "NiFloatKey.hpp"
#include "NiBoolKey.hpp"

NiSmartPointer(NiPSysEmitterCtlrData);

class NiPSysEmitterCtlrData : public NiObject {
public:
	NiPSysEmitterCtlrData();
	virtual ~NiPSysEmitterCtlrData();

	UInt32				m_uiNumBirthRateKeys;
	NiFloatKey*			m_pkBirthRateKeys;
	NiFloatKey::KeyType m_eBirthRateKeyType;
	UInt8				m_ucBirthRateSize;
	UInt32				m_uiNumEmitterActiveKeys;
	NiBoolKey*			m_pkEmitterActiveKeys;
	NiBoolKey::KeyType	m_eEmitterActiveKeyType;
	UInt8				m_ucEmitterActiveSize;

	CREATE_OBJECT(NiPSysEmitterCtlrData, 0xC2BA60)
};

ASSERT_SIZE(NiPSysEmitterCtlrData, 0x28);