#pragma once

#include "BSShaderProperty.hpp"
#include "BSString.hpp"

NiSmartPointer(BSShaderNoLightingProperty);

class BSShaderNoLightingProperty : public BSShaderProperty {
public:
	BSShaderNoLightingProperty();
	virtual ~BSShaderNoLightingProperty();

	// Returns 1
	virtual UInt32 Unk_31();
	// 3 = clamp, anything else = wrap
	virtual void PickClampMode(UInt32 aeMode); 
	virtual NiTexturingProperty::ClampMode GetClampMode();
	virtual void SetClampMode(NiTexturingProperty::ClampMode aeMode);

	struct FalloffData {
		float fFalloffStartAngle;
		float fFalloffStopAngle;
		float fFalloffStartOpacity;
		float fFalloffStopOpacity;
	};

	NiTexturePtr					spTexture;
	BSStringT						strTexturePath;
	NiTexturingProperty::ClampMode	eClampMode;
	FalloffData						kFalloffData;

	CREATE_OBJECT(BSShaderNoLightingProperty, 0xB6FE80);

	void SetTexture(NiTexture* apTexture);

	static RenderPassArray* __fastcall GetRenderPassesEx(BSShaderNoLightingProperty* apThis, void*, NiGeometry* apGeometry, UInt32 auiEnabledPasses, WORD* pusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
};

ASSERT_SIZE(BSShaderNoLightingProperty, 0x80);