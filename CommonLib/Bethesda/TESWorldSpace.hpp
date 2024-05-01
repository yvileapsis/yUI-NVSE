#pragma once
#include "TESForm.hpp"
#include "TESFullName.hpp"
#include "TESTexture.hpp"
#include "BSMap.hpp"

class BGSTerrainManager;
class TESClimate;
class TESImageSpace;
class TESWaterForm;
class BGSMusicType;
class BGSEncounterZone;
class BGSImpactData;
class LODdata;

class TESObjectCell;

// 0xEC
class TESWorldSpace :
	public TESForm,
	public TESFullName,
	public TESTexture 
{
public:
	TESWorldSpace();
	virtual ~TESWorldSpace();
	virtual bool GetMapNameForLocation(void*, BSStringT<char>& akName, NiPoint3 akLocation);
	virtual void Func004F(float arg0, float a3, float a4, float a5, int a6, unsigned int a7);

	struct CoordXY {
		Float32	X;
		Float32	Y;
	};

	struct DCoordXY {
		SInt32	X;
		SInt32	Y;
	};

	struct WCoordXY {
		SInt16	X;
		SInt16	Y;
	};

	struct OffsetData
	{
		UInt32** unk000;	// 000 array of UInt32 stored in OFST record (indexed by relative CellXY).
		CoordXY min;		// 004 NAM0
		CoordXY max;		// 00C NAM9
		UInt32	fileOffset;	// 014 TESWorldSpace file offset in modInfo
	};

	struct MapData
	{
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

	enum EnumFlags : UInt8
	{
		SMALL_WORLD				= 1 << 0,
		NO_FAST_TRAVEL			= 1 << 1,
		UNK_2					= 1 << 2,
		UNK_3					= 1 << 3,
		NO_LOD_WATER			= 1 << 4,
		NO_LOD_NOISE			= 1 << 5,
		NO_NPC_FALL_DAMAGE		= 1 << 6,
		NEEDS_WATER_ADJUSTMENT	= 1 << 7,
	};

	NiTPointerMap<SInt32, TESObjectCELL*>*		pCellMap;			// 030 confirmed
	TESObjectCELL*						pPersistentCell;			// 034 should be the Permanent cell
	UInt32								unk038;						// 038
	BGSTerrainManager*					pTerrainManager;
	TESClimate*							pClimate;
	TESImageSpace*						pImageSpace;
	ImpactData*							pImpactSwap;
	EnumFlags							eFlags;						// 04C confirmed DATA
	UInt16								sParentUseFlags;			// 04E init'd to FF if has a parent. 5 is use ImageSpace, 4 is use parent climate, 3 is use parent Water, 1 is use parent LOD data, 0 is use parent LAND data
	NiTPointerMap<UInt32, BSSimpleList<TESObjectREFR*>*>		kFixedPersistentRefMap;			// 050 confirmed
	BSSimpleList<TESObjectREFR*>		kMobilePersistentRefs;		// 060
	NiTMap<UInt32, TESObjectREFR*>*		pOverlappedMultiboundMap;
	NiPointer<BSPortalGraph>			spPortalGraph;				// 068 confirmed as BSSimpleList
	TESWorldSpace*						pParentWorld;				// 070 confirmed
	TESWaterForm*						pWorldWater;
	TESWaterForm*						pLODWater;
	float								fWaterLODHeight;			// 07C confirmed NAM4
	MapData								mapData;					// 080 confirmed MNAM
	float								fWorldMapScale;				// 090 confirmed ONAM for three floats
	float								fWorldMapCellX;				// 094 confirmed
	float								fWorldMapCellY;				// 098 confirmed
	BGSMusicType*						pMusic;						// 09C confirmed ZNAM
	CoordXY								kMin;						// 0A0 confirmed NAM0 min of all Offset_Data.min
	CoordXY								kMax;						// 0A8 confirmed NAM9 max of all Offset_data.max
	NiTMap<TESFile*, OffsetData*>		kOffsetMap;					// 0B0 guarded by an isESM
	BSStringT<char>						kEditorID;					// 0C0
	float								fDefaultLandHeight;			// 0C8 confirmed DNAM for the two
	float								fDefaultWaterHeight;		// 0CC
	BGSEncounterZone*					pEncounterZone;				// 0D0 confirmed
	TESTexture							kCanopyShadow;				// 0D4 confirmed NNAM
	TESTexture							kWaterNoiseTexture;			// 0E0 confirmed XNAM

	BGSTerrainManager* GetTerrainManager();
	TESObjectCELL* GetCellAtCoord(SInt32 x, SInt32 y);
	bool GetNoLODWater() const;
	float GetWaterLODHeight() const;
	TESWaterForm* GetLODWaterType() const;
	BSPortalGraph* GetPortalGraph();
	void UpdateLODAnimations();
};
static_assert(sizeof(TESWorldSpace) == 0xEC);