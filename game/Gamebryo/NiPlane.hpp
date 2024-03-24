#pragma once

#include "NiPoint3.hpp"

class NiPlane {
public:
	NiPlane() : m_kNormal(0.0f, 0.0f, 0.0f), m_fConstant(0.0f) {}
	NiPlane(const NiPoint3& kNormal, float fConstant) : m_kNormal(kNormal), m_fConstant(fConstant) {}
	NiPlane(const NiPoint3& kNormal, const NiPoint3& kPoint) : m_kNormal(kNormal), m_fConstant(kNormal.Dot(kPoint)) {}
	NiPlane(const NiPoint3& kPoint0, const NiPoint3& kPoint1, const NiPoint3& kPoint2) {
		NiPoint3 kDif1 = kPoint1 - kPoint0;
		NiPoint3 kDif2 = kPoint2 - kPoint1;
		m_kNormal = kDif1.UnitCross(kDif2);
		m_fConstant = m_kNormal * kPoint0;
	}

	NiPoint3	m_kNormal;
	float		m_fConstant;

#ifdef USE_DX_COLLISION
	NiPlane(const XMFLOAT4& arPlane) {
		m_kNormal = NiPoint3(arPlane);
		m_fConstant = arPlane.w;
	}

	XMFLOAT4* GetPlanePtr() const {
		return (XMFLOAT4*)(this);
	}

	XMVECTOR GetPlaneV() const {
		return XMLoadFloat4(GetPlanePtr());
	}

	XMVECTOR GetNormalV() {
		return XMLoadFloat3(&m_kNormal);
	}

	NiPlane operator=(const XMFLOAT4& arCopy) {
		m_kNormal = NiPoint3(arCopy);
		m_fConstant = arCopy.w;
		return *this;
	}

	bool operator==(const XMFLOAT4& arOther) const {
		return m_kNormal == NiPoint3(arOther) && m_fConstant == arOther.w;
	}
#endif

	enum Sides {
		NO_SIDE			= 0,
		POSITIVE_SIDE	= 1,
		NEGATIVE_SIDE	= 2,
	};

	float Distance(const NiPoint3& arPoint) const;
	Sides WhichSide(const NiPoint3& arPoint) const;
	static UInt32 __fastcall WhichSide_Hook(NiPlane* apThis, void*, NiPoint3& arPoint);
};

ASSERT_SIZE(NiPlane, 0x10);