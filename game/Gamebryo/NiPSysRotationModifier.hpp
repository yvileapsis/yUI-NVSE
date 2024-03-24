#pragma once

#include "NiPSysModifier.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(NiPSysRotationModifier);

class NiPSysRotationModifier : public NiPSysModifier {
public:
	NiPSysRotationModifier();
	virtual ~NiPSysRotationModifier();

	float		m_fInitialRotSpeed;
	float		m_fInitialRotSpeedVar;
	float		m_fInitialRotAngle;
	float		m_fInitialRotAngleVar;
	NiPoint3	m_kInitialAxis;
	bool		m_bRandomInitialAxis;
	bool		m_bRandomRotSpeedSign;

	CREATE_OBJECT(NiPSysRotationModifier, 0xC20640)
};

ASSERT_SIZE(NiPSysRotationModifier, 0x38);