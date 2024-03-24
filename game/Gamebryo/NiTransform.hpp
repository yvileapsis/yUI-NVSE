#pragma once

#include "NiMatrix3.hpp"
#include "NiPoint3.hpp"

class NiTransform {
public:
	NiTransform();

	NiMatrix3	m_Rotate;
	NiPoint3	m_Translate;
	float		m_fScale;

	// 0x62C250
	inline NiTransform operator*(const NiTransform& xform) const {
		NiTransform res;

		res.m_fScale = m_fScale * xform.m_fScale;
		res.m_Rotate = m_Rotate * xform.m_Rotate;
		res.m_Translate = m_Translate + m_fScale * (m_Rotate * xform.m_Translate);

		return res;
	};

	// 0x524C40
	inline NiPoint3 operator*(const NiPoint3& kPoint) const {
		return(((m_Rotate * kPoint) * m_fScale) + m_Translate);
	};

	void MakeIdentity();
	void Invert(NiTransform& kDest);
	static void __fastcall Invert_Hook(NiTransform* apThis, void*, NiTransform& kDest);

};

ASSERT_SIZE(NiTransform, 0x34);