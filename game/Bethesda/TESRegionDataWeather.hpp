#pragma once

#include "TESRegionData.hpp"
#include "WeatherEntry.hpp"

class TESRegionDataWeather : public TESRegionData {
public:
	TESRegionDataWeather();
	~TESRegionDataWeather();

	TESWeatherList	weatherList;	// 08
};

ASSERT_SIZE(TESRegionDataWeather, 0x10)