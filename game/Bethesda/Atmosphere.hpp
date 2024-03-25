#pragma once
#include "SkyObject.hpp"

class BSFogProperty;

// 0x1C
class Atmosphere : public SkyObject
{
public:
	virtual void		Init(NiNode* niNode, void* _fogProp);

	NiPointer<NiNode>			spAtmosphere;
	NiPointer<BSFogProperty>	spFog;
	NiPointer<NiNode>			spSkyQuadNode;
	NiPointer<NiRefObject>		spSkyQuad;
	bool						bUpdateFogDistance;
};
static_assert(sizeof(Atmosphere) == 0x1Cu);