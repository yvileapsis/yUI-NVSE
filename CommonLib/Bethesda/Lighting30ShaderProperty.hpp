#pragma once

#include "BSShaderPPLightingProperty.hpp"

NiSmartPointer(Lighting30ShaderProperty);

class Lighting30ShaderProperty : public BSShaderPPLightingProperty {
public:
	Lighting30ShaderProperty();
	virtual ~Lighting30ShaderProperty();

	int unk104;

	CREATE_OBJECT(Lighting30ShaderProperty, 0xBB45D0);
	NIRTTI_ADDRESS(0x12007B0);


	static Lighting30ShaderProperty* Create(BSShaderPPLightingProperty* apProperty);
};

ASSERT_SIZE(Lighting30ShaderProperty, 0x108);

typedef Lighting30ShaderProperty NoFaderShaderProperty;