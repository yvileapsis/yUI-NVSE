#pragma once

#include "BSTextureSet.hpp"
#include "BSString.hpp"

NiSmartPointer(BSShaderTextureSet);

class BSShaderTextureSet : public BSTextureSet {
public:
	BSShaderTextureSet();
	~BSShaderTextureSet();

	BSStringT strTextures[6];

	CREATE_OBJECT(BSShaderTextureSet, 0xBA8A00)
};

ASSERT_SIZE(BSShaderTextureSet, 0x38);