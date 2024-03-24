#pragma once

#include "BSSimpleList.hpp"

class TESWeather;
class TESGlobal;

struct WeatherEntry {
	TESWeather* pWeather;
	UInt32 uiChance;
	TESGlobal* pChanceVar;

	void Set(TESWeather* pWtr, UInt32 pChn, TESGlobal* pGlb)
	{
		pWeather = pWtr;
		uiChance = pChn;
		pChanceVar = pGlb;
	}
};
typedef BSSimpleList<WeatherEntry*> TESWeatherList;