#pragma once

#include "BSShaderProperty.hpp"

NiSmartPointer(BSDistantTreeShaderProperty);

class BSDistantTreeShaderProperty : public BSShaderProperty {
public:
	BSDistantTreeShaderProperty();
	~BSDistantTreeShaderProperty();

	UInt32	uiMaxInstanceCount;
	float*	pfInstanceData;

	CREATE_OBJECT(BSDistantTreeShaderProperty, 0xBB7300);

	void SetInstanceData(const UInt32 auiID, const NiPoint3& arPos, const float afRotZ, const float afScale);

	static RenderPassArray* __fastcall GetRenderPassesEx(BSDistantTreeShaderProperty* apThis, void*, NiGeometry* apGeometry, UInt32 auiEnabledPasses, WORD* pusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
};

ASSERT_SIZE(BSDistantTreeShaderProperty, 0x68);