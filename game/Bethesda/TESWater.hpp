#pragma once
#include "NiPlane.hpp"
#include "NiPoint2.hpp"
#include "NiNode.hpp"
#include "BSSimpleList.hpp"
#include "NiRefObject.hpp"
#include "NiTListBase.hpp"
#include "NiTStringMap.hpp"
#include "BSSoundHandle.hpp"

class NiCamera;
class BSRenderedTexture;
class BSShaderAccumulator;
class TESObjectREFR;
class TESWaterForm;

class TESWater
{
public:
	TESWater();
	~TESWater();

	struct WaterGroup
	{
		TESWaterForm*					pWaterForm;
		NiPlane							kMainPlane;
		NiPlane							kPlane014;
		NiTPointerList<TESObjectREFR*>	kWaterPlanes;
		NiTPointerList<TESObjectREFR*>	kRefsInWater;
		NiTPointerList<Actor*>			kActorsInWater;
		NiTPointerList<UInt32>			kList048;
		NiPointer<BSRenderedTexture>	spReflectionTexture;
		NiPointer<NiAVObject>			spRef058;
		bool							bUsesWaterLevel;
		bool							bIsVisible;
		bool							bRenderDepth;
		bool							bIsInterior;
		bool							bAllowLowDetailReflections;
		NiTPointerList<NiNode*>			kGeometryGroup;
		NiTPointerList<NiNode*>			kExplosionList;
		NiTPointerList<NiNode*>			kDepth_CellGeometryList;
		NiTPointerList<NiNode*>			kList088;
		NiPointer<BSShaderAccumulator>	spShaderAccum;
		NiPointer<BSShaderAccumulator>	spDepthShaderAccum;
		UInt32							uiReflectionGroupCount;
		UInt32							uiDepthGroupCount;
		NiPointer<NiCamera>				spCamera;
		NiPointer<NiCamera>				spDepthCamera;
		UInt32							uiStencilMask;

		static WaterGroup* CreateObject();

		__forceinline NiTPointerList<TESObjectREFR*>& GetPlanes() { return kWaterPlanes; };
	};
	static_assert(sizeof(WaterGroup) == 0xB0);

	struct WadingWaterData {
		NiPoint2 pair0;
		NiPoint2 pair8;
		NiPoint3 kPoint3;
	};

	UInt32												uiNumWaterGroups;
	UInt32												uiUnk004;
	NiPointer<NiRefObject>								spRT008;
	NiPointer<NiRefObject>								spRef00C;
	NiPointer<NiRefObject>								spRt010;
	NiPointer<NiRefObject>								spRt014;
	NiPointer<NiRefObject>								spRef018;
	NiPointer<NiSourceTexture>							spWaterNoiseTexture;
	NiPointer<NiRefObject>								spRef020;
	Float32												fBlendTimer;
	NiPoint2											kLastDisplaceOffset;
	UInt32												uiUnk30;
	BYTE												unk34;
	Float32												fUnk38;
	NiTPointerList<WaterGroup*>							kWaterGroups;
	WaterGroup*											pkWaterLOD;
	NiTPointerMap<TESObjectREFR*, TESObjectREFR*>		kRefMap04C;
	NiTPointerMap<TESObjectREFR*, TESObjectREFR*>		kRefMap05C;
	NiTPointerMap<TESWaterForm*, bool>					kWaterFormMap06C;
	NiTPointerMap<TESObjectREFR*, WadingWaterData*>		kWadingWaterMap;
	BSSoundHandle										kSoundHandle;
	Float32												fLastSplashTime;
	bool												bUnk09C;



	static bool bSkipNextUpdate;
	static bool* const bForceRender;
	static NiCamera* pCamera;
	static UInt32 uiActiveWaterGroups;
	static UInt32* const uiExteriorReflections;
	static UInt32* const uiCustomExteriorReflections;
	static bool* const bPauseTime;
	static NiPlane* const kWaterPlane;
	static NiPoint2* const kWaterFogDistances;

