#pragma once

#include "BSShaderProperty.hpp"

class PrecipitationShaderProperty : public BSShaderProperty {
public:
	UInt32	iParticleCount;
	float	float64;
	float	float68;
	NiColor color6C;
	NiColor color78;
	NiColor color8C;
	void*	pBaseTexture;
	DWORD	dword94;
	DWORD	eBillboardMode;
	DWORD	dword9C;
};

ASSERT_SIZE(PrecipitationShaderProperty, 0xA0)