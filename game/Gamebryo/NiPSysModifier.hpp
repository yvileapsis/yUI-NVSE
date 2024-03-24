#pragma once

#include "NiObject.hpp"
#include "NiFixedString.hpp"

class NiParticleSystem;

NiSmartPointer(NiPSysModifier);

class NiPSysModifier : public NiObject {
public:
	NiPSysModifier();
	virtual ~NiPSysModifier();

	virtual void Unk23();
	virtual void Unk24();
	virtual void Unk25();
	virtual void SetActive(bool bActive);
	virtual void SetSystemPointer(NiParticleSystem* pkTarget);
	virtual void HandleReset();

	NiFixedString		m_kName;
	UInt32				m_uiOrder;
	NiParticleSystem*	m_pkTarget;
	bool				m_bActive;
};

ASSERT_SIZE(NiPSysModifier, 0x18)