#pragma once

#include "BSShaderPPLightingProperty.hpp"

NiSmartPointer(HairShaderProperty);

class HairShaderProperty : public BSShaderPPLightingProperty {
public:
	HairShaderProperty();
	~HairShaderProperty();

	NiRefObject* dword104;
	NiRefObject* dword108;
	NiPoint3 nipoint310C;
	NiPoint3 nipoint3118;
	NiPoint3 nipoint3124;
	float float130;
	float float134;
	float float138;
	float float13C;
	float float140;
	float float144;
	float float148;
	float float14C;
	float float150;
	float float154;
	float float158;
	float float15C;
	float float160;
	float float164;
	float float168;
	float float16C;
	float float170;
	float float174;
	float float178;
	NiTexture* heightMap;

	CREATE_OBJECT(HairShaderProperty, 0xBC4340)
};

ASSERT_SIZE(HairShaderProperty, 0x180);