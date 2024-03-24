#pragma once

#include "NiNode.hpp"
#include "ShadowSceneLight.hpp"
#include "BSCompoundFrustum.hpp"
#include "BSCubeMapCamera.hpp"
#include "DList.hpp"
#include "BSFogProperty.hpp"

class BSCullingProcess;

NiSmartPointer(ShadowSceneNode);

class ShadowSceneNode : public NiNode {
public:
	UInt32									unk0AC[2];
	NiTPointerList<ShadowSceneLightPtr>		kLights;
	NiTPointerList<ShadowSceneLightPtr>		kActorShadowCasters;
	NiTListIterator							kActorShadowCastersIter;
	NiTPointerList<ShadowSceneLightPtr>*	pListNode0D0;
	NiTPointerList<ShadowSceneLightPtr>*	pListNode0D4;
	ShadowSceneLightPtr						spDummyLight[2]; // Empty lights to fill kActorShadowCasters
	ShadowSceneLight*						pSunLight;		
	NiTPointerList<ShadowSceneLightPtr>		kLightQueueAdd;
	NiTPointerList<ShadowSceneLightPtr>		kLightQueueRemove;
	NiTPointerList<ShadowSceneLightPtr>		kDynamicLights;
	NiTPointerList<NiNodePtr>				kList108;
	NiTPointerList<NiNodePtr>				kLitActors;
	UInt8									cSceneGraphIndex;
	NiObjectPtr								spObject124;
	NiPointer<BSCubeMapCamera>				spShadowMapCubeCam;
	UInt32									unk12C;
	bool									bDisableLightUpdate;
	bool									bWireframe;
	BSFogPropertyPtr						spFog;
	BSCompoundFrustum						kCompoundFrustum;
	bool									bVisibleUnboundSpace;
	BSPortalGraph*							pBSPortalGraph;
	NiPoint3								kLightingOffset;
	NiPoint3								kCameraPos;
	bool									bAllowLightRemoveQueues;

	NIRTTI_ADDRESS(0x11F9E80);

#if _EDITOR
	NiObjectPtr spObject200;
	NiObjectPtr spObject204;
	NiNodePtr spDebugNode;
	UInt32 uiDebugFlags;
	NiCameraPtr spFrozenCamera;
	NiCameraPtr spRestoreCamera;
#endif

	static NiPointer<NiCamera>				spRestoreCamera;
	static NiPointer<NiCamera>				spFrozenCamera;

	static BSSpinLock* const pLightQueueLock;
	static BSSpinLock* const pLitObjectsLock;

	BSPortalGraph* GetPortalGraph() const { return pBSPortalGraph; };

	static ShadowSceneNode* Create();

	static void __fastcall OnVisibleEx(ShadowSceneNode* apThis, void*, BSCullingProcess* apCuller);

	static BSFogProperty* GetFogProperty(UInt32 aeType);

	void DisableLightUpdate(bool abDisable);

	double GetLuminanceAtPoint(NiPoint3 aPoint, int& aiNumberlights, float& afAmbientLum, float& afDiffuseLum, NiLight* apIgnore);

	void SetShadowCastToLight(NiLight* apLight, bool bEnableShadow) const;

	void PreOnVisible(BSCullingProcess* apCuller);

	ShadowSceneLight* GetShadowCaster();
	ShadowSceneLight* GetNextShadowCaster();
	ShadowSceneLight* AddShadowCaster(NiNode* apCaster);

	void RenderShadowMaps(BSCullingProcess* apCuller);
	void ResetShadowCasters() const;
	void ClearShadows();

	void ProcessQueuedLights();

	static void ClearLightsFromGeometry(NiGeometry* apGeometry);

	ShadowSceneLight* AddLight(NiLight* apLight, bool abDynamic);
	void RemoveLight(NiLight* apLight);
	void RemoveLight(ShadowSceneLightPtr& arLight);
	void RemoveAllLights();

	UInt32 AddAllLightsFromNode(NiNode* apNode, float afRadius);

	void BuildSharedCompoundFrustum(NiCamera* apCamera, BSPortal* apPortal);

	void UpdateMultiBoundVisibility(BSMultiBoundNode* apNode, NiPoint3 akViewPoint, BSCullingProcess& arCuller) const;
	void UpdateRoomAndPortalVisibility(BSMultiBoundRoom* apRoom, NiCamera* apCamera, NiFrustumPlanes* apPlanes, bool abAddIntersection);

	void AddObject(NiNode* apNode, bool abIsActor);
	void AddVisibleObject(NiNode* apNode);
};

static_assert(sizeof(ShadowSceneNode) == 0x200);