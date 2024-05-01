#pragma once

#include "BSShaderProperty.hpp"

NiSmartPointer(VolumetricFogShaderProperty);

class VolumetricFogShaderProperty : public BSShaderProperty {
public:
	VolumetricFogShaderProperty();
	virtual ~VolumetricFogShaderProperty();

	NiColorA kFogProps;

	CREATE_OBJECT(VolumetricFogShaderProperty, 0xBC38C0);

	static BSShaderProperty::RenderPassArray* __fastcall GetRenderPassesEx(VolumetricFogShaderProperty* apThis, void*, NiGeometry* apGeometry, UInt32 auiEnabledPasses, UInt16* pusPassCount, int a5, BSShaderAccumulator* apAccumulator, int a7);
};

ASSERT_SIZE(VolumetricFogShaderProperty, 0x70);