#pragma once

#include "BSRenderedTexture.hpp"
#include "BSPortalGraph.hpp"
#include "BSSimpleArray.hpp"
#include "NiLight.hpp"
#include "NiCamera.hpp"

class BSShaderAccumulator;
class NiGeometry;
class BSMultiBoundRoom;
class BSPortal;
class BSPortalSharedNode;
class BSCullingProcess;
class BSCubeMapCamera;

NiSmartPointer(ShadowSceneLight);

class ShadowSceneLight : public NiRefObject {
public:
	UInt32									unk008;
	float									fLuminance;
	D3DXMATRIX								kViewProjMatrix;
	D3DXMATRIX								kViewMatrix;
	D3DXMATRIX								kProjMatrix;
	float									fLODDimmer;
	float									fFade;
	float									fShadowFadeTime0D8;
	float									fShadowFadeTime0DC;
	NiTPointerList<NiPointer<NiGeometry>>	kGeometryList;
	bool									bIsShadowCasting;
	bool									byte0ED;
	NiAVObjectPtr							spObject0F0;
	bool									bPointLight;
	bool									bAmbientLight;
	NiLightPtr								spLight;
	bool									bDynamicLight;
	NiPoint3								kPointPosition;
	BSRenderedTexturePtr					spShadowRenderTarget;
	UInt16									usFrustumCull;
	NiAVObjectPtr							spObject114;
	bool									bUnk118;
	float									fUnk11C;
	float									fUnk120;
	bool									bShowDebugTexture;
	NiPointer<NiNode>						spShadowCasterNode;
	NiTPointerList<NiObjectPtr>				kList12C;
	NiTListItem<NiPointer<NiGeometry>>*		pGeomListFence;
	NiPointer<NiTriShape>					spFenceObject;
	NiPointer<BSCubeMapCamera>				spShadowMapCamera;
	NiFrustumPlanes							kFrustumPlanes;
	float									fClipPlanes[24];
	bool									bSetClipPlanes;
	NiPointer<BSShaderAccumulator>			spShadowAccum;
	BSSimpleArray<BSMultiBoundRoom*>		kMultiBoundRooms;
	BSSimpleArray<BSPortal*>				kPortals;
	BSSimpleArray<BSPortalSharedNode*>		kPortalSharedNodes;
	BSPortalGraph*							pPortalGraph;
	UInt32									unk244;
	UInt32									unk248;
	UInt32									unk24C;

	static ShadowSceneLight* Create();

	NiLight* GetLight() const;
	void SetLight(NiLight* apLight);
	NiNode* GetShadowCasterNode() const;
	void ToggleShadow(bool abEnableShadow);
	double GetLuminanceAtPoint(NiPoint3 aPoint, NiLight* apIgnore);
	double GetLuminanceToBound(NiBound* apBound);
	bool BoundWithinLightRadius(NiBound* apBound, NiLight* apLight,	float afRadiusScale);
	SInt32 __cdecl SortByLuminance(ShadowSceneLight** appLight1, ShadowSceneLight** appLight2);
	void ParseObjectAddRadius(NiAVObject* apObject);
	void CreateFence();
	void UpdateShadowFadeTimes(float afiTme, bool abNoFade);

	void ClearGeometry();
	void ClearGeometryFence();
	void ClearRenderPasses();

	void AccumulateGeometry(BSCullingProcess* apCuller, UInt32 auiLightNumber);
	void RenderShadowMap(BSCullingProcess* apCuller, UInt32 auiLightNumber);
	void AddGeometry(NiGeometry* apGeometry);
	void RemoveGeometry(NiGeometry* apGeometry, NiTListIterator apIter = nullptr);
};
// TODO: deal with shadow scene nonsense later
// ASSERT_SIZE(ShadowSceneLight, 0x250);