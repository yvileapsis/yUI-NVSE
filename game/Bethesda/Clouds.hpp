#pragma once

#include "SkyObject.hpp"
#include "NiColor.hpp"

class NiTriStrips;
class NiSourceTexture;

class Clouds : public SkyObject {
public:
	NiTriStrips* spClouds[4];
	NiSourceTexture* spTransTexture[4];
	NiColor pColors[4];
	UInt16 usNumLayers;
	bool bForceUpdate;
};