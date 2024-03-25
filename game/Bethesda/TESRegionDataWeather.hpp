#pragma once
#include "TESRegionData.hpp"
#include "WeatherEntry.hpp"

// 0x10
class TESRegionDataWeather : public TESRegionData {
public:
	TESRegionDataWeather();
	~TESRegionDataWeather();

	BSSimpleList<WeatherEntry*> kWeatherList;	// 08
};
static_assert(sizeof(TESRegionDataWeather) == 0x10);