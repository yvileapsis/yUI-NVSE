#pragma once

#include "BSShaderProperty.hpp"

class BSShaderBloodSplatterProperty : public BSShaderProperty {
public:
	NiTexturePtr	spTexture[2];
	DWORD			dword68;
	float*			pfFadeAlpha;

	struct SplatterGeometry {
		NiGeometry*	pGeometry;
		RenderPass*	pPass;
	};


	static UInt32* const uiShapeCount;
	static SplatterGeometry* const pShapes;

	static __forceinline UInt32 GetShapeCount() { return *(UInt32*)0x11FF0F0; };
	static __forceinline void SetShapeCount(UInt32 uiCount) { *(UInt32*)0x11FF0F0 = uiCount; };

	void SetTexture(UInt32 auiIndex, NiTexture* apTexture);
	static void SetFadeSourceRecurse(NiAVObject* apObject, float* apfFadeAlpha);

	static RenderPassArray* __fastcall GetRenderPassesEx(BSShaderBloodSplatterProperty* apThis, void*, NiGeometry* apGeometry, UInt32 auiEnabledPasses, WORD* pusPassCount, UInt32 aeRenderMode, BSShaderAccumulator* apAccumulator, bool abAddPass);
};

ASSERT_SIZE(BSShaderBloodSplatterProperty, 0x70)