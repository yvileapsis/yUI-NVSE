#pragma once

#include "ImageSpaceEffect.hpp"

class DepthOfFieldTextures : public ImageSpaceEffect {
public:
	NiTPrimitiveArray<bool> kTextureStates;
};

class ImageSpaceEffectDepthOfField : public DepthOfFieldTextures {
public:
	float				float68;
	float				float6C;
	float				fStrength;
	DWORD				dword74;
	float				fUnk78;
	ImageSpaceTexture	kDOFMask;
	ImageSpaceTexture	kBuffers[3];

	bool IsStrengthActive() const { return fStrength > 0.0f; }

	BSRenderedTexture* GetDOFMask();

	void Toggle(bool bEnable) { *(bool*)0x12023D4 = bEnable; }

	void ClearTexture() { kDOFMask.Reset(); }

	void DoStandard();
	void DoMasked();
};

ASSERT_SIZE(ImageSpaceEffectDepthOfField, 0xBC);