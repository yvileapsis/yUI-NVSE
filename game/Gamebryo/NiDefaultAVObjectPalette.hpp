#pragma once

#include "NiAVObjectPalette.hpp"
#include "NiTFixedStringMap.hpp"

NiSmartPointer(NiDefaultAVObjectPalette);

class NiDefaultAVObjectPalette : public NiAVObjectPalette {
	NiDefaultAVObjectPalette();
	virtual ~NiDefaultAVObjectPalette();

	virtual void			SetScene(NiAVObject* apScene);
	virtual NiAVObject*		GetScene();

	NiTFixedStringMap<NiAVObject*>	m_kHash;
	NiAVObject*						m_pkScene;

	CREATE_OBJECT(NiDefaultAVObjectPalette, 0xA6F000);
};

ASSERT_SIZE(NiDefaultAVObjectPalette, 0x1C)