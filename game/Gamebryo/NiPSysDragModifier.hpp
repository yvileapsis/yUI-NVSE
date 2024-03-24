#pragma once

#include "NiPSysModifier.hpp"
#include "NiAVObject.hpp"
#include "NiPoint3.hpp"

NiSmartPointer(NiPSysDragModifier);

class NiPSysDragModifier : public NiPSysModifier {
public:
	NiPSysDragModifier();
	virtual ~NiPSysDragModifier();

	NiAVObject*		m_pkDragObj;
	NiPoint3		m_kDragAxis;
	float			m_fPercentage;
	float			m_fRange;
	float			m_fFalloff;

	CREATE_OBJECT(NiPSysDragModifier, 0xC2C080)
};

ASSERT_SIZE(NiPSysDragModifier, 0x34);