#pragma once

#include "hkaBone.hpp"
#include "hkArray.hpp"
#include "hkQsTransform.hpp"
#include "hkReferencedObject.hpp"
#include "hkStringPtr.hpp"

class hkaSkeleton : public hkReferencedObject {
public:
	hkStringPtr				m_name;
	hkArray<UInt16>			m_parentIndices;
	hkArray<hkaBone>		m_bones;
	hkArray<hkQsTransform>	m_referencePose;
	hkArray<UInt32>			m_floatSlots;
	hkArray<UInt32>			m_localFrames;
};

ASSERT_SIZE(hkaSkeleton, 0x48);