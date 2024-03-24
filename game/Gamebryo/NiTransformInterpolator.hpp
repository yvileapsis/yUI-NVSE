#pragma once

#include "NiKeyBasedInterpolator.hpp"
#include "NiQuatTransform.hpp"

NiSmartPointer(NiTransformData);
NiSmartPointer(NiTransformInterpolator);

class NiTransformInterpolator : public NiKeyBasedInterpolator {
public:
	NiQuatTransform		m_kTransformValue;
	NiTransformDataPtr	m_spData;
	UInt16				m_uiLastTransIdx;
	UInt16				m_uiLastRotIdx;
	UInt16				m_uiLastScaleIdx;
	UInt32				unk38;
	UInt32				unk3C;
	UInt32				unk40;
	bool				bPose;
};

ASSERT_SIZE(NiTransformInterpolator, 0x48);