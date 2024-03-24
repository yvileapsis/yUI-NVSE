#pragma once

#include "BSExtraData.hpp"
#include "BSMultiBoundNode.hpp"
#include "BSPortalGraph.hpp"
#include "BSRenderedTexture.hpp"
#include "ExtraDataList.hpp"
#include "NiSmartPointer.hpp"
#include "NiTMap.hpp"
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESTexture.hpp"
#include "bhkWorld.hpp"

class NiNode;
class NavMeshArray;
class TESObjectLAND;
class TESWorldSpace;
class TESObjectREFR;
class BGSLightingTemplate;
class DECAL_CREATION_DATA;

class TESObjectCELL : public TESForm, public TESFullName {
public:
	TESObjectCELL();
	~TESObjectCELL();

	typedef BSSimpleList<TESObjectREFR*> RefList;

	enum CELL_STATE {
		CS_NOTLOADED = 0x0,
		CS_UNLOADING = 0x1,
		CS_LOADING = 0x2,
		CS_LOADED = 0x3,
		CS_DETACHING = 0x4,
		CS_ATTACHING = 0x5,
		CS_ATTACHED = 0x6,
	};


	struct LoadedData {
		NiNodePtr											spCell3D;
		BSSimpleList<TESObjectREFR*>						kLargeAnimatedRefs;
		NiTMap<TESObjectREFR*, NiNode*>						kAnimatedRefs;
		// Form can be either TESRegion or TESObjectLIGH
		NiTMap<TESForm*, TESObjectREFR*>					kEmittanceSourceRefMap;
		NiTMap<TESObjectREFR*, NiNode*>						kEmittanceLightRefMap;
		NiTMap<TESObjectREFR*, NiPointer<BSMultiBoundNode>> kMultiboundRefMap;
		BSSimpleList<TESObjectREFR*>						kScriptedRefs;
		BSSimpleList<TESObjectREFR*>						kActivatingRefs;
		BSSimpleList<TESObjectREFR*>						kWaterRefs;
	};

	struct ExteriorData {
		SInt32 iCellX;
		SInt32 iCellY;
		UInt8 byte08;
	};

	struct InteriorData {
		UInt32		uiAmbient;
		UInt32		uiDirectional;
		UInt32		uiFogColorNear;
		float		fFogNear;
		float		fFogFar;
		SInt32		iDirectionalXY;
		SInt32		iDirectionalZ;
		float		fDirectionalFade;
		float		fClipDist;
		float		fFogPower;
		void*		pGetValuesFrom;
	};

	union CellData {
		ExteriorData* pCellDataExterior;
		InteriorData* pCellDataInterior;
	};

	enum CELLNODE {
		CN_ACTOR = 0x0,
		CN_MARKER = 0x1,
		CN_LAND = 0x2,
		CN_STATIC = 0x3,
		CN_DYNAMIC = 0x4,
		CN_OCCLUSION_PLANE = 0x5,
		CN_PORTAL = 0x6,
		CN_MULTIBOUND = 0x7,
		CN_COLLISION = 0x8,
		CN_LIGHT_MARKER = 0x9,
		CN_SOUND_MARKER = 0xA,
		CN_WATER_WADE = 0xB,
		CN_MAX = 0xC,
	};


	enum CELL_INHERIT_FLAGS {
		CIF_AMBIENT_COLOR = 0x1,
		CIF_DIRECTIONAL_COLOR = 0x2,
		CIF_FOG_COLOR = 0x4,
		CIF_FOG_NEAR = 0x8,
		CIF_FOG_FAR = 0x10,
		CIF_DIRECTIONAL_ROT = 0x20,
		CIF_DIRECTIONAL_FADE = 0x40,
		CIF_CLIP_DIST = 0x80,
		CIF_FOG_POWER = 0x100,
	};

	Bitfield8								ucCellFlags;
	Bitfield8								ucCellGameFlags;
	UInt8									cCellState;
	UInt8									byte27;
	ExtraDataList							kExtraList;
	CellData								pCellData;
	TESObjectLAND*							pCellLand;
	float									fWaterHeight;
	bool									bAutoWaterLoaded;
	TESTexture								noiseTexture;
	NavMeshArray*							pNavMeshes;
	UInt32									unk68[6];
	BSSpinLock								kSpinLock;
	SInt32									iCriticalQueuedRefCount;
	SInt32									iQueuedRefCount;
	UInt16									usCountVisibleDistant;
	UInt16									usVisibleWhenDistantCountLoaded;
	BSSimpleList<TESObjectREFR*>			kReferences;
	NiNodePtr								spLightMarkerNode;
	NiNodePtr								spSoundMarkerNode;
	UInt32									unkBC;
	TESWorldSpace*							pWorldSpace;
	LoadedData*								pLoadedData;
	float									fLODFadeOut;
	bool									byteCC;
	bool									byteCD;
	bool									bTriggeredLODHide;
	bool									bAlwaysFalse_CF;
	bool									bCanHideLOD;
	bool									bCellDetached;
	bool									bSkippedFade;
	bool									byteD3;
	BSPortalGraphPtr						spPortalGraph;
	BGSLightingTemplate*					pLightingTemplate;
	Bitfield32								uiInheritFlags;

	__forceinline bool IsInterior() const { return ucCellFlags.GetBit(1); }

	__forceinline bool HasWater() const { return ucCellFlags.GetBit(2); }

	__forceinline TESWorldSpace* GetWorldspace() const { return IsInterior() ? nullptr : pWorldSpace; }

	CELL_STATE GetState();

	bool IsAttached();
	bool IsLoaded();
	bool GetInheritFlag(CELL_INHERIT_FLAGS aeFlag) const;
	BGSLightingTemplate* GetLightingTemplate() const;
	float GetExteriorWaterHeight() const;
	void UpdateState(CELL_STATE aeState);

	static BSSimpleList<TESObjectREFR*>* GetAutoWaterReflectList();
	static BSSimpleList<TESObjectREFR*>* GetAutoWaterRefractList();

	NiNode* Get3D() const;

	NiNode* GetChildNode(CELLNODE aeNode) const;

	NiNode* GetGeometry(UInt32 auiIndex) const;

	static SInt32 GetCoord(SInt16 x, SInt16 y);
	SInt32 GetDataX() const;
	SInt32 GetDataY() const;

	bhkWorld* GetHavokWorld() const;

	InteriorData* GetInteriorData() const;
	ExteriorData* GetExteriorData() const;
	double GetDataClipDist() const;

	BSPortalGraph* CreatePortalGraph();
	void InitHavok();
	void AttachMultiBoundNodes();
	void CreateLoadedData();
	static NiNode* __fastcall Load3D(TESObjectCELL* apThis);
	void QueueReferences(bool abHighPriorityOnly);
	void AttachModels(bool abSetupMopp);
	TESObjectLAND* GetLand();

	void AddDecal(DECAL_CREATION_DATA* apDecalData, UInt32 aeDecalType, bool abForceAdd) const;

	void RenderLocalMaps(NiCamera* apCamera, BSRenderedTexturePtr& apTexture);
	void GenerateExteriorLocalMapTexture(BSRenderedTexturePtr& apTexture);
};

ASSERT_SIZE(TESObjectCELL::LoadedData, 0x64);
ASSERT_OFFSET(TESObjectCELL, pCellLand, 0x4C);
ASSERT_OFFSET(TESObjectCELL, spPortalGraph, 0xD4);
ASSERT_SIZE(TESObjectCELL, 0xE0);