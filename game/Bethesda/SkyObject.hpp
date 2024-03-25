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

	NiPointer<NiNode> spRoot;
};
static_assert(sizeof(SkyObject) == 0x8);