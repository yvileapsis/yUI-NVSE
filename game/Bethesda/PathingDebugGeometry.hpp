#pragma once

#include "NiRefObject.hpp"

// GECK
class PathingDebugGeometry : public NiRefObject {
public:
	NiRefObjectPtr	spObject08;
	NiRefObjectPtr	spObject0C;
	NiRefObjectPtr	spObject10;
	NiRefObjectPtr	spObject14;
	NiRefObjectPtr	spObject18;
	NiRefObjectPtr	spObject1C;
	NiRefObjectPtr	spObject20;
	NiRefObjectPtr	spObject24;
	NiRefObjectPtr	spObject28;
	NiRefObjectPtr	spObject2C;
	NiRefObjectPtr	spObject30;
	NiRefObjectPtr	spObject34;
	NiRefObjectPtr	spObject38;
	NiRefObjectPtr	spObject3C;
	bool			byte40;
};

ASSERT_SIZE(PathingDebugGeometry, 0x44)