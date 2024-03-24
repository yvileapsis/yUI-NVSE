#pragma once

#include "NiRefObject.hpp"
#include "NiTexture.hpp"

class NiTexturePalette : public NiRefObject {
public:
	NiTexture* m_kHash[1];
	bool m_bSearchGlobalTextureList;
};