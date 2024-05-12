#pragma once

#include "NiInterpController.hpp"
#include "NiMorphData.hpp"

class NiGeomMorpherController : public NiInterpController {
public:
	NiGeomMorpherController();
	virtual ~NiGeomMorpherController();

	class InterpItem : public NiMemObject {
	public:
		NiInterpolatorPtr m_spInterpolator;
		float m_fWeight;
	};

	UInt16			m_usFlags;
	NiMorphData*	m_spMorphData;
	InterpItem*		m_pkInterpItems;
	bool			m_bNeedPreDisplay;
	bool			m_bIgnoreBaseTarget;
	bool			m_bAlwaysUpdate;
	bool			m_bBoundCalculated;

	NIRTTI_ADDRESS(0x11F3728);
};

ASSERT_SIZE(NiGeomMorpherController, 0x44);