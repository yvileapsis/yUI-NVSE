#pragma once

#include "NiNode.hpp"

class Sky;

NiSmartPointer(SkyObject);

class SkyObject {
public:
	virtual				~SkyObject();
	virtual NiNode*		GetSkyObject();
	virtual void		InitSkyObject(NiNode* apRoot);
	virtual void		Update(Sky* apSky, float afValue);

	NiNodePtr spRoot;
};

ASSERT_SIZE(SkyObject, 0x8)