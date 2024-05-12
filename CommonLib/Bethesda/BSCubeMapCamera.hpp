#pragma once

#include "NiCamera.hpp"
#include "NiTPointerList.hpp"
#include "BSRenderedTexture.hpp"
#include "NiNode.hpp"

class BSShaderAccumulator;
class ShadowSceneNode;
class ShadowSceneLight;

NiSmartPointer(BSCubeMapCamera);

class BSCubeMapCamera : public NiCamera {
public:
	virtual void UnkRender(UInt32 unk);
	enum etFace
	{
		FACE_POS_X = 0x0,
		FACE_NEG_X = 0x1,
		FACE_POS_Y = 0x2,
		FACE_NEG_Y = 0x3,
		FACE_POS_Z = 0x4,
		FACE_NEG_Z = 0x5,
		FACE_NUM = 0x6,
	};

	UInt32							uiLastUpdatedCamera; // named for my own purposes
	BSRenderedTexturePtr			spFaces[FACE_NUM];
	BSRenderedTexturePtr			spTexture;
	ShadowSceneLight*				pLight;
	NiNodePtr						spSceneNode;
	NiPointer<BSShaderAccumulator>	spAccumulator;

	static BSCubeMapCamera* CreateObject();
	static BSCubeMapCamera* Create(NiNode* apSceneNode, float afViewDistance, UInt32 auiTextureSize = 0, D3DFORMAT aeFormat = D3DFMT_UNKNOWN);

	void SetupTransformation(UInt32 auiFace);

	void RenderCubeMap(NiTPointerList<NiAVObject*>* apNodeList = nullptr, UInt32 auiMaxUpdates = 6, UInt32 auiCullMode = 4, bool abRenderWater = false);

	void RenderDefault();
	void RenderShadowMap(ShadowSceneLight* apLight);
};
ASSERT_SIZE(BSCubeMapCamera, 0x140u)