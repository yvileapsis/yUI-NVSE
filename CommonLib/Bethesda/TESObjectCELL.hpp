#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"

#include "BSSimpleArray.hpp"
#include "NiSmartPointer.hpp"
#include "TESTexture.hpp"
#include "ExtraDataList.hpp"
#include "NiTMap.hpp"

// TOOD: Research into the fact maps require full definitions
#include "BSMultiBoundNode.hpp"

class bhkWorld;
class NiNode;
class NiCamera;
class NavMesh;
class BSPortalGraph;
class BSRenderedTexture;
class TESObjectLAND;
class TESWorldSpace;
class TESObjectREFR;
class BGSLightingTemplate;
class DECAL_CREATION_DATA;

// 0xE0
class TESObjectCELL :
	public TESForm,
	public TESFullName
{
public:
	TESObjectCELL();
	~TESObjectCELL() override;

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

	struct LoadedData
	{
		NiPointer<NiNode>									spCell3D;
		BSSimpleList<TESObjectREFR*>						kLargeAnimatedRefs;
		NiTMap<TESObjectREFR*, NiNode*>						kAnimatedRefA;

		// Form can be either TESRegion or TESObjectLIGH
		NiTMap<TESForm*, TESObjectREFR*>					kEmittanceSourceRefMap;
		NiTMap<TESObjectREFR*, NiNode*>						kEmittanceLightRefMap;
		NiTMap<TESObjectREFR*, NiPointer<BSMultiBoundNode>>	kMultiboundRefMap;
		BSSimpleList<TESObjectREFR*>						kScriptedRefs;
		BSSimpleList<TESObjectREFR*>						kActivatingRefs;
		BSSimpleList<TESObjectREFR*>						kWaterRefs;
	};
	static_assert(sizeof(LoadedData) == 0x64);

	struct ExteriorData {
		SInt32 iCellX;
		SInt32 iCellY;
		UInt8 byte08;
	};

	struct InteriorData {
		UInt32		uiAmbient;
		UInt32		uiDirectional;
		UInt32		uiFogColorNear;
		Float32		fFogNear;
		Float32		fFogFar;
		SInt32		iDirectionalXY;
		SInt32		iDirectionalZ;
		Float32		fDirectionalFade;
		Float32		fClipDist;
		Float32		fFogPower;
		UInt32*		pGetValuesFrom;
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

	UInt8									cCellFlags;
	UInt8									cCellGameFlags;
	UInt8									cCellState;
	UInt8									byte27;
	ExtraDataList							kExtraList;
	CellData								pCellData;
	TESObjectLAND*							pCellLand;
	Float32									fWaterHeight;
	bool									bAutoWaterLoaded;
	TESTexture								kNoiseTexture;
	BSSimpleArray<NavMesh>*					pkNavMeshes;
	UInt32									unk68[6];
	BSSpinLock								kSpinLock;
	SInt32									iCriticalQueuedRefCount;
	SInt32									iQueuedRefCount;
	UInt16									usCountVisibleDistant;
	UInt16									usVisibleWhenDistantCountLoaded;
	BSSimpleList<NiPointer<TESObjectREFR>>	kspReferencesA;
	NiPointer<NiNode>						spLightMarkerNode;
	NiPointer<NiNode>						spSoundMarkerNode;
	UInt32									unkBC;
	TESWorldSpace*							pkWorldSpace;
	LoadedData*								pkLoadedData;
	Float32									fLODFadeOut;
	bool									byteCC;
	bool									byteCD;
	bool									bTriggeredLODHide;
	bool									bAlwaysFalse_CF;
	bool									bCanHideLOD;
	bool									bCellDetached;
	bool									bSkippedFade;
	bool									byteD3;
	NiPointer<BSPortalGraph>				spPortalGraph;
	BGSLightingTemplate*					pkLightingTemplate;
	UInt32									uiInheritFlags;

	__forceinline bool IsInterior() { return (eFlags & 1) != 0; }

	__forceinline bool HasWater() { return (eFlags & 2) != 0; }

	__forceinline TESWorldSpace* GetWorldspace() { return IsInterior() ? nullptr : pkWorldSpace; }

	CELL_STATE GetState();

	bool IsAttached();
	bool IsLoaded();
	bool GetInheritFlag(CELL_INHERIT_FLAGS aeFlag) const;
	BGSLightingTemplate* GetLightingTemplate() const;
	Float32 GetExteriorWaterHeight() const;
	void UpdateState(CELL_STATE aeState);

	static BSSimpleList<TESObjectREFR*>* GetAutoWaterReflectList();
	static BSSimpleList<TESObjectREFR*>* GetAutoWaterRefractList();

	NiNode* Get3D() const;

	NiNode* GetChildNode(CELLNODE aeNode) const;

	NiNode* GetGeometry(UInt32 auiIndex) const;

	static SInt32 GetCoord(SInt16 asX, SInt16 asY);
	SInt32 GetDataX() const;
	SInt32 GetDataY() const;

	bhkWorld* GetHavokWorld() const;

	InteriorData* GetInteriorData() const;
	ExteriorData* GetExteriorData() const;
	Float64 GetDataClipDist() const;

	BSPortalGraph* CreatePortalGraph();
	void InitHavok();
	void AttachMultiBoundNodes();
	void CreateLoadedData();
	static NiNode* __fastcall Load3D(TESObjectCELL* apThis);
	void QueueReferences(bool abHighPriorityOnly);
	void AttachModels(bool abSetupMopp);
	TESObjectLAND* GetLand();

	void AddDecal(DECAL_CREATION_DATA* apDecalData, UInt32 aeDecalType, bool abForceAdd) const;

	void RenderLocalMaps(NiCamera* apCamera, NiPointer<BSRenderedTexture>& apTexture);
	void GenerateExteriorLocalMapTexture(NiPointer<BSRenderedTexture>& apTexture);
};
static_assert(sizeof(TESObjectCELL) == 0xE0);