#pragma once

#include "NiPoint3.hpp"
#include "NIQuaternion.hpp"

class NiQuatTransform {
public:
	NiQuatTransform();
	~NiQuatTransform();

	NiPoint3		m_kTranslate;
	NiQuaternion	m_kRotate;
	float		   m_fScale;
};

ASSERT_SIZE(NiQuatTransform, 0x20)