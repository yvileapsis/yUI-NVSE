#pragma once

#include "hkArray.hpp"
#include "hkQsTransform.hpp"

class hkaSkeleton;

class hkaPose {
public:
	hkaSkeleton*			m_skeleton;
	hkArray<hkQsTransform>	m_localPose;
	hkArray<hkQsTransform>	m_modelPose;
	hkArray<UInt32>			m_boneFlags;
	bool					m_modelInSync;
	bool					m_localInSync;
	hkArray<float>			m_floatSlotValues;
};

ASSERT_SIZE(hkaPose, 0x38);