	static NiTPointerList<NiAVObject*> WaterLevelReflect_Nodes;
	static NiTPointerList<NiAVObject*> CustomWaterLevelReflect_Nodes;

	__forceinline NiTPointerList<WaterGroup*>& GetWaterGroups() { return kWaterGroups; };

	static __forceinline void SetIsUnderwater(bool abIsUnderwater) { *(bool*)0x11C7A59 = abIsUnderwater; }
	static __forceinline bool IsUnderwater() { return *(bool*)0x11C7A59; }
	static __forceinline bool IsUnderwater2() { return *(bool*)0x11C7A58; }

	static __forceinline bool IsWaterEnabled() { return *(bool*)0x1189624; }
	static __forceinline void SetWaterEnabled(bool abEnabled) { *(bool*)0x1189624 = abEnabled; }

	static __forceinline float GetCameraHeight() { return *(float*)0x11FF8A8; }
	static __forceinline void SetCameraHeight(float afHeight) { *(float*)0x11FF8A8 = afHeight; }

	static __forceinline bool IsInInterior() { return *(bool*)0x11FF101; }
	static __forceinline void SetIsInInterior(bool abIsInInterior) { *(bool*)0x11FF101 = abIsInInterior; }

	static __forceinline bool CanReflectAtCustomLvl() { return *(bool*)0x11C7A67; }
	static __forceinline void SetCanReflectAtCustomLvl(bool abCanReflect) { *(bool*)0x11C7A67 = abCanReflect; }

	static __forceinline bool CanReflectAtWaterLvl() { return *(bool*)0x11C7A66; }
	static __forceinline void SetCanReflectAtWaterLvl(bool abCanReflect) { *(bool*)0x11C7A66 = abCanReflect; }

	static __forceinline NiNode* GetWadeRoot() { return *(NiNodePtr*)0x11C7C28; }

	// Worldspace height
	static __forceinline NiCamera* GetWaterHeightCamera() { return *(NiPointer<NiCamera>*)0x11C7B44; }
	static __forceinline void SetWaterHeightCamera(NiCamera* apCamera) { *(NiPointer<NiCamera>*)0x11C7B44 = apCamera; }
	static __forceinline BSShaderAccumulator* GetWaterHeightAccumulator() { return *(NiPointer<BSShaderAccumulator>*)0x11C7CB8; }
	static __forceinline void SetWaterHeightAccumulator(BSShaderAccumulator* apAccumulator) { (*(NiPointer<BSShaderAccumulator>*)0x11C7CB8) = apAccumulator; }

	// Custom height
	static __forceinline NiCamera* GetCustomWaterHeightCamera() { return *(NiPointer<NiCamera>*)0x11C7CBC; }
	static __forceinline void SetCustomWaterHeightCamera(NiCamera* apCamera) { *(NiPointer<NiCamera>*)0x11C7CBC = apCamera; }
	static __forceinline BSShaderAccumulator* GetCustomWaterHeightAccumulator() { return *(NiPointer<BSShaderAccumulator>*)0x11C7BE8; }
	static __forceinline void SetCustomWaterHeightAccumulator(BSShaderAccumulator* apAccumulator) { (*(NiPointer<BSShaderAccumulator>*)0x11C7BE8) = apAccumulator; }

	// Textures
	static __forceinline NiPointer<BSRenderedTexture> GetReflectionTexture() { return *(NiPointer<BSRenderedTexture>*)0x11C7AD4; }
	static __forceinline void SetReflectionTexture(BSRenderedTexture* apTexture) { *(NiPointer<BSRenderedTexture>*)0x11C7AD4 = apTexture; }

	static __forceinline NiPointer<BSRenderedTexture> GetReflectionTextureCustom() { return *(NiPointer<BSRenderedTexture>*)0x11C7C2C; }
	static __forceinline void SetReflectionTextureCustom(BSRenderedTexture* apTexture) { *(NiPointer<BSRenderedTexture>*)0x11C7C2C = apTexture; }

	static __forceinline NiPointer<BSRenderedTexture> GetDepthTexture() { return *(NiPointer<BSRenderedTexture>*)0x11C7B68; }
	static __forceinline void SetDepthTexture(BSRenderedTexture* apTexture) { *(NiPointer<BSRenderedTexture>*)0x11C7B68 = apTexture; }

	static __forceinline NiPointer<BSRenderedTexture> GetWadeTexture() { return *(NiPointer<BSRenderedTexture>*)0x11C7B64; }
	static __forceinline void SetWadeTexture(BSRenderedTexture* apTexture) { *(NiPointer<BSRenderedTexture>*)0x11C7B64 = apTexture; }

	static __forceinline BSRenderedTexture* GetRefractionTexture() { return *(BSRenderedTexture**)0x11F943C; }
	static __forceinline void SetRefractionTexture(BSRenderedTexture* apTexture) { *(BSRenderedTexture**)0x11F943C = apTexture; }


	void ExteriorReflections_Accumulate(NiCamera* apCamera);
	void ExteriorReflections_Render();

	void CustomExteriorReflections_Accumulate(NiCamera* apCamera);
	void CustomExteriorReflections_Render();

	static void CalculateCameraPosition(NiCamera* apWorldCamera, NiPlane* apWaterPlane, NiCamera* apWaterCamera);

	static bool IsLODPresent();

	static void SetCurrentPlane(NiPlane akPlane);
	static NiPlane* GetCurrentPlane();

	void GetNoiseTexture(NiPointer<NiSourceTexture>& aspTexture) const;
	void SetNoiseTexture(NiSourceTexture* apTexture);

	void UpdateWaterSystem(NiCamera* apCamera, NiNode* apNode, BSShaderAccumulator* apAccum = nullptr);
	bool IsWaterObjectVisible(TESObjectREFR* apRefr, NiCamera* apCamera);
	NiTriShape* GetGeometry(TESObjectREFR* apRefr);

	static void* __fastcall RenderSurface_Hook(NiAVObjectPtr* apThis);
	void RenderSurface(WaterGroup* apWaterGroup);

	void RemoveWaterGroup(WaterGroup* apWaterGroup);
	void UpdateLOD(NiCamera* apCamera, bool abForceDisplay);
	void RenderWaterPlane(NiCamera* apCamera, WaterGroup* apWaterGroup, UInt16 usStencilMask);
	void RenderWater(NiCamera* apCamera, bool abForceDisplay);
	void Depth_Accumulate(NiCamera* apCamera, WaterGroup* apWaterGroup);
	void RenderDepthStencil(WaterGroup* apWaterGroup, UInt16 ausStencilMask);
	bool IsPlayerInWaterQuad(WaterGroup* apWaterGroup);
	bool ShouldHandleWaterGroupDisplacements();
	void HandleWaterGroupDisplacements();
	void RenderStaticRipples();
	void UpdateWaterAudio();

	void UpdateShaderForGroup(WaterGroup* apGroup);
	void UpdateShader(WaterGroup* apGroup, TESObjectREFR* apRefr);

	void SetClipPlanes(NiPlane akPlane, int unk);

	void InteriorReflections_Accumulate(NiCamera* apCamera, WaterGroup* apWaterGroup, bool abUseLowDetail);
	void InteriorReflections_Render(WaterGroup* apWaterGroup);

	void ToggleVisibility(bool abEnable, bool abUnk1, bool abToggle);

	void RemoveSubmergedReference(TESObjectREFR* apWaterRefr, Actor* apRefr, float afHeight);
	void RemoveSubmergedReferenceFromGroup(Actor* apRefr, WaterGroup* apWaterGroup);
	WaterGroup* GetWaterGroupForReference(TESObjectREFR* apWaterRef, TESObjectREFR* apLookupRefr, float afHeight) const;

	TESObjectREFR* AddLODWater(NiGeometry* apLODWater, TESWorldSpace* apWorldSpace, NiNode* apWaterLODRoot, BSMultiBoundNode* apBound, bool abLODWaterHeight);
};
static_assert(sizeof(TESWater) == 0xA0);