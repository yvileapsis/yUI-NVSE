#pragma once

#include "NiObject.hpp"

NiSmartPointer(NiPalette);

class NiPalette : public NiObject {
public:
	NiPalette();
	virtual ~NiPalette();

	struct PaletteEntry {
		UInt8 m_ucRed;
		UInt8 m_ucGreen;
		UInt8 m_ucBlue;
		UInt8 m_ucAlpha;
	};

	struct RendererData {
		NiPalette* m_pkPalette;
	};

	bool m_bAlpha;
	UInt32 m_uiEntries;
	UInt32 m_uiRevID;
	PaletteEntry* m_pkPalette;
	RendererData* m_pkRendererData;
	NiPalette* m_pkPrev;
	NiPalette* m_pkNext;

	CREATE_OBJECT(NiPalette, 0xA8B220)
};

ASSERT_SIZE(NiPalette, 0x24)