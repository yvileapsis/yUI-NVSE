#pragma once

#include "TESForm.hpp"
#include "TESModel.hpp"
#include "TESWeather.hpp"
#include "TESTexture.hpp"
#include "WeatherEntry.hpp"

struct WeatherEntry;

class TESClimate : public TESForm {
public:
	TESClimate();
	~TESClimate();

	TESModel		kNightSky;
	TESWeatherList	kWeatherList;
	TESTexture		kSkyObjects[2];
	UInt8			ucData[6]; // sunriseBegin, sunriseEnd, sunsetBegin, sunsetEnd, volatility, phaseLength

	WeatherEntry* GetWeatherEntry(TESWeather* weather, bool rmv);
};

ASSERT_SIZE(TESClimate, 0x58);