#pragma once

#include "NiColor.hpp"

struct ImageSpaceBaseData {
	float unk000;
	float unk004;
	float fSkinDimmer;
	float fEmissiveHDRMult;
	float fTargetLUM;
	float fUpperLUMClamp;
	float fBrightScale;
	float fBrightClamp;
	float fSunlightDImmer;
	float unk024;
	float unk028;
	float fSunlightDimmer;
	float fGrassDimmer;
	float fTreeDimmer;
	float unk038;
	float unk03C;
	float unk040;
	float unk044;
	float unk048;
	float unk04C;
	float unk050;
	float unk054;
	float unk058;
	float unk05C;
	float unk060;
	float unk064;
	float unk068;
	float unk06C;
	float unk070;
	NiColorA someScreenColor;
	float unk088;
	float unk08C;
	float unk090;
	float unk094;
	float unk98;
};

ASSERT_SIZE(ImageSpaceBaseData, 0x98);