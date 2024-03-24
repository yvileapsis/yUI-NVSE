#pragma once

#include "NiPSysCollider.hpp"
#include "NiAVObject.hpp"

NiSmartPointer(NiPSysPlanarCollider);

class NiPSysPlanarCollider : public NiPSysCollider {
public:
	NiPSysPlanarCollider();
	virtual ~NiPSysPlanarCollider();

	NiAVObject*		m_pkColliderObj;
	float			m_fWidth;
	float			m_fHeight;
	NiPoint3		m_kXAxis;
	NiPoint3		m_kYAxis;
	float			m_fHalfSqScaledWidth;
	float			m_fHalfSqScaledHeight;
	NiPlane			m_kPlane;
	NiPoint3		m_kPosition;
	NiMatrix3		m_kRotation;
	NiTransform		m_kLastColliderTransform;
	NiTransform		m_kLastPSysTransform;

	CREATE_OBJECT(NiPSysPlanarCollider, 0xC29AF0)
};

ASSERT_SIZE(NiPSysPlanarCollider, 0x100);