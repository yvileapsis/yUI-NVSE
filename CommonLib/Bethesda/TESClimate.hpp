#pragma once
#include "TESForm.hpp"
#include "TESModel.hpp"
#include "TESTexture.hpp"

struct WeatherEntry;
class TESWeather;

// 0x58
class TESClimate : public TESForm
{
public:
	TESClimate();
	~TESClimate();

	TESModel			kNightSkyModel;
	BSSimpleList<TESWeather*> kWeatherList;
	TESTexture			kSunTexture;		// 38
	TESTexture			kSunGlareTexture;	// 44
	UInt8				ucSunriseBegin;		// 50
	UInt8				ucSunriseEnd;		// 51
	UInt8				ucSunsetBegin;		// 52
	UInt8				ucSunsetEnd;		// 53
	UInt8				ucVolatility;		// 54
	UInt8				ucPhaseLength;		// 55
	UInt8				pad56[2];			// 56

	WeatherEntry* GetWeatherEntry(TESWeather* weather, bool rmv);
};
static_assert(sizeof(TESClimate) == 0x58);