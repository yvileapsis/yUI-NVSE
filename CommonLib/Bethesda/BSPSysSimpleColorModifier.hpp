#pragma once

#include "NiPSysModifier.hpp"
#include "NiColor.hpp"

NiSmartPointer(BSPSysSimpleColorModifier);

class BSPSysSimpleColorModifier : public NiPSysModifier {
public:
	BSPSysSimpleColorModifier();
	~BSPSysSimpleColorModifier();

	NiColorA Color1;
	NiColorA Color2;
	NiColorA Color3;
	float fFadeIn;
	float fFadeOut;
	float fColor1End;
	float fColor2Start;
	float fColor2End;
	float fColor3Start;

	CREATE_OBJECT(BSPSysSimpleColorModifier, 0xC607D0)
};

ASSERT_SIZE(BSPSysSimpleColorModifier, 0x60)