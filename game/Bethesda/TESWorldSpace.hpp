#pragma once

#include "TESForm.hpp"
#include "TESTexture.hpp"
#include "TESObjectCELL.hpp"
#include "BSPortalGraph.hpp"
#include "BSSimpleArray.hpp"
#include "BSMap.hpp"

class BGSTerrainManager;
class TESClimate;
class TESImageSpace;
class TESWaterForm;
class BGSMusicType;
class BGSEncounterZone;
class BGSImpactData;
class LODdata;

class TESWorldSpace : public TESForm, public TESFullName, public TESTexture {
public:
	TESWorldSpace();
	virtual ~TESWorldSpace();
	virtual bool GetMapNameForLocation(void*, BSStringT& arName, NiPoint3 akLocation);
	virtual void Func004F(float arg0, float a3, float a4, float a5, int a6, unsigned int a7);

	struct CoordXY {
		float	X;
		float	Y;
	};

	struct DCoordXY {
		SInt32	X;
		SInt32	Y;
	};

	struct WCoordXY {
		SInt16	X;
		SInt16	Y;
	};

	struct OFFSET_DATA {
		UInt32** unk000;	// 000 array of UInt32 stored in OFST record (indexed by relative CellXY).
		CoordXY min;		// 004 NAM0
		CoordXY max;		// 00C NAM9
		UInt32	fileOffset;	// 014 TESWorldSpace file offset in modInfo
	};

	struct MapData {
		DCoordXY	usableDimensions;
		WCoordXY	cellNWCoordinates;
		WCoordXY	cellSECoordinates;
	};	// 010

	struct ImpactData {
		typedef BSMap<BGSImpactData*, BGSImpactData*> ImpactImpactMap;
		enum MaterialType
		{
			eMT_Stone = 0,
			eMT_Dirt,
			eMT_Grass,
			eMT_Glass,
			eMT_Metal,
			eMT_Wood,
			eMT_Organic,
			eMT_Cloth,
			eMT_Water,
			eMT_HollowMetal,
			eMT_OrganicBug,
			eMT_OrganicGlow,

			eMT_Max
		};

		ImpactImpactMap	impactImpactMap[eMT_Max];	// 000
		char			footstepMaterials[0x12C];	// 030
	};

	enum Flags {
		SMALL_WORLD				= 1 << 0,
		NO_FAST_TRAVEL			= 1 << 1,
		UNK_2					= 1 << 2,
		UNK_3					= 1 << 3,
		NO_LOD_WATER			= 1 << 4,
		NO_LOD_NOISE			= 1 << 5,
		NO_NPC_FALL_DAMAGE		= 1 << 6,
		NEEDS_WATER_ADJUSTMENT	= 1 << 7,
	};

	typedef NiTPointerMap<UInt32, BSSimpleList<TESObjectREFR*>*>	RefListPointerMap;
	typedef NiTPointerMap<SInt32, TESObjectCELL*>					CellPointerMap;
	typedef NiTMap<TESFile*, TESWorldSpace::OFFSET_DATA*>			OffsetDataMap;

	CellPointerMap*						pCellMap;
	TESObjectCELL*						pPersistentCell;
	UInt32								kTerrainLODManager; // Unused
	BGSTerrainManager*					pTerrainManager;
	TESClimate*							pClimate;
	TESImageSpace*						pImageSpace;
	ImpactData*							pImpactSwap;
	Bitfield8							cFlags;
	Bitfield16							sParentUseFlags;
	RefListPointerMap					kFixedPersistentRefMap;
	BSSimpleList<TESObjectREFR*>		kMobilePersistentRefs;
	NiTMap<UInt32, TESObjectREFR*>*		pOverlappedMultiBoundMap;
	BSPortalGraphPtr					spPortalGraph;
	TESWorldSpace*						pParentWorld;
	TESWaterForm*						pWorldWater;
	TESWaterForm*						pLODWater;
	float								fWaterLODHeight;
	MapData								kMapData;
	float								fWorldMapScale;
	float								fWorldMapCellX;
	float								fWorldMapCellY;
	BGSMusicType*						pMusic;
	CoordXY								kMin;
	CoordXY								kMax;
	OffsetDataMap						kOffsetMap;
	BSStringT							strEditorID;
	float								fDefaultLandHeight;
	float								fDefaultWaterHeight;
	BGSEncounterZone*					pEncounterZone;
	TESTexture							kCanopyShadow;
	TESTexture							kWaterNoiseTexture;

	BGSTerrainManager* GetTerrainManager() const;
	TESObjectCELL* GetCellAtCoord(SInt32 x, SInt32 y);
	TESObjectCELL* GetCellFromKey(SInt32 aiKey);
	bool GetNoLODWater() const;
	float GetWaterLODHeight() const;
	TESWaterForm* GetLODWaterType() const;
	BSPortalGraph* GetPortalGraph();

	void UpdateLODAnimations();
};

ASSERT_SIZE(TESWorldSpace, 0xEC);