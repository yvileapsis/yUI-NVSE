#pragma once

#include "NiRefObject.hpp"

class NiSourceTexture;

class BSFaceGenImage : public NiRefObject {
public:
	NiPointer<NiSourceTexture>	spTexture;
	BYTE						unkD[24];
	UInt32						uiWidth;
	UInt32						uiHeight;
};

ASSERT_SIZE(BSFaceGenImage, 0x2C);