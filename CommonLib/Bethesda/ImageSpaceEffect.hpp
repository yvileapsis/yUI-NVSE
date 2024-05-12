#pragma once

#include "ImageSpaceTexture.hpp"
#include "ImageSpaceShaderParam.hpp"
#include "ImageSpaceEffectParam.hpp"
#include "BSGraphics.hpp"

class ImageSpaceManager;
class NiScreenGeometry;
class NiDX9Renderer;
class ImageSpaceShader;

class ImageSpaceEffect {
public:
	virtual ~ImageSpaceEffect();
	virtual void RenderShader(NiGeometry* apScreenShape, NiDX9Renderer* apRenderer, ImageSpaceEffectParam* apParam, bool abEndFrame);
	virtual void Setup(ImageSpaceManager* pISManager, ImageSpaceEffectParam* apParam);
	virtual void Shutdown();
	virtual void BorrowTextures(ImageSpaceEffectParam* apParam);
	virtual void ReturnTextures();
	virtual bool IsActive() const;
	virtual bool UpdateParams(ImageSpaceEffectParam* apParam);

	struct EffectInput {
		SInt32			  iTexIndex;
		TextureFilterMode iFilterMode;
	};


	bool										bIsActive;
	bool										bParamsChanged;
	NiTPrimitiveArray<ImageSpaceShader*>		kShaders;
	NiTPrimitiveArray<ImageSpaceShaderParam*>	kShaderParams;
	NiTPrimitiveArray<ImageSpaceTexture*>		kTextures;
	NiTPrimitiveArray<EffectInput*>				kShaderInputs;
	NiTPrimitiveArray<UInt32*>					kShaderOutputs;

	void SetTexture(UInt32 aiIndex, ImageSpaceTexture* apTex, TextureFilterMode aiFilterMode) {
		ThisStdCall(0xBA3CB0, this, aiIndex, apTex, aiFilterMode);
	}
};

ASSERT_SIZE(ImageSpaceEffect, 0x58)