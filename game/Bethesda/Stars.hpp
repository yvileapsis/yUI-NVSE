#pragma once

#include "SkyObject.hpp"

class NiNode;

class Stars : public SkyObject {
public:
	NiNodePtr spStars;
	float fAlpha;
};