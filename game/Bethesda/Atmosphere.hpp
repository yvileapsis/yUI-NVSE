#pragma once

#include "SkyObject.hpp"

class BSFogProperty;

class Atmosphere : public SkyObject {
public:
	virtual void		Init(NiNode* niNode, void* _fogProp);

	NiNodePtr					spAtmosphere;
	NiPointer<BSFogProperty>	spFog;
	NiNodePtr					spSkyQuadNode;
	NiRefObjectPtr				spSkyQuad;
	bool						bUpdateFogDistance;
};

ASSERT_SIZE(Atmosphere, 0x1Cu);