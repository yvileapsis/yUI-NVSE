#pragma once

#include "NiProperty.hpp"

NiSmartPointer(NiStencilProperty);

class NiStencilProperty : public NiProperty {
public:
	NiStencilProperty();
	virtual ~NiStencilProperty();

	enum {
		ENABLE_MASK = 0x1,
		FAILACTION_MASK = 0xE,
		FAILACTION_POS = 0x1,
		ZFAILACTION_MASK = 0x70,
		ZFAILACTION_POS = 0x4,
		PASSACTION_MASK = 0x380,
		PASSACTION_POS = 0x7,
		DRAWMODE_MASK = 0xC00,
		DRAWMODE_POS = 0xA,
		TESTFUNC_MASK = 0xF000,
		TESTFUNC_POS = 0xC,
	};

	enum DrawMode {
		DRAW_CCW_OR_BOTH = 0x0,
		DRAW_CCW = 0x1,
		DRAW_CW = 0x2,
		DRAW_BOTH = 0x3,
		DRAW_MAX,
	};

	Bitfield16	m_usFlags;
	UInt32		m_uiRef;
	UInt32		m_uiMask;

	CREATE_OBJECT(NiStencilProperty, 0xA6F410);

	bool IsEnabled() {
		return m_usFlags.GetBit(ENABLE_MASK);
	}

	void SetDrawMode(NiStencilProperty::DrawMode aeDraw) {
		m_usFlags.SetField(aeDraw, DRAWMODE_MASK, DRAWMODE_POS);
	}
};

ASSERT_SIZE(NiStencilProperty, 0x24);