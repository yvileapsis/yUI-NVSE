#pragma once

#include "TESWater.hpp"
#include "Sky.hpp"
#include "GridCellArray.hpp"

class TESCreature;
class NavMeshInfoMap;
class LoadedAreaBound;
class NiFogProperty;
class TESWorldSpace;
class ImageSpaceModifierInstance;
class TESObjectREFR;
class QueuedFile;
class GridDistantArray;
class BSTempNodeManager;

class TES {
public:
	virtual bool Fn_00(UInt32 arg1, UInt32 arg2, UInt32 arg3, UInt32 arg4, TESWorldSpace* apWorldSpace);

	struct DeathCount {
		TESCreature*	pkCreatureOrNPC;
		UInt16			usCount;
	};

	GridDistantArray*				pGridDistantArray;
	GridCellArray*					pGridCellArray;
	NiNode*							pObjRoot;
	NiNode*							pLODRoot;
	NiNode*							pObjLODWaterRoot;
	BSTempNodeManager*				pTempNodeManager;
	NiDirectionalLight*				pObjLight;
	NiFogProperty*					pObjFog;
	SInt32							iCurrentGridX;
	SInt32							iCurrentGridY;
	SInt32							iCurrentQueuedX;
	SInt32							iCurrentQueuedY;
	TESObjectCELL*					pInteriorCell;
	TESObjectCELL**					pInteriorBuffer;
	TESObjectCELL**					pExteriorBuffer;
	UInt32							uiTempInteriorBufferSize;
	UInt32							uiTempExteriorBufferSize;
	SInt32							iSaveGridX;
	SInt32							iSaveGridY;
	UInt8							byte50;
	bool							bRunningCellTests;
	UInt8							byte52;
	UInt8							gap53;
	UInt32							renderTestCellsCallback;
	UInt32							unk58;
	UInt32							unk5C;
	bool							bShowLandBorders;
	TESWater*						pWaterManager;
	Sky*							pSky;
	BSSimpleList<NiPointer<ImageSpaceModifierInstance>> kActiveIMODs;
	UInt32							unk74;
	UInt32							unk78;
	UInt8							byte7C;
	UInt8							byte7D;
	UInt8							gap7E;
	UInt8							gap7F;
	Float32							fCell_delta_x;
	Float32							fCell_delta_y;
	TESWorldSpace*					pWorldSpace;
	UInt32*							list8C[2];
	BSSimpleList<TESObjectREFR*>	kCellFurnitureList;
	BSSimpleList<DeathCount*>		kDeathCounts;
	NiPointer<QueuedFile>			spQueuedFileA4;
	NiSourceTexturePtr				spBloodDecalPreload1;
	NiPointer<QueuedFile>			spQueuedFileAC;
	void*							pParticleCacheHead;
	bool							bFadeWhenLoading;
	bool							bAllowUnusedPurge;
	UInt8							byteB6;
	UInt8							byteB7;
	UInt32							iPlaceableWaterCount;
	NavMeshInfoMap*					pNavMeshInfoMap;
	NiPointer<LoadedAreaBound>		spLoadedAreaBound;

	__forceinline static TES* GetSingleton() { return *(TES**)0x11DEA10; }

	static Sky* GetSky();

	static TESWater* GetWaterManger();

	static TESWorldSpace* GetWorldSpace();

	static TESObjectCELL* GetInterior();
	TESObjectCELL* GetCurrentCell();
	TESObjectCELL* GetGridCellCell(SInt32 aiX, SInt32 aiY);

	static void AddTempDebugObject(NiAVObject* apObject, float afTime);

	static bool IsCellLoaded(TESObjectCELL* apCell, bool abIgnoreBuffered);

	GridCell* GetGridCell(SInt32 aX, SInt32 aY);

	bool GetLandHeight(const NiPoint3& akPosition, float* afHeight) const;
	NiGeometry* GetLandGeometry(const NiPoint2& arPosition) const;

	void CreateTextureImage(const char* apPath, NiSourceTexturePtr& aspTexture, bool abNoFileOK, bool abArchiveOnly);

	bool FreeTexture(const char* apPath, UInt32 auiMinRefCount = 0);

	void UpdateMultiBoundVisibility(NiCamera* apCamera);
	void ResetAllMultiBoundNodes(bool abLandscape, bool abRooms);

	__forceinline bool GetTerrainHeight(NiPoint2* posXY, float* result)
	{
		return ThisCall<bool>(0x4572E0, this, posXY, result);
	}

	__forceinline void AddTempNode(NiAVObject* object, float lifetime)
	{
		ThisCall(0x458E20, this, object, lifetime);
	}
};
static_assert(sizeof(TES) == 0xC4);

extern TES* g_TES;