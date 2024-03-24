#pragma once

#include "NiNode.hpp"

class Precipitation {
public:
	virtual Precipitation* Destroy(bool doFree);

	NiNodePtr spCurrentPrecip;
	NiNodePtr spLastPrecip;
	NiNode* pWeatherRootNode;
	float unk10;
	UInt32 unk14;
};