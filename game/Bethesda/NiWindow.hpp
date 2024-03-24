#pragma once

#include "NiRefObject.hpp"
#include "BSSimpleList.hpp"

class NiAVObject;
class BSRenderedTexture;
class ShadowSceneNode;
class BSShaderAccumulator;
class NiCamera;

// parent : Renderer
class NiWindow {
public:
	NiWindow();
	virtual void Render(NiCamera* apCamera, ShadowSceneNode* apNode, BSRenderedTexture* apTexture, bool abWireframe);
	virtual bool SetObject4(NiAVObject* apObject);
	virtual void Unk_2(NiPointer<NiCamera>* aspCamera);
	virtual bool Update();
	virtual bool UpdateCamera(NiCamera* apCamera, UInt32 auiWidth, UInt32 auiHeight, float afEndClipDist);

	NiRefObjectPtr					spObject4;
	UInt32							uiFrameCount;
	BSSimpleList<NiRefObjectPtr>	kList0C;
	NiPointer<BSShaderAccumulator>	spAccumulator;
};

ASSERT_SIZE(NiWindow, 0x18);