#pragma once

#include "NiInterpolator.hpp"

class NiTransformInterpolator;

class NiBlendInterpolator : public NiInterpolator {
public:
	struct InterpArrayItem {
		NiInterpolatorPtr			m_spInterpolator;
		float						m_fWeight;
		float						m_fNormalizedWeight;
		SInt8						m_cPriority;
		float						m_fEaseSpinner;
		float						m_fUpdateTime;
	};

	UInt8				ucFlags;
	UInt8				m_ucArraySize;
	UInt8				m_ucInterpCount;
	UInt8				m_ucSingleIdx;
	SInt8				m_cHighPriority;
	SInt8				m_cNextHighPriority;
	InterpArrayItem*	m_pkInterpArray;
	NiInterpolator*		m_pkSingleInterpolator;
	float				m_fWeightThreshold;
	float				m_fSingleTime;
	float				m_fHighSumOfWeights;
	float				m_fNextHighSumOfWeights;
	float				m_fHighEaseSpinner;

	NIRTTI_ADDRESS(0x11F36FC);
};