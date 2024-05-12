#pragma once

#include "NiPoint3.hpp"

ALIGN16 class hkVector4 {
public:
	hkVector4() : m_quad(_mm_set1_ps(0.0f)) {}

	hkVector4(const float& a_x) : m_quad(_mm_set1_ps(a_x)) {}

	hkVector4(const float& a_x, const float& a_y, const float& a_z, const float& a_w) :
		m_quad(_mm_setr_ps(a_x, a_y, a_z, a_w))
	{}

	hkVector4(const hkVector4& a_rhs) : m_quad(a_rhs.m_quad) {}

	hkVector4(const NiPoint3& a_point) :
		m_quad(_mm_setr_ps(a_point.x, a_point.y, a_point.z, 0.0f))
	{}

	__m128 m_quad;
};
ASSERT_SIZE(hkVector4, 0x10);