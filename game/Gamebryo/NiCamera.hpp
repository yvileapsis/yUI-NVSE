#pragma once

#include "NiAVObject.hpp"
#include "NiRect.hpp"
#include "NiFrustum.hpp"

NiSmartPointer(NiCamera);

class NiCamera : public NiAVObject {
public:
	NiCamera();
	virtual ~NiCamera();

	float			m_aafWorldToCam[4][4];
	NiFrustum		m_kViewFrustum;
	float			m_fMinNearPlaneDist;
	float			m_fMaxFarNearRatio;
	NiRect<float>	m_kPort;
	float			m_fLODAdjust;

	CREATE_OBJECT(NiCamera, 0xA71430);

	[[nodiscard]] const NiFrustum* GetFrustum() const { return &m_kViewFrustum; }

	[[nodiscard]] const NiRect<float>* GetViewport() const { return &m_kPort; }

	void SetViewport(const NiRect<float>& kPort) {
		m_kPort = kPort;
	}

	__forceinline NiPoint3 GetWorldDirection() const {
		NiPoint3 kDVector;
		m_kWorld.m_Rotate.GetCol(0, kDVector);
		return kDVector;
	}

	__forceinline NiPoint3 GetWorldUpVector() const {
		NiPoint3 kUVector;
		m_kWorld.m_Rotate.GetCol(1, kUVector);
		return kUVector;
	}
	__forceinline NiPoint3 GetWorldRightVector() const {
		NiPoint3 kRVector;
		m_kWorld.m_Rotate.GetCol(2, kRVector);
		return kRVector;
	}

	void SetViewFrustum(const NiFrustum& kFrustum);

	bool LookAtWorldPoint(const NiPoint3& kWorldPt, const NiPoint3& kWorldUp);
	bool FitNearAndFarToBound(const NiBound& kBound);

	bool WorldPtToScreenPt(NiPoint3& kPt, float& fBx, float& fBy, float fZeroTolerance);

	void WorldToCameraMatrix();
};

ASSERT_SIZE(NiCamera, 0x114);