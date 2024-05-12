#pragma once

#include "BSShaderProperty.hpp"
#include "BSString.hpp"

NiSmartPointer(SkyShaderProperty);

class SkyShaderProperty : public BSShaderProperty {
public:
	SkyShaderProperty();
	virtual ~SkyShaderProperty();

	enum SkyObject : UInt32 {
		SO_SUN = 0x0,
		SO_SUN_GLARE = 0x1,
		SO_ATMOSPHERE = 0x2,
		SO_CLOUDS = 0x3,
		SO_SKYQUAD = 0x4,
		SO_STARS = 0x5,
		SO_MOON = 0x6,
		SO_MOON_SHADOW = 0x7,
		SO_OTHER = 0x8,
		SO_COUNT = 0x9,
	};

	virtual int								Unk_2F();
	virtual void							Unk_30(int);
	virtual NiTexturingProperty::ClampMode	GetClampMode();
	virtual void							SetClampMode(NiTexturingProperty::ClampMode aeClamp);


	NiColorA kVertexColor;
	NiSourceTexture* pBaseTexture;
	BSStringT sTextureName;
	NiTexturingProperty::ClampMode eClampMode;
	NiSourceTexture* pBlendTexture;
	float fBlendValue;
	UInt16 usCloudLayer;
	SkyObject eSkyObjectType;

	CREATE_OBJECT(SkyShaderProperty, 0xBAEE40)
};

ASSERT_SIZE(SkyShaderProperty, 0x90);