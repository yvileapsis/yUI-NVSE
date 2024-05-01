#pragma once
#include "TESBoundObject.hpp"
#include "TESModel.hpp"

// 0x68
class TESGrass :
	public TESBoundObject,
	public TESModel
{
public:
	TESGrass();
	~TESGrass();

	enum GRASS_WATER_STATE {
		GWS_ABOVE_ONLY_ATLEAST = 0x0,
		GWS_ABOVE_ONLY_ATMOST = 0x1,
		GWS_BELOW_ONLY_ATLEAST = 0x2,
		GWS_BELOW_ONLY_ATMOST = 0x3,
		GWS_BOTH_ATLEAST = 0x4,
		GWS_BOTH_ATMOST = 0x5,
		GWS_BOTH_ATMOST_ABOVE = 0x6,
		GWS_BOTH_ATMOST_BELOW = 0x7,
		GWS_COUNT = 0x8,
	};

	virtual SInt8				GetDensity();
	virtual void				SetDensity(SInt8 acDensity);
	virtual SInt8				GetMinSlopeDegrees();
	virtual void				SetMinSlopeDegrees(SInt8 acMinSlopeDegrees);
	virtual SInt8				GetMaxSlopeDegrees();
	virtual void				SetMaxSlopeDegrees(SInt8 acMaxSlopeDegrees);
	virtual SInt8				GetMinSlope();
	virtual SInt8				GetMaxSlope();
	virtual SInt16				GetDistanceFromWater();
	virtual void				SetDistanceFromWater(SInt16 asDistanceFromWater);
	virtual GRASS_WATER_STATE	GetUnderwaterState();
	virtual void				SetUnderwaterState(GRASS_WATER_STATE aeUnderwaterState);
	virtual float				GetPositionRange();
	virtual void				SetPositionRange(float afPositionRange);
	virtual float				GetHeightRange();
	virtual void				SetHeightRange(float afHeightRange);
	virtual float				GetColorRange();
	virtual void				SetColorRange(float afColorRange);
	virtual float				GetWavePeriod();
	virtual void				SetWavePeriod(float afWavePeriod);
	virtual bool				GetVertexLighting();
	virtual void				SetVertexLighting(bool abVertexLighting);
	virtual bool				GetUniformScaling();
	virtual void				SetUniformScaling(bool abUniformScaling);
	virtual bool				GetFitToSlope();
	virtual void				SetFitToSlope(bool abFitToSlope);

	struct GRASS_DATA {
		SInt8						cDensity;
		SInt8						cMinSlopeDegrees;
		SInt8						cMaxSlopeDegrees;
		SInt16						sDistanceFromWaterLevel;
		TESGrass::GRASS_WATER_STATE eUnderwater;
		Float32						fPositionRange;
		Float32						fHeightRange;
		Float32						fColorRange;
		Float32						fWavePeriod;
		SInt8						cFlags;
	};

	GRASS_DATA	kData;
};
static_assert(sizeof(TESGrass) == 0x68